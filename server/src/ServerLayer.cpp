#include "ServerLayer.h"

#include "Walnut/Application.h"
#include "Walnut/Core/Log.h"

namespace PixelChase
{
	void ServerLayer::OnAttach()
	{
		m_Console.SetMessageSendCallback([this](std::string_view message)
										 { OnConsoleMessage(message); });
	}

	void ServerLayer::OnDetach()
	{
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
}
