#pragma once

#include "Walnut/Layer.h"
#include "Walnut/Application.h"

#include "Walnut/Networking/Client.h"

#include <glm/glm.hpp>
#include <map>

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
        std::string m_ServerAddress = "127.0.0.1:8192";
        int m_ServerConnAttempts = 0;
        Walnut::Client m_Client;
        uint32_t m_ClientID = 0;
        uint32_t m_PlayerID = 0;

        struct PlayerData
        {
            glm::vec2 Position;
            glm::vec2 Velocity;
        };

        std::mutex m_PlayerDataMutex;
        std::map<uint32_t, PlayerData> m_PlayerData;
    };
}
