#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "ClientLayer.h"

Walnut::Application *Walnut::CreateApplication(int argc, char **argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pixel Chase";
	spec.CustomTitlebar = false;
	spec.UseDockspace = false;

	Walnut::Application *app = new Walnut::Application(spec);
	app->PushLayer<PixelChase::ClientLayer>();
	return app;
}
