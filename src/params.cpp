#include <params.h>
#include <imgui.h>


ParamsBox::ParamsBox() :
    m_linear_velocity{0.0f, 0.0f},
    m_angular_velocity(0.0f),
    m_density(1.0f),
    m_friction(0.3f),
    m_restitution(0.5f),
    m_bOpen(true)
{
}

void ParamsBox::draw(void)
{
    ImGuiWindowFlags window_flags =   ImGuiWindowFlags_NoResize
                                    | ImGuiWindowFlags_NoMove
                                    | ImGuiWindowFlags_NoCollapse
                                    | ImGuiWindowFlags_NoTitleBar
                                    | ImGuiWindowFlags_NoScrollbar
                                    | ImGuiWindowFlags_NoScrollWithMouse
                                    | ImGuiWindowFlags_AlwaysAutoResize
                                    | ImGuiWindowFlags_NoBackground
                                    ;
    // ImGui::SetWindowPos(ImVec2(0, 64));
    ImGui::Checkbox("Params", &m_bOpen);
    if(m_bOpen)
    {
        ImGui::BeginChild("Params", ImVec2(0, 0), false, window_flags);

        ImGui::SliderFloat2("Linear Velocity", m_linear_velocity, -100.0f, 100.0f);
        ImGui::SliderFloat("Angular Velocity", &m_angular_velocity, -100.0f, 100.0f);
        ImGui::SliderFloat("Density", &m_density, 0.0f, 100.0f);
        ImGui::SliderFloat("Friction", &m_friction, 0.0f, 1.0f);
        ImGui::SliderFloat("Restitution", &m_restitution, 0.0f, 1.0f);

        ImGui::EndChild();
    }
}
