#include "ClientLayer.h"

namespace PixelChase
{
    static void DrawRect(float x, float y, float width, float height, int color, ImVec2 &windowPos)
    {
        ImVec2 pos = ImVec2(windowPos.x + x, windowPos.y + y);
        ImVec2 size = ImVec2(width, height);
        ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + size, color);
    }

    void ClientLayer::OnAttach()
    {
    }

    void ClientLayer::OnDetach()
    {
    }

    void ClientLayer::OnUpdate(float ts)
    {
    }

    void ClientLayer::OnUIRender()
    {

        ImGui::ShowDemoWindow();

        // ImGui::Begin("Pixel Chase");

        // ImVec2 windowPos = ImGui::GetWindowPos();

        // DrawRect(0, 0, 100, 100, IM_COL32(255, 0, 0, 255), windowPos);
        // DrawRect(100, 0, 100, 100, IM_COL32(0, 255, 0, 255), windowPos);
        // DrawRect(200, 0, 100, 100, IM_COL32(0, 0, 255, 255), windowPos);

        // ImGui::End();
    }
}
