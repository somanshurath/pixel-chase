#pragma once

#include "Walnut/Layer.h"
#include "Walnut/Application.h"

#include "Walnut/Networking/Client.h"

#include <glm/glm.hpp>

namespace PixelChase
{
    class ClientLayer : public Walnut::Layer
    {
    public:
        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;

    private:
        void OnDataReceived(const Walnut::Buffer buffer);

    private:
        glm::vec2 m_PlayerPosition{0.0f, 0.0f};
        glm::vec2 m_PlayerVelocity{0.0f, 0.0f};
        std::string m_ServerAddress;
        int m_ServerConnAttempts = 0;
        Walnut::Client m_Client;
    };
}
