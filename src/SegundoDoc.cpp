#include <iostream>
#include <SegundoDoc.h>

using namespace std;


CSegundoDoc::CSegundoDoc()
{
    // m_pWorld = new b2World(b2Vec2(0.0f, 0.0f));
    m_crBack = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	b2AABB worldAABB;
    worldAABB.lowerBound.Set(-100.0f, -100.0f);
    worldAABB.upperBound.Set(100.0f, 100.0f);
	b2Vec2 gravity(0.0f, 100.0f);
    bool doSleep = true;

	m_pWorld = new b2World(worldAABB, gravity, doSleep);
	cout << "World created at " << m_pWorld << endl;

	b2PolygonDef groundBoxDefH, groundBoxDefV;
	b2BodyDef groundBodyDef[4];
	b2Body* bodies[4];

    groundBodyDef[0].position = b2Vec2( 50.0f,  -99.0f);
    groundBodyDef[1].position.Set( 0.0f, 98.0f);
    groundBodyDef[2].position.Set( 0.0f,  0.0f);
    groundBodyDef[3].position.Set(98.0f,  0.0f);

	for(int i = 0; i < 4; i++)
		bodies[i] = m_pWorld->CreateBody(&groundBodyDef[i]);


	groundBoxDefH.SetAsBox(100.0f, 1.0f);
    groundBoxDefV.SetAsBox(1.0f, 100.0f);
	groundBoxDefH.density = groundBoxDefV.density = 0.0f;

	bodies[0]->CreateShape(&groundBoxDefH);
	bodies[1]->CreateShape(&groundBoxDefH);
	bodies[2]->CreateShape(&groundBoxDefV);
	bodies[3]->CreateShape(&groundBoxDefV);
}

CSegundoDoc::~CSegundoDoc()
{
	delete m_pWorld;
}