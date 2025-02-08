#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Core/Log.h"

#include <iostream>

class ServerLayerCustom : public Walnut::Layer
{
	public:
	virtual void OnAttach() override
	{
	}

	virtual void OnUpdate(float ts) override
	{
		WL_INFO("server timestamp: {0}", ts);
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pixel Chase Server";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ServerLayerCustom>();
	return app;
}
