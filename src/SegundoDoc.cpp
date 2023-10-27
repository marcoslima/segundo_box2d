#include <iostream>
#include <SegundoDoc.h>

using namespace std;

b2PolygonShape makeHorizontalGroundBox()
{
	b2PolygonShape groundBoxDefH;
	groundBoxDefH.SetAsBox(100.0f, 1.0f);
	return groundBoxDefH;
}

b2PolygonShape makeVerticalGroundBox()
{
	b2PolygonShape groundBoxDefV;
    groundBoxDefV.SetAsBox(1.0f, 100.0f);
	return groundBoxDefV;
}

b2World* makeWorld()
{
	b2Vec2 gravity(0.0f, 100.0f);
	return new b2World(gravity);
}

CSegundoDoc::CSegundoDoc()
{
	m_pWorld = makeWorld();
	b2PolygonShape groundBoxDefH = makeHorizontalGroundBox();
	b2PolygonShape groundBoxDefV = makeVerticalGroundBox();

	b2BodyDef groundBodyDef[4];

    groundBodyDef[0].position.Set( 50.0f,  -99.0f);
    groundBodyDef[1].position.Set( 0.0f, 98.0f);
    groundBodyDef[2].position.Set( 0.0f,  0.0f);
    groundBodyDef[3].position.Set(98.0f,  0.0f);


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