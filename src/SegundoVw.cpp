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
		ImGui::SameLine();
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

void CSegundoVw::Draw(sf::RenderWindow& window)
{
	// CCronometro cr;
	// cr.Start();

	vector<b2Body*> vecDel;
	b2World *pWorld = m_pDoc->m_pWorld;
	sf::Color crFill,crStroke;
	float maxImpulse = 0;
	b2Contact *cn;
	float fImpulses = 0;
	vector<pair<CPoint,CPoint> > vecContacts;
	vector<CPoint> vecPontCont;

    // for(b2Contact* cn = pWorld->GetPairCount GetContactList(); cn; cn = cn->GetNext())
    // {
	// 	fImpulses = 0;
	// 	for(cn = b->GetContactList(); cn; cn = cn->next)
	// 	{
	// 		fImpulses += cn->contact->GetManifolds()->points->normalImpulse;
	// 	}
	// 	fImpulses /= b->GetMass();
	// 	maxImpulse = max(fImpulses,maxImpulse);
    // }

	for (b2Body* b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		const b2Transform& xf = b->GetTransform();
		b2Vec2 position = b->GetPosition();

		if(!b->IsAwake())
		{
			// vecDel.push_back(b);
		}
		for (b2Fixture* s = b->GetFixtureList(); s; s = s->GetNext())
		{
			// FILL:
			if(!b->IsAwake() && b->GetType() != b2_staticBody)
			{
				crFill = sf::Color::Red;
			}
			else if(b == m_pGrabbed)
			{
				crFill = sf::Color(255,255,128);
			}
			else if(!b->IsAwake())
			{
				crFill = sf::Color(s->GetRestitution() * 255, 0, 0);
			}
			else if(b->GetType() == b2_staticBody)
			{
				crFill = sf::Color(192,192,192);
			}
			else
			{
				crFill = sf::Color(fImpulses * 255.0f / maxImpulse,
							       fImpulses * 255.0f / maxImpulse,
							       fImpulses * 255.0f / maxImpulse);
			}

			// Stroke:
			if(b == m_pGrabbed)
			{
				crStroke = sf::Color::Green;
			}
			else
			{
				crStroke = sf::Color::White;
			}

			DrawShape(window, s, xf, position, crFill, crStroke);
		}
	}
	// m_maxImpulse = maxImpulse;


#if 0
	pDc->BeginPath();
	for(size_t i = 0; i < vecContacts.size(); i++)
	{
		pDc->MoveTo(vecContacts[i].first);
		pDc->LineTo(vecContacts[i].second);
	}
	for(size_t i = 0; i < vecPontCont.size(); i++)
	{
		pDc->Ellipse(vecPontCont[i].x-1,vecPontCont[i].y-1,
					 vecPontCont[i].x+1,vecPontCont[i].y+1);
	}

	pDc->EndPath();
	CPen penCont(PS_SOLID,1,RGB(255,0,0)), *pOldPen = pDc->SelectObject(&penCont);
	pDc->StrokePath();
	pDc->SelectObject(pOldPen);
#endif

	// Removemos todos os objetos congelados:
	for(size_t i = 0; i < vecDel.size(); i++)
	{
		if(vecDel[i] != m_pGrabbed )
			pWorld->DestroyBody(vecDel[i]);
	}

#if 0
	if(m_pTempJoint != NULL)
	{
		b2Vec2 p = m_pTempJoint->GetAnchor1();
		pDc->MoveTo(p.x-1,p.y-1);
		pDc->LineTo(p.x+1,p.y+1);
		pDc->MoveTo(p.x-1,p.y+1);
		pDc->LineTo(p.x+1,p.y-1);
	}

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

// CSegundoVw message handlers
void CSegundoVw::OnSimulaAtivada()
{
	m_bRunning = ! m_bRunning;
}

#if 0
void CSegundoVw::OnUpdateSimulaAtivada(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRunning);
}

void CSegundoVw::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == IDT_SIMULACAO)
	{
		KillTimer(IDT_SIMULACAO);
		if(m_Tool == toolPointer && m_pGrabbed != NULL)
		{
			OnPointerStep();
		}
		OnSimulacao();
		SetTimer(IDT_SIMULACAO,25,NULL);
	}

	CView::OnTimer(nIDEvent);
}
#endif

void CSegundoVw::DrawShape(sf::RenderWindow& window, const b2Fixture* fixture, const b2Transform& xf, b2Vec2 position, sf::Color crFill, sf::Color crCont)
{
	const b2Shape* shape = fixture->GetShape();
	b2Shape::Type shapeType = shape->GetType();
	switch (shapeType)
	{
	case b2Shape::Type::e_circle:
		if(false)
		{
			// const b2CircleShape* circle = (const b2CircleShape*)shape;
			// b2Vec2 x = circle->m_position;
			// float32 r = circle->m_radius;
			// pDc->Ellipse(x.x-r,x.y-r,x.x+r,x.y+r);
			// pDc->MoveTo(x.x,x.y);
			// b2Vec2 ax = circle->m_R.col1;
			// pDc->LineTo(x.x + r * ax.x, x.y + r * ax.y);
		}
		if(true)
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

			// b2Vec2 ax = circle->m_R.col1;
			// pDc->MoveTo(x.x,x.y);
			// pDc->LineTo(x.x + r * ax.x, x.y + r * ax.y);
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
    // TODO: Completar aqui
	case ToolBox::TTool::toolAddBox:
		AddBox(point);
		break;
	case ToolBox::TTool::toolAddCircle:
		AddCircle(point);
		break;
	// case ToolBox::TTool::toolJoint:
	// 	AddJoint(point,point);
	// 	break;
	// case ToolBox::TTool::toolVarBox:
	// 	AddVarBox(point);
	// 	break;
	// case ToolBox::TTool::toolAddHexagon:
	// 	AddHexagon(point);
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
		// case toolAddHexagon:
		// 	AddHexagon(point);
		// 	break;
		case ToolBox::TTool::toolAddCircle:
			AddCircle(point);
			break;
		// case toolAddPoly:
		// 	OnAddPoly(point);
		// 	break;
		}	
	}
}

void CSegundoVw::OnRButtonDown(uint64_t nFlags, sf::Vector2i point)
{
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

void CSegundoVw::OnPointer(sf::Vector2i ptWhere)
{
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
		
	b2Vec2 p = DeviceToWorld(ptWhere);

	b2Vec2 d(0.001f, 0.001f);
	b2AABB aabb;
	QueryCallback cb(p);

	aabb.upperBound = p + d;
	aabb.lowerBound = p - d;
	m_pDoc->m_pWorld->QueryAABB(&cb, aabb);

	if(!cb.m_fixture) return;

	float frequencyHz = 5.0f;
	float dampingRatio = 0.7f;

	b2Body* body = cb.m_fixture->GetBody();
	b2MouseJointDef jd;
	jd.bodyA = m_pDoc->m_pGroundBody;
	jd.bodyB = body;
	jd.target = p;
	jd.maxForce = 1000.0f * body->GetMass();
	b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

	m_pTempJoint = (b2MouseJoint*)m_pDoc->m_pWorld->CreateJoint(&jd);
	body->SetAwake(true);
	m_pGrabbed = body;

	ImGui::CaptureKeyboardFromApp(true);

/*
	CMainFrame *pFrm = (CMainFrame *)GetParentFrame();
	pFrm->m_barObjPrm.m_fAngular = m_pGrabbed->m_angularVelocity;
	pFrm->m_barObjPrm.m_fAtrito = m_pGrabbed->m_linearDamping;
//	pFrm->m_barObjPrm.m_fElasticidade = m_pGrabbed->m_torque;
	pFrm->m_barObjPrm.m_fLinear[0] = m_pGrabbed->m_linearVelocity.x;
	pFrm->m_barObjPrm.m_fLinear[1] = m_pGrabbed->m_linearVelocity.y;
	pFrm->m_barObjPrm.UpdateData(FALSE);
*/
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



void CSegundoVw::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(cx == 0 || cy == 0)
		return;

	if(m_bmpBb.m_hObject)
	{
		m_bmpBb.DeleteObject();
	}

	CPaintDC dc(this);
	m_bmpBb.CreateCompatibleBitmap(&dc,cx,cy);
	Invalidate();
}

BOOL CSegundoVw::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSegundoVw::OnPointerStep(void)
{
	if(m_pTempJoint)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		CRect rc;
		GetClientRect(rc);
		if(!rc.PtInRect(point))
			return;

		m_pTempJoint->SetTarget(MakeLP(point));
	}
}

void CSegundoVw::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_Tool == toolAddPoly && m_nCurPoly > 0)
	{
		FinalizePoly();
	}
	CView::OnRButtonDown(nFlags, point);
}

void CSegundoVw::OnRButtonUp(UINT nFlags, CPoint point)
{
	CView::OnRButtonUp(nFlags, point);
}

void CSegundoVw::OnObjetosBoxVar()
{
	m_toolbox.setCurrentTool(ToolBox::TTool::toolAddBox);
}

void CSegundoVw::OnUpdateObjetosBoxVar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolVarBox);
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
#pragma warning( default: 4244 )

void CSegundoVw::OnObjetosPolyline()
{
	m_Tool = toolAddPoly;
}

void CSegundoVw::OnKeyDown(uint64_t nChar, uint64_t nRepCnt, uint64_t nFlags)
{
}

void CSegundoVw::OnUpdateObjetosPolyline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolAddPoly);
}
void CSegundoVw::OnObjetosHex()
{
	m_Tool = toolAddHexagon;
}

void CSegundoVw::OnUpdateObjetosHex(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolAddHexagon);
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

void CSegundoVw::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DELETE && m_pGrabbed != NULL)
	{
		
		{b2World *pWorld = GetDocument()->m_Wa.GetWorld();
		if(m_pTempJoint != NULL)
		{
			pWorld->DestroyJoint(m_pTempJoint);
			m_pTempJoint = NULL;
		}
		pWorld->DestroyBody(m_pGrabbed);
		GetDocument()->m_Wa.ReleaseWorld();}

		m_pGrabbed = NULL;

		if(!m_bRunning) Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
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

#define RAD(a) (a*M_PI/180.0)
void CSegundoVw::AddHexagon(CPoint pt)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->m_barObjPrm.UpdateData();
	
	b2PolyDef polydef;
	polydef.density = pFrm->m_barObjPrm.m_fDensidade;
	polydef.friction = pFrm->m_barObjPrm.m_fAtrito;
	polydef.restitution = pFrm->m_barObjPrm.m_fElasticidade;
	polydef.vertexCount = 6;
	b2Vec2 pCenter = MakeLP(pt);
	for(int i= 0; i < 6; i++)
	{
		polydef.vertices[i].Set(cos(RAD(60*i))*pFrm->m_barObjPrm.m_fDim[0],
								sin(RAD(60*i))*pFrm->m_barObjPrm.m_fDim[1]);
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