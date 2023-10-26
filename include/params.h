#pragma once

class ParamsBox
{
public:
    float m_linear_velocity[2];
    float m_angular_velocity;
    float m_density;
    float m_friction;
    float m_restitution;
public:
    ParamsBox();
    void draw();
};
