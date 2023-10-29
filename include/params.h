#pragma once

class ParamsBox
{
private:
    bool m_bHovered;
public:
    float m_size[2];
    float m_linear_velocity[2];
    float m_angular_velocity;
    float m_density;
    float m_friction;
    float m_restitution;
    float m_gravity[2];
    bool m_bShowMomentum;
    bool m_bOpen;
public:
    ParamsBox();
    void draw();
    bool isHovered() { return m_bHovered; }
};
