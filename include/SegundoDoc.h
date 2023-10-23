#pragma once
#include <Box2D.h>
#include <imgui.h>


class CSegundoDoc
{
public:
	CSegundoDoc();
	~CSegundoDoc();

// Attributes
public:
	b2World *m_pWorld;
	ImVec4	m_crBack;
};
