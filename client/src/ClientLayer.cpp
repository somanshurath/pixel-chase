#include "ClientLayer.h"

#include "Walnut/Input/Input.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include <Walnut/ImGui/ImGuiTheme.h>

namespace PixelChase
{
    static void DrawRect(glm::vec2 rectPos, float width, float height, int color)
    {
        ImDrawList *drawList = ImGui::GetBackgroundDrawList();
        ImVec2 min = ImGui::GetWindowPos() + ImVec2(rectPos.x, rectPos.y);
        ImVec2 max = min + ImVec2(width, height);

        drawList->AddRectFilled(min, max, color);
    }

    void ClientLayer::OnAttach()
    {
        m_Client.SetDataReceivedCallback([this](const Walnut::Buffer buffer)
                                         { OnDataReceived(buffer); });
    }

    void ClientLayer::OnDetach()
    {
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
    }

    void ClientLayer::OnUIRender()
    {
        Walnut::Client::ConnectionStatus status = m_Client.GetConnectionStatus();
        if (status == Walnut::Client::ConnectionStatus::Connected)
        {
            DrawRect(m_PlayerPosition, 100, 100, IM_COL32(255, 0, 0, 255));
        }
        else
        {
            bool readOnly = status != Walnut::Client::ConnectionStatus::Disconnected;

            ImGui::Begin("Connect to Server");
            ImGui::InputText("Server Address", &m_ServerAddress, ImGuiInputTextFlags_CharsNoBlank | (readOnly ? ImGuiInputTextFlags_ReadOnly : 0));

            ImColor color = ImColor(Walnut::UI::Colors::Theme::text);
            std::string statusString = "";
            if (status == Walnut::Client::ConnectionStatus::Connecting)
            {
                color = ImColor(Walnut::UI::Colors::Theme::textDarker);
                statusString = "Connecting...";
            }
            else if (status == Walnut::Client::ConnectionStatus::Disconnected && m_ServerConnAttempts)
            {
                color = ImColor(Walnut::UI::Colors::Theme::textError);
                statusString = "Failed to connect. Try Again";
            }

            if (statusString != "")
                ImGui::TextColored(color, statusString.c_str());

            if (ImGui::Button("Connect"))
            {
                m_Client.ConnectToServer(m_ServerAddress);
                m_ServerConnAttempts++;
            }

            ImGui::End();
        }
    }

    void ClientLayer::OnDataReceived(const Walnut::Buffer buffer)
    {
    }
}
