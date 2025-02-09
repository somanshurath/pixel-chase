#include "ClientLayer.h"

#include "Walnut/Input/Input.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include <Walnut/ImGui/ImGuiTheme.h>

#include "Walnut/Serialization/BufferStream.h"
#include "ServerPacket.h"

#include "./layout/ControlCenterer.h"

namespace PixelChase
{
    static Walnut::Buffer s_ScratchBuffer;

    static void DrawRect(glm::vec2 rectPos, float width, float height, int color)
    {
        ImDrawList *drawList = ImGui::GetBackgroundDrawList();
        ImVec2 min = ImGui::GetWindowPos() + ImVec2(rectPos.x, rectPos.y);
        ImVec2 max = min + ImVec2(width, height);

        drawList->AddRectFilled(min, max, color);
    }

    void ClientLayer::OnAttach()
    {
        s_ScratchBuffer.Allocate(1024 * 1024 * 30); // 10MB

        m_Client.SetDataReceivedCallback([this](const Walnut::Buffer buffer)
                                         { OnDataReceived(buffer); });
    }

    void ClientLayer::OnDetach()
    {
        s_ScratchBuffer.Release();
    }

    void ClientLayer::OnUpdate(float ts)
    {
        if (!(m_Client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connected))
            return;

        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Escape))
            Walnut::Application::Get().Close();

        glm::vec2 direction{0.0f, 0.0f};
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W) || Walnut::Input::IsKeyDown(Walnut::KeyCode::Up))
            direction.y -= 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S) || Walnut::Input::IsKeyDown(Walnut::KeyCode::Down))
            direction.y += 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A) || Walnut::Input::IsKeyDown(Walnut::KeyCode::Left))
            direction.x -= 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D) || Walnut::Input::IsKeyDown(Walnut::KeyCode::Right))
            direction.x += 1.0f;

        float length = glm::length(direction);
        if (length > 0.0f)
        {
            direction /= length;
            m_PlayerVelocity = direction * 200.0f;
        }

        m_PlayerPosition += m_PlayerVelocity * ts;
        m_PlayerVelocity = glm::mix(m_PlayerVelocity, glm::vec2(0.0f), 0.3f);

        Walnut::BufferStreamWriter stream(s_ScratchBuffer);
        stream.WriteRaw(PacketType::ClientUpdate);
        stream.WriteRaw<glm::vec2>(m_PlayerPosition);
        stream.WriteRaw<glm::vec2>(m_PlayerVelocity);
        m_Client.SendBuffer(stream.GetBuffer());
    }

    void ClientLayer::OnUIRender()
    {
        Walnut::Client::ConnectionStatus status = m_Client.GetConnectionStatus();
        if (status == Walnut::Client::ConnectionStatus::Connected)
        {
            DrawRect(m_PlayerPosition, 100, 100, IM_COL32(255, 0, 0, 255));

            m_PlayerDataMutex.lock();
            std::map<uint32_t, PlayerData> playerData = m_PlayerData;
            m_PlayerDataMutex.unlock();

            for (auto &[id, data] : playerData)
            {
                if (id == m_PlayerID)
                    continue;

                DrawRect(data.Position, 100, 100, IM_COL32(0, 255, 0, 255));
            }
        }
        else
        {
            bool readOnly = status != Walnut::Client::ConnectionStatus::Disconnected;

            ImGui::Begin("Connect to Server");
            ImGui::PushItemWidth(-1);
            ImGuiInputTextFlags flags = 0;
            flags |= ImGuiInputTextFlags_CharsNoBlank;
            flags |= (readOnly ? ImGuiInputTextFlags_ReadOnly : 0);

            ImGui::InputText(" Server Address", &m_ServerAddress, flags);
            ImGui::PopItemWidth();
            ImColor color = ImColor(Walnut::UI::Colors::Theme::text);
            std::string statusString = "";
            if (status == Walnut::Client::ConnectionStatus::Connecting)
            {
                color = ImColor(Walnut::UI::Colors::Theme::textDarker);
                statusString = " Connecting...";
            }
            else if (status == Walnut::Client::ConnectionStatus::Disconnected && m_ServerConnAttempts)
            {
                color = ImColor(Walnut::UI::Colors::Theme::textError);
                statusString = " Failed to connect. Try Again";
            }

            if (ImGui::Button("Connect"))
            {
                m_Client.ConnectToServer(m_ServerAddress);
                m_ServerConnAttempts++;
            }
            ImGui::SameLine();

            if (statusString != "")
                ImGui::TextColored(color, statusString.c_str());

            ImGui::End();
        }
    }

    void ClientLayer::OnDataReceived(const Walnut::Buffer buffer)
    {
        Walnut::BufferStreamReader stream(buffer);

        PacketType packetType;
        stream.ReadRaw(packetType);

        switch (packetType)
        {
        case PacketType::ClientConnect:
        {
            uint32_t idFromServer;
            stream.ReadRaw<uint32_t>(idFromServer);
            WL_INFO_TAG("Client", "Connected to server");
            WL_INFO_TAG("Client", "Assigned ID: {}", idFromServer);
            WL_INFO_TAG("Client", "Our ID: {}", m_Client.GetID());
            m_PlayerID = idFromServer;
            break;
        }
        case PacketType::ClientUpdate:
        {
            m_PlayerDataMutex.lock();
            stream.ReadMap(m_PlayerData);
            m_PlayerDataMutex.unlock();
            break;
        }
        break;
        }
    }
}
