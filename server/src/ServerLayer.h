#pragma once

#include "Walnut/Layer.h"
#include "Walnut/Networking/Server.h"

#include "HeadlessConsole.h"

namespace PixelChase
{
	class ServerLayer : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;

	private:
		void OnConsoleMessage(std::string_view message);

	private:
		HeadlessConsole m_Console;
	};
}
