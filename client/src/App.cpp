#include "ClientLayer.h"

#include "Walnut/EntryPoint.h"

Walnut::Application *Walnut::CreateApplication(int argc, char **argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pixel Chase";
	spec.CustomTitlebar = false;
	spec.UseDockspace = false;
	spec.Height = 640;
	spec.Width = 1280;

	Walnut::Application *app = new Walnut::Application(spec);
	app->PushLayer<PixelChase::ClientLayer>();
	return app;
}
