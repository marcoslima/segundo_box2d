#include <params.h>
#include <imgui.h>


ParamsBox::ParamsBox() :
    m_size{5.0f, 5.0f},
    m_linear_velocity{0.0f, 0.0f},
    m_angular_velocity(0.0f),
    m_density(1.0f),
    m_friction(0.3f),
    m_restitution(0.5f),
    m_gravity{0.0f, 10.0f},
    m_bShowMomentum(false),
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
                                    // | ImGuiWindowFlags_NoBackground
                                    ;
    ImGui::BeginChild("Params", ImVec2(340, 250), true, window_flags);
    m_bHovered = ImGui::IsWindowHovered();

    ImGui::Text("Object Parameters");
    ImGui::SliderFloat2("Size", m_size, 0.0f, 100.0f);
    ImGui::SliderFloat2("Linear Velocity", m_linear_velocity, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Angular Velocity", &m_angular_velocity, -100.0f, 100.0f);
    ImGui::SliderFloat("Density", &m_density, 0.0f, 100.0f);
    ImGui::SliderFloat("Friction", &m_friction, 0.0f, 1.0f);
    ImGui::SliderFloat("Restitution", &m_restitution, 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::Text("World Parameters");
    ImGui::SliderFloat2("Gravity", m_gravity, -100.0f, 100.0f);
    ImGui::Checkbox("Show Momentum", &m_bShowMomentum);
    ImGui::EndChild();
}
