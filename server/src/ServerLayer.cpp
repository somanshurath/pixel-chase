#include "ServerLayer.h"

#include "Walnut/Application.h"
#include "Walnut/Core/Log.h"
#include "Walnut/Serialization/BufferStream.h"

#include "ServerPacket.h"

namespace PixelChase
{
	static Walnut::Buffer s_ScratchBuffer;

	void ServerLayer::OnAttach()
	{
		s_ScratchBuffer.Allocate(1024 * 1024 * 10); // 10MB

		m_Console.SetMessageSendCallback([this](std::string_view message)
										 { OnConsoleMessage(message); });
		m_Server.SetClientConnectedCallback([this](const Walnut::ClientInfo &clientInfo)
											{ OnClientConnected(clientInfo); });
		m_Server.SetClientDisconnectedCallback([this](const Walnut::ClientInfo &clientInfo)
											   { OnClientDisconnected(clientInfo); });
		m_Server.SetDataReceivedCallback([this](const Walnut::ClientInfo &clientInfo, const Walnut::Buffer buffer)
										 { OnDataReceived(clientInfo, buffer); });
		m_Server.Start();
	}

	void ServerLayer::OnDetach()
	{
		m_Server.Stop();
	}

	void ServerLayer::OnUpdate(float ts)
	{
	}

	void ServerLayer::OnUIRender()
	{
	}

	void ServerLayer::OnConsoleMessage(std::string_view message)
	{
		if (message.starts_with('/'))
		{
			if (message == "/exit")
			{
				Walnut::Log::PrintMessageTag(Walnut::Log::Type::Core, Walnut::Log::Level::Info, "Server", "Received exit command. Shutting down server...");
				Walnut::Application::Get().Close();
			}
		}
	}
	void ServerLayer::OnClientConnected(const Walnut::ClientInfo &clientInfo)
	{
		WL_INFO_TAG("Server", "Client connected: {}", clientInfo.ID);

		Walnut::BufferStreamWriter stream(s_ScratchBuffer);
		stream.WriteRaw(PacketType::ClientConnect);
		stream.WriteRaw(clientInfo.ID);
		m_Server.SendBufferToClient(clientInfo.ID, stream.GetBuffer());
	}
	void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo &clientInfo)
	{
		WL_INFO_TAG("Server", "Client disconnected: {}", clientInfo.ID);
	}
	void ServerLayer::OnDataReceived(const Walnut::ClientInfo &clientInfo, const Walnut::Buffer buffer)
	{
		Walnut::BufferStreamReader stream(buffer);

		PacketType packetType;
		stream.ReadRaw(packetType);

		switch (packetType)
		{
		case PacketType::ClientUpdate:
		{
			glm::vec2 pos, vel;
			stream.ReadRaw<glm::vec2>(pos);
			stream.ReadRaw<glm::vec2>(vel);

			WL_INFO_TAG("Server", "ClientUpdate: ID: {}, Pos: ({}, {}), Vel: ({}, {})", clientInfo.ID, pos.x, pos.y, vel.x, vel.y);

			break;
		}
		}
	}
}
