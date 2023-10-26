#include <params.h>
#include <imgui.h>


ParamsBox::ParamsBox() :
    m_linear_velocity{0.0f, 0.0f},
    m_angular_velocity(0.0f),
    m_density(1.0f),
    m_friction(0.3f),
    m_restitution(0.5f)
{
}

void ParamsBox::draw(void)
{
    ImGui::Begin("Params");

    ImGui::SliderFloat2("Linear Velocity", m_linear_velocity, -100.0f, 100.0f);
    ImGui::SliderFloat("Angular Velocity", &m_angular_velocity, -100.0f, 100.0f);
    ImGui::SliderFloat("Density", &m_density, 0.0f, 100.0f);
    ImGui::SliderFloat("Friction", &m_friction, 0.0f, 1.0f);
    ImGui::SliderFloat("Restitution", &m_restitution, 0.0f, 1.0f);

    ImGui::End();
}
