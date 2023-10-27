#pragma once
#include <box2d/box2d.h>
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
