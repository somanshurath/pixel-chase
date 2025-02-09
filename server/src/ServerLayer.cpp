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
		s_ScratchBuffer.Allocate(1024 * 1024 * 30); // 10MB

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
		Walnut::BufferStreamWriter stream(s_ScratchBuffer);
		m_PlayerDataMutex.lock();
		{
			stream.WriteRaw(PacketType::ClientUpdate);
			stream.WriteMap(m_PlayerData);
		}
		m_PlayerDataMutex.unlock();

		m_Server.SendBufferToAllClients(stream.GetBuffer());
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
			m_PlayerDataMutex.lock();
			{
				PlayerData &playerData = m_PlayerData[clientInfo.ID];
				stream.ReadRaw<glm::vec2>(playerData.Position);
				stream.ReadRaw<glm::vec2>(playerData.Velocity);
				WL_INFO_TAG("Server", "ID: {}, Pos: ({}, {}), Vel: ({}, {})", clientInfo.ID, playerData.Position.x, playerData.Position.y, playerData.Velocity.x, playerData.Velocity.y);
			}
			m_PlayerDataMutex.unlock();

			break;
		}
		}
	}
}
