#pragma once

#include "Walnut/Layer.h"
#include "Walnut/Application.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace PixelChase
{
    class ClientLayer : public Walnut::Layer
    {
        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;
    };
}
