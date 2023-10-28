#include <iostream>
#include <SegundoDoc.h>

using namespace std;

b2PolygonShape makeHorizontalGroundBox(float width)
{
	b2PolygonShape groundBoxDefH;
	groundBoxDefH.SetAsBox(width, 1.0f);
	return groundBoxDefH;
}

b2PolygonShape makeVerticalGroundBox(float height)
{
	b2PolygonShape groundBoxDefV;
    groundBoxDefV.SetAsBox(1.0f, height);
	return groundBoxDefV;
}

b2World* makeWorld()
{
	b2Vec2 gravity(0.0f, 100.0f);
	return new b2World(gravity);
}

CSegundoDoc::CSegundoDoc(float aspectRatio)
{
	m_pWorld = makeWorld();

	float height = 200.0f;
	float width = height * aspectRatio;

	b2PolygonShape groundBoxDefH = makeHorizontalGroundBox(width);
	b2PolygonShape groundBoxDefV = makeVerticalGroundBox(height);

	b2BodyDef groundBodyDef[4];

	float posV = height / 2.0f - 1.0f;
	float posH = width / 2.0f - 1.0f;
    groundBodyDef[0].position.Set(  1.0f,-posV);
    groundBodyDef[1].position.Set(  1.0f, posV);
    groundBodyDef[2].position.Set(-posH,  1.0f);
    groundBodyDef[3].position.Set( posH,  1.0f);

	m_world_top_left = b2Vec2(-posH, -posV);
	m_world_size = b2Vec2(width, height);


	b2Body* bodies[4];
	for(int i = 0; i < 4; i++)
		bodies[i] = m_pWorld->CreateBody(&groundBodyDef[i]);

	bodies[0]->CreateFixture(&groundBoxDefH, 0.0f);
	bodies[1]->CreateFixture(&groundBoxDefH, 0.0f);
	bodies[2]->CreateFixture(&groundBoxDefV, 0.0f);
	bodies[3]->CreateFixture(&groundBoxDefV, 0.0f);
}

CSegundoDoc::~CSegundoDoc()
{
	delete m_pWorld;
}