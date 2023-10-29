#pragma once
#include <box2d/box2d.h>
#include <imgui.h>


class CSegundoDoc
{
public:
	CSegundoDoc(float aspectRatio = 1.0);
	~CSegundoDoc();

// Attributes
public:
	b2World *m_pWorld;
	ImVec4	m_crBack;
	b2Vec2 m_world_top_left;
	b2Vec2 m_world_size;
	b2Body *m_pGroundBody;

};
