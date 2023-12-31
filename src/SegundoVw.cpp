// SegundoVw.cpp : implementation of the CSegundoVw class
//
#include "SegundoDoc.h"
#include "SegundoVw.h"
#include <toolbox.h>
#include <vector>
#include <iostream>
using namespace std;
#define CRect sf::IntRect
#define CPoint sf::Vector2i

// using namespace FileSystem;
// #include <cronometro.h>

// CSegundoVw
// BEGIN_MESSAGE_MAP(CSegundoVw, CView)
// 	ON_COMMAND(ID_SIMULA_ATIVADA, OnSimulaAtivada)
// 	ON_UPDATE_COMMAND_UI(ID_SIMULA_ATIVADA, OnUpdateSimulaAtivada)
// 	ON_WM_TIMER()
// 	ON_WM_LBUTTONUP()
// 	ON_WM_SIZE()
// 	ON_WM_ERASEBKGND()
// 	ON_COMMAND(ID_OBJETOS_BOX, OnObjetosBox)
// 	ON_COMMAND(ID_OBJETOS_CIRCULO, OnObjetosCirculo)
// 	ON_WM_LBUTTONDOWN()
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_BOX, OnUpdateObjetosBox)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_CIRCULO, OnUpdateObjetosCirculo)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_POINTER, OnUpdateObjetosPointer)
// 	ON_COMMAND(ID_OBJETOS_POINTER, OnObjetosPointer)
// 	ON_COMMAND(ID_OBJETOS_JUN, OnObjetosJun)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_JUN, OnUpdateObjetosJun)
// 	ON_WM_MOUSEMOVE()
// 	ON_WM_SETCURSOR()
// 	ON_WM_RBUTTONDOWN()
// 	ON_WM_RBUTTONUP()
// 	ON_COMMAND(ID_OBJETOS_BOX_VAR, OnObjetosBoxVar)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_BOX_VAR, OnUpdateObjetosBoxVar)
// 	ON_COMMAND(ID_OBJETOS_POLYLINE, OnObjetosPolyline)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_POLYLINE, OnUpdateObjetosPolyline)
// 	ON_WM_KEYDOWN()
// 	ON_COMMAND(ID_OBJETOS_HEX, OnObjetosHex)
// 	ON_UPDATE_COMMAND_UI(ID_OBJETOS_HEX, OnUpdateObjetosHex)
// 	ON_WM_MOUSEWHEEL()
// END_MESSAGE_MAP()


class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};


// CSegundoVw construction/destruction
CSegundoVw::CSegundoVw(CSegundoDoc *pDoc, sf::RenderWindow& window) :
	  m_bRunning(true)
	, m_toolbox()
	, m_pGrabbed(nullptr)
	, m_pTempJoint(nullptr)
	, m_nCurPoly(0)
    , m_pDoc(pDoc)
    , m_Window(window)
	, m_bShowDemo(false)
	, m_bShowToolbox(true)
	, m_bShowParamsBox(false)
{
	sf::Vector2f top_left = WorldToLogical(m_pDoc->m_world_top_left);
	sf::Vector2f size = WorldToLogical(m_pDoc->m_world_size);
	sf::FloatRect rc = sf::FloatRect(top_left, size);
	m_Window.setView(sf::View(rc));
}

CSegundoVw::~CSegundoVw()
{
}

// CSegundoVw drawing
void CSegundoVw::OnDraw(sf::RenderWindow& window)
{
	b2World *pWorld = m_pDoc->m_pWorld;
	pWorld->SetGravity(b2Vec2(m_paramsBox.m_gravity[0], m_paramsBox.m_gravity[1]));
	pWorld->Step(1.0f/60.0f, 6, 2);

	{
		ImGuiWindowFlags window_flags = 0
		                                // | ImGuiWindowFlags_NoBackground 
										// | ImGuiWindowFlags_NoTitleBar 
										| ImGuiWindowFlags_NoResize 
										| ImGuiWindowFlags_NoMove
										| ImGuiWindowFlags_AlwaysAutoResize
										;
		ImGui::Begin("Segundo", NULL, window_flags);

		ImGui::Checkbox("Show toolbox", &m_bShowToolbox);
		ImGui::SameLine();
		ImGui::Checkbox("Show params box", &m_bShowParamsBox);
		// ImGui::SameLine();
		ImGui::Checkbox("Show demo window", &m_bShowDemo);

		if(m_bShowToolbox) m_toolbox.draw();
		if(m_bShowParamsBox) m_paramsBox.draw();
		if(m_bShowDemo) ImGui::ShowDemoWindow();

		// Render
		Draw(window);
		
		int32 nQtdBodies = pWorld->GetBodyCount();

		ImGui::Begin("Status");
		ImGui::Text("Número de corpos: %d", nQtdBodies);
		ImGui::End();	

		ImGui::End();
	}
}

void CSegundoVw::DrawTempJoint(sf::RenderWindow& window)
{
	if(m_pTempJoint == NULL) return;

	sf::Vector2f p = WorldToLogical(m_pTempJoint->GetAnchorA());
	sf::Vector2f p2 = WorldToLogical(m_pTempJoint->GetAnchorB());
	float size = 2.0f;
	sf::Vertex line[] =
	{
		sf::Vertex(p - sf::Vector2f(-size, -size)),
		sf::Vertex(p - sf::Vector2f( size,  size)),
		sf::Vertex(p - sf::Vector2f(-size,  size)),
		sf::Vertex(p - sf::Vector2f( size, -size)),
	};
	window.draw(line, 4, sf::Lines);
}

float CSegundoVw::CComputeFillColor::GetImpulseSum(b2Body* pBody)
{
	float fImpulse = 0;
	for(b2ContactEdge* cn = pBody->GetContactList(); cn; cn = cn->next)
	{
		b2Manifold *manifold = cn->contact->GetManifold();
		for(int i = 0; i < manifold->pointCount; i++)
		{
			fImpulse += manifold->points[i].normalImpulse;
		}
	}
	return fImpulse;
}

sf::Color CSegundoVw::CComputeFillColor::GetFill(b2Fixture* pFixture)
{
	sf::Color crFill;
	b2Body* b = pFixture->GetBody();
	float fMomentum = b->GetLinearVelocity().Length() * b->GetMass();
	if(!b->IsAwake() && b->GetType() != b2_staticBody)
	{
		crFill = sf::Color::Red;
	}
	else if(b == m_pGrabbed)
	{
		crFill = sf::Color(255,255,128);
	}
	else if(b->GetType() == b2_staticBody)
	{
		crFill = sf::Color(192,192,192);
	}
	else
	{
		if(m_bShowMomentum)
		{
			float color = fMomentum * 255.0f / m_fMaxMomentum;
			crFill = sf::Color(color, color, color);
		}
		else if(m_bShowImpulses)
		{
			float fImpulse = GetImpulseSum(b);
			float color = fImpulse * 255.0f / m_fMaxImpulse;
			crFill = sf::Color(color, color, color);
		}
		else
		{
			crFill = sf::Color(200,200,255);
		}
	}
	return crFill;
}

sf::Color CSegundoVw::CComputeFillColor::GetStroke(b2Fixture* pFixture)
{
	b2Body* b = pFixture->GetBody();

	if(b == m_pGrabbed)
	{
		return sf::Color::Green;
	}
	else
	{
		return sf::Color::White;
	}
}


void CSegundoVw::Draw(sf::RenderWindow& window)
{
	// CCronometro cr;
	// cr.Start();

	vector<b2Body*> vecDel;
	b2World *pWorld = m_pDoc->m_pWorld;
	sf::Color crFill,crStroke;
	float fMaxMomentum = 0;
	float fMomentum = 0;
	float fImpulse = 0;
	float fMaxImpulse = 0;

	if(m_paramsBox.m_bShowImpulses)
	{
		for(b2Contact* cn = pWorld->GetContactList() ; cn ; cn = cn->GetNext())
		{
			b2Manifold *manifold = cn->GetManifold();
			for(int i = 0; i < manifold->pointCount; i++)
			{
				fImpulse = manifold->points[i].normalImpulse;
				fMaxImpulse = max(fMaxImpulse, fImpulse);
			}
		}
	}

	if(m_paramsBox.m_bShowMomentum)
	{
		for (b2Body* b = pWorld->GetBodyList(); b; b = b->GetNext())
		{
			fMomentum = b->GetLinearVelocity().Length() * b->GetMass();
			fMaxMomentum = max(fMaxMomentum, fMomentum);
		}
	}

	for (b2Body* b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		const b2Transform& xf = b->GetTransform();
		b2Vec2 position = b->GetPosition();
		
		CComputeFillColor computeFillColor(fMaxImpulse, 
										   fMaxMomentum, 
										   m_pGrabbed, 
										   m_paramsBox.m_bShowMomentum, 
										   m_paramsBox.m_bShowImpulses);

		for (b2Fixture* s = b->GetFixtureList(); s; s = s->GetNext())
		{
			// Compute fill color:
			crFill = computeFillColor.GetFill(s);
			crStroke = computeFillColor.GetStroke(s);
			DrawShape(window, s, xf, position, crFill, crStroke);
		}
	}

	DrawTempJoint(window);

#if 0
	b2Vec2 jp;
	for(b2Joint* j = pWorld->m_jointList; j; j = j->m_next)
	{
		jp = j->GetAnchor1();
		pDc->Ellipse(jp.x-1,jp.y-1,jp.x+1,jp.y+1);
		pDc->MoveTo(jp.x-1,jp.y);
		pDc->LineTo(jp.x+1,jp.y);
	}

	if(m_nCurPoly != 0)
	{
		pDc->BeginPath();
		CPoint pt;
		for(int i = 0; i < m_nCurPoly; i++)
		{
			pt = m_ptPoly[i];
			pDc->DPtoLP(&pt);
			if(i == 0)
				pDc->MoveTo(pt);
			else
				pDc->LineTo(pt);
		}
		pDc->CloseFigure();
		pDc->EndPath();
		pDc->StrokeAndFillPath();
	}
	pDoc->m_Wa.ReleaseWorld();
	CMainFrame *pFrm = (CMainFrame *)GetParentFrame();
	pFrm->m_barObjPrm.UpdateData();
	pFrm->m_barObjPrm.m_dwDraw = 1.0/cr.Get(CCronometro::uS);
	pFrm->m_barObjPrm.UpdateData(FALSE);
    #endif
}

void CSegundoVw::DrawShape(sf::RenderWindow& window, 
						   const b2Fixture* fixture, 
						   const b2Transform& xf, 
						   b2Vec2 position, 
						   sf::Color crFill, 
						   sf::Color crCont)
{
	const b2Shape* shape = fixture->GetShape();
	b2Shape::Type shapeType = shape->GetType();
	switch (shapeType)
	{
	case b2Shape::Type::e_circle:
	{
		const b2CircleShape* circle = (const b2CircleShape*)shape;
		sf::Vector2f center = WorldToLogical(b2Mul(xf, circle->m_p));
		float r = WorldToLogical(b2Vec2(circle->m_radius, 0)).x;
		sf::CircleShape circle_shape(r);
		circle_shape.setPosition(center - WorldToLogical(b2Vec2(r, r)));
		circle_shape.setFillColor(crFill);
		circle_shape.setOutlineColor(crCont);
		circle_shape.setOutlineThickness(0.2f);

		window.draw(circle_shape);

		sf::Vector2f circle_pos = WorldToLogical(b2Mul(xf, circle->m_p + (circle->m_radius * b2Vec2(1.0f, 0))));
		sf::Vertex line[] =
		{
			sf::Vertex(circle_pos, crCont),
			sf::Vertex(center, crCont)
		};
		window.draw(line, 2, sf::Lines);
	}
		break;

	case b2Shape::Type::e_polygon:
		{
			const b2PolygonShape* poly = (const b2PolygonShape*)shape;
            size_t vertexCount = poly->m_count;
            sf::ConvexShape convex(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i)
            {
                b2Vec2 v = b2Mul(xf, poly->m_vertices[i]);
                convex.setPoint(i, WorldToLogical(v));
            }
			convex.setFillColor(crFill);
			convex.setOutlineColor(crCont);
			convex.setOutlineThickness(0.2f);

			window.draw(convex);
		}
		break;
	}
}

void CSegundoVw::OnLButtonUp(uint64_t nFlags, sf::Vector2i point)
{
	if(m_toolbox.getCurrentTool() == ToolBox::TTool::toolPointer)
	{
		if(m_pGrabbed)
			m_pGrabbed = NULL;

		if(m_pTempJoint)
		{
			m_pDoc->m_pWorld->DestroyJoint(m_pTempJoint);
			m_pTempJoint = NULL;
		}
        ImGui::CaptureMouseFromApp(false);
	}
    // TODO: Completar aqui
	// if(m_toolbox.getCurrentTool() == ToolBox::TTool::toolAddPoly)
	// {
	// 	OnAddPoly(point);
	// }
}

void CSegundoVw::OnLButtonDown(uint64_t nFlags, sf::Vector2i point)
{
	switch(m_toolbox.getCurrentTool())
	{
	case ToolBox::TTool::toolPointer:
		OnPointer(point);
		break;
	}
}

void CSegundoVw::OnLButtonClicked(uint64_t nFlags, sf::Vector2i point)
{
	if(ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		return;
	}

	switch(m_toolbox.getCurrentTool())
	{
	case ToolBox::TTool::toolAddBox:
		AddBox(point);
		break;
	case ToolBox::TTool::toolAddCircle:
		AddCircle(point);
		break;
	case ToolBox::TTool::toolAddHexagon:
		AddHexagon(point);
		break;
    // TODO: Completar aqui
	// case ToolBox::TTool::toolJoint:
	// 	AddJoint(point,point);
	// 	break;
	// case ToolBox::TTool::toolVarBox:
	// 	AddVarBox(point);
	// 	break;
	}
}

bool CSegundoVw::IsPointer()
{
	return m_toolbox.getCurrentTool() == ToolBox::TTool::toolPointer;
}

bool CSegundoVw::IsGrabbed()
{
	return m_pGrabbed != NULL;
}

void CSegundoVw::OnMouseMove(bool bShift, sf::Vector2i point)
{
	if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && IsPointer() && IsGrabbed())
	{
		m_pTempJoint->SetTarget(DeviceToWorld(point));
	}

	if(bShift  && m_paramsBox.m_density != 0.0f)
	{
		switch(m_toolbox.getCurrentTool())
		{
		case ToolBox::TTool::toolAddBox:
			AddBox(point);
			break;
		case ToolBox::TTool::toolAddHexagon:
			AddHexagon(point);
			break;
		case ToolBox::TTool::toolAddCircle:
			AddCircle(point);
			break;
		// case toolAddPoly:
		// 	OnAddPoly(point);
		// 	break;
		}	
	}
	else if(IsPointer() && ImGui::IsKeyDown(ImGuiKey_Delete))
	{
		RemoveBody(point);
	}
}

void CSegundoVw::OnRButtonDown(uint64_t nFlags, sf::Vector2i point)
{
	// TODO: Completar aqui
	// if(m_toolbox.getCurrentTool() == ToolBox::TTool::toolAddPoly && m_nCurPoly > 0)
	// {
	// 	FinalizePoly();
	// }
}

b2Vec2 CSegundoVw::LogicalToWorld(sf::Vector2f devicePoint)
{
	return b2Vec2((float)devicePoint.x,(float)devicePoint.y);
}

sf::Vector2f CSegundoVw::DeviceToLogical(sf::Vector2i devicePoint)
{
    return m_Window.mapPixelToCoords(devicePoint);
}

b2Vec2 CSegundoVw::DeviceToWorld(sf::Vector2i devicePoint)
{
    return LogicalToWorld(DeviceToLogical(devicePoint));
}

sf::Vector2f CSegundoVw::WorldToLogical(b2Vec2 worldPoint)
{
	return sf::Vector2f(worldPoint.x, worldPoint.y);
}

sf::Vector2i CSegundoVw::LogicalToDevice(sf::Vector2f logicalPoint)
{
	return m_Window.mapCoordsToPixel(logicalPoint);
}

sf::Vector2i CSegundoVw::WorldToDevice(b2Vec2 worldPoint)
{
	return LogicalToDevice(WorldToLogical(worldPoint));
}

void CSegundoVw::AddBox(sf::Vector2i ptWhere)
{
	b2PolygonShape boxDef;
	boxDef.SetAsBox(m_paramsBox.m_size[0], m_paramsBox.m_size[1]);
	boxDef.m_centroid.Set(0.0f, 0.0f);
	boxDef.m_radius = 0.5f;

	b2BodyDef bodyDef;
    bodyDef.position = DeviceToWorld(ptWhere);
	bodyDef.angularVelocity = m_paramsBox.m_angular_velocity;
	bodyDef.linearVelocity.Set( m_paramsBox.m_linear_velocity[0],
								m_paramsBox.m_linear_velocity[1]);
	bodyDef.type = b2_dynamicBody;

	b2Body* body = m_pDoc->m_pWorld->CreateBody(&bodyDef);
	b2FixtureDef fixdef;
	fixdef.shape = &boxDef;
	fixdef.density = m_paramsBox.m_density;
    fixdef.friction = m_paramsBox.m_friction;
    fixdef.restitution = m_paramsBox.m_restitution;
    body->CreateFixture(&fixdef);
}

void CSegundoVw::AddCircle(sf::Vector2i ptWhere)
{
	b2CircleShape circleShape;
	circleShape.m_radius = m_paramsBox.m_size[0];
	circleShape.m_p.Set(0.0f, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.position = DeviceToWorld(ptWhere);
	bodyDef.angularVelocity = m_paramsBox.m_angular_velocity;
	bodyDef.linearVelocity.Set(m_paramsBox.m_linear_velocity[0],
							   m_paramsBox.m_linear_velocity[1]);

	b2FixtureDef fixdef;
	fixdef.shape = &circleShape;
	fixdef.density = m_paramsBox.m_density;
	fixdef.friction = m_paramsBox.m_friction;
	fixdef.restitution = m_paramsBox.m_restitution;

	b2Body* body = m_pDoc->m_pWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixdef);
	body->SetType(b2_dynamicBody);
}

#define RAD(a) (a*M_PI/180.0)
void CSegundoVw::AddHexagon(sf::Vector2i pt)
{
	b2PolygonShape hexagonShape;
	b2Vec2 pCenter = DeviceToWorld(pt);
	b2Vec2 vertices[6];
	for(int i= 0; i < 6; i++)
	{
		vertices[i].Set(cos(RAD(60*i)) * m_paramsBox.m_size[0],
						sin(RAD(60*i)) * m_paramsBox.m_size[0]);
	}
	hexagonShape.Set(vertices, 6);
	hexagonShape.m_radius = 0.5f;

	b2FixtureDef fixtureDef;
	fixtureDef.density = m_paramsBox.m_density;
	fixtureDef.friction = m_paramsBox.m_friction;
	fixtureDef.restitution = m_paramsBox.m_restitution;
	fixtureDef.shape = &hexagonShape;
	
	b2BodyDef bodyDef;
	bodyDef.position = pCenter;
	bodyDef.angularVelocity = m_paramsBox.m_angular_velocity;
	bodyDef.linearVelocity.Set(m_paramsBox.m_linear_velocity[0],
							   m_paramsBox.m_linear_velocity[1]);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = m_pDoc->m_pWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

}

b2Fixture* CSegundoVw::QueryFixture(b2Vec2 point)
{
	b2Vec2 d(0.001f, 0.001f);
	b2AABB aabb;
	QueryCallback cb(point);

	aabb.upperBound = point + d;
	aabb.lowerBound = point - d;
	m_pDoc->m_pWorld->QueryAABB(&cb, aabb);

	return cb.m_fixture;
}

void CSegundoVw::OnPointer(sf::Vector2i ptWhere)
{
	b2Vec2 p = DeviceToWorld(ptWhere);
	b2Fixture* fixture = QueryFixture(p);
	if(!fixture) return;

	float frequencyHz = 5.0f;
	float dampingRatio = 0.7f;

	b2Body* body = fixture->GetBody();
	b2MouseJointDef jd;
	jd.bodyA = m_pDoc->m_pGroundBody;
	jd.bodyB = body;
	jd.target = p;
	jd.maxForce = 10000.0f * body->GetMass();
	b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

	m_pTempJoint = (b2MouseJoint*)m_pDoc->m_pWorld->CreateJoint(&jd);
	body->SetAwake(true);
	m_pGrabbed = body;

	ImGui::CaptureKeyboardFromApp(true);

	m_paramsBox.m_linear_velocity[0] = body->GetLinearVelocity().x;
	m_paramsBox.m_linear_velocity[1] = body->GetLinearVelocity().y;
	m_paramsBox.m_angular_velocity = body->GetAngularVelocity();
	m_paramsBox.m_density = fixture->GetDensity();
	m_paramsBox.m_friction = fixture->GetFriction();
	m_paramsBox.m_restitution = fixture->GetRestitution();
}

void CSegundoVw::RemoveBody(sf::Vector2i point)
{
	if(m_pGrabbed != nullptr)
	{
		if(m_pTempJoint != nullptr)
		{
			m_pDoc->m_pWorld->DestroyJoint(m_pTempJoint);
			m_pTempJoint = nullptr;
		}
		m_pDoc->m_pWorld->DestroyBody(m_pGrabbed);
		m_pGrabbed = nullptr;
	}
	else
	{
		b2Vec2 p = DeviceToWorld(point);
		b2Fixture* fixture = QueryFixture(p);
		if(fixture)
		{
			b2Body* body = fixture->GetBody();
			m_pDoc->m_pWorld->DestroyBody(body);
		}
	}
}

#if 0
void CSegundoVw::AddJoint(CPoint apt1, CPoint apt2)
{
	CPrimeiroDoc *pDoc = GetDocument();
	b2Vec2 p1,p2;
	p1 = MakeLP(apt1);
	p2 = MakeLP(apt2);
	
	b2Body *b1,*b2;

	b2AABB aabb;
	aabb.minVertex.Set(p1.x-.1f,p1.y-.1f);
	aabb.maxVertex.Set(p1.x+.1f,p1.y+.1f);
	b2Shape *shape1;
	b2World *pWorld = pDoc->m_Wa.GetWorld();
	int32 count = pWorld->Query(aabb,&shape1,1);
	if(count != 1)
	{
		pDoc->m_Wa.ReleaseWorld();
		return;
	}

	b1 = shape1->GetBody();

	aabb.minVertex.Set(p2.x-0.1f,p2.y-0.1f);
	aabb.maxVertex.Set(p2.x+0.1f,p2.y+0.1f);
	b2Shape *shape2[2];
	count = pWorld->Query(aabb,shape2,2);
	if(count == 0 || (count == 1 && shape2[1] != shape1) )
	{
		b2 = pWorld->GetGroundBody();
	}
	else if(count == 2 && shape2[1] != shape1)
	{
		b2 = shape2[1]->GetBody();
	}
	else
	{
		pDoc->m_Wa.ReleaseWorld();
		return;
	}

	b2RevoluteJointDef jointDef;
    jointDef.body1 = b1;
    jointDef.body2 = b2;
	jointDef.anchorPoint = p1;

	pWorld->CreateJoint(&jointDef);
	pDoc->m_Wa.ReleaseWorld();
}

#pragma warning( disable : 4244 )
void CSegundoVw::AddVarBox(CPoint pt)
{
	CRectTracker tr(CRect(pt,pt+CPoint(1,1)),CRectTracker::dottedLine);
	if(tr.TrackRubberBand(this,pt))
	{
		CRect rc(tr.m_rect);
		rc.NormalizeRect();
		CPaintDC dc(this);
		SetDc(&dc);
		dc.DPtoLP(rc);
		CPoint ptc = rc.CenterPoint();

		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->m_barObjPrm.UpdateData();

		b2BoxDef boxDef;
		boxDef.extents.Set(rc.Width()/2.0,rc.Height()/2.0);
		boxDef.density = pFrm->m_barObjPrm.m_fDensidade;
		boxDef.friction = pFrm->m_barObjPrm.m_fAtrito;
		boxDef.restitution = pFrm->m_barObjPrm.m_fElasticidade;

		b2BodyDef bodyDef;
		bodyDef.position.Set(ptc.x,ptc.y);
		bodyDef.angularVelocity = pFrm->m_barObjPrm.m_fAngular;
		bodyDef.linearVelocity.Set( pFrm->m_barObjPrm.m_fLinear[0],
									pFrm->m_barObjPrm.m_fLinear[1]);
		bodyDef.AddShape(&boxDef);

		GetDocument()->m_Wa.GetWorld()->CreateBody(&bodyDef);
		GetDocument()->m_Wa.ReleaseWorld();

		if(!m_bRunning)
			Invalidate();
	}
}

void CSegundoVw::FinalizePoly(void)
{
	if(m_nCurPoly < 3)
		return;


	// Ordenamento dos v�rtices:
	{
		// Lista de v�rtices:
//		map<float,b2Vec2> vecVertices;

	}
	// End Of Ordenamento dos v�rtices


	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->m_barObjPrm.UpdateData();



	b2PolyDef polydef;
	polydef.density = pFrm->m_barObjPrm.m_fDensidade;
	polydef.friction = pFrm->m_barObjPrm.m_fAtrito;
	polydef.restitution = pFrm->m_barObjPrm.m_fElasticidade;
	polydef.vertexCount = m_nCurPoly;
	b2Vec2 pCenter(0,0);
	for(int i = 0; i < m_nCurPoly; i++)
	{
		polydef.vertices[i] = MakeLP(m_ptPoly[i]);
		pCenter += polydef.vertices[i];
	}
	pCenter.x /= m_nCurPoly;
	pCenter.y /= m_nCurPoly;
	for(int i = 0; i < m_nCurPoly; i++)
	{
		polydef.vertices[i].Set(m_ptPoly[i].x-pCenter.x,m_ptPoly[i].y-pCenter.y);
	}
	
	b2BodyDef bodyDef;
	bodyDef.position = pCenter;
	bodyDef.angularVelocity = pFrm->m_barObjPrm.m_fAngular;
	bodyDef.linearVelocity.Set( pFrm->m_barObjPrm.m_fLinear[0],
								pFrm->m_barObjPrm.m_fLinear[1]);
	bodyDef.AddShape(&polydef);

	GetDocument()->m_Wa.GetWorld()->CreateBody(&bodyDef);
	GetDocument()->m_Wa.ReleaseWorld();

	if(!m_bRunning)
		Invalidate();

	m_nCurPoly = 0;
}

void CSegundoVw::ProcessSounds(void)
{
}

void CSegundoVw::OnAddPoly(CPoint point)
{
	m_ptPoly[m_nCurPoly++] = point;
	if(m_nCurPoly == MAX_POLY_VERTEXES)
	{
		FinalizePoly();
	}
	if(!m_bRunning)
		Invalidate();
}

BOOL CSegundoVw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_pGrabbed != NULL)
	{
		m_pGrabbed->m_rotation -= (zDelta/1000.0);
		m_pGrabbed->SetAngularVelocity(0);
		Invalidate();
	}
		
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
#endif