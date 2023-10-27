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
	  m_bRunning(false)
	, m_toolbox()
	, m_pGrabbed(nullptr)
	, m_pTempJoint(nullptr)
	, m_nCurPoly(0)
	, m_dwStep(0)
    , m_pDoc(pDoc)
    , m_Window(window)
{
	m_Window.setView(sf::View(sf::FloatRect(-100, -100, 200, 200)));
	// Abaixo é código antigo, que ia implementar ar (aerodinamica)
	// m_circle_def.radius = 2.0f;
	// m_circle_def.density = 10.0f;
	// m_circle_def.friction = 0.0f;
	// m_circle_def.restitution = 1.0f;

    // m_bodyDefAr.position.Set(-99.0f,(float32)((rand()%200)-100.0f));
    // b2Body* m_bodyAr = m_pDoc->m_pWorld->CreateBody(&m_bodyDefAr);
    // m_bodyAr->CreateShape(&m_circle_def);
    // m_bodyAr->SetMassFromShapes();

//	m_bodyDefAr.angularVelocity = 0.0f;
//	m_bodyDefAr.linearVelocity.Set( 50.0f,0);
}

CSegundoVw::~CSegundoVw()
{
}

// CSegundoVw drawing
void CSegundoVw::OnDraw(sf::RenderWindow& window)
{
	m_pDoc->m_pWorld->Step(1.0f/60.0f, 10, 10);

    m_paramsBox.draw();
    m_toolbox.draw();
	Draw(window);
	
	b2World *pWorld = m_pDoc->m_pWorld;
	int32 nQtdBodies = pWorld->GetBodyCount();
	// string strStat = format("Número de corpos: {}", nQtdBodies);
	// pFrm->SetSbText(strStat);
}

CRect GetClientRect(sf::RenderWindow& window)
{
    sf::Vector2i size = (sf::Vector2i)window.getSize();
    sf::Vector2f wsize = window.mapPixelToCoords(size);
    return CRect(0,0,wsize.x,wsize.y);
}

void CSegundoVw::Draw(sf::RenderWindow& window)
{
	// CCronometro cr;
	// cr.Start();
	CRect rc = GetClientRect(window);
    float border = 5;
    sf::RectangleShape rect(sf::Vector2f(rc.width-2*border, rc.height-2*border));
    rect.setPosition(border, border);
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1 );
    window.draw(rect);

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
		if(!b->IsAwake())
		{
			vecDel.push_back(b);
		}
		for (b2Fixture* s = b->GetFixtureList(); s; s = s->GetNext())
		{
			// FILL:
			if(!b->IsAwake())
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
			
			if(b->GetMass() > 0 && fImpulses/b->GetMass() > 1.0)
			{
				FragmentaObjeto(b);
			}
			else
			{
				DrawShape(window, s, crFill, crStroke);
			}
		}
	}
	m_maxImpulse = maxImpulse;


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

void CSegundoVw::FragmentaObjeto(b2Body *b)
{
#if 0
	b2World *world = GetDocument()->m_Wa.GetWorld();
	float w = b->GetAngularVelocity();
	b2Vec2  r = b->GetLinearVelocity();
	float32 m = b->GetMass();
	if(m == 0)
	{
		GetDocument()->m_Wa.ReleaseWorld();
		return;
	}
	
	// Cada shape ser� um novo objeto.
	// Cada novo objeto ser� o shape de origem fragmentado
	// O n�mero de fragmentos ser� um rand�mico entre 2 e 5
	// Por enquanto os fragmentos ser�o tri�ngulos aleat�rios.
	for (b2Shape* s = b->m_shapeList; s; s = s->m_next)
	{
		switch (s->m_type)
		{
		case e_circleShape:
/*
			const b2CircleShape* circle = (const b2CircleShape*)shape;
			b2Vec2 x = circle->m_position;
			float32 r = circle->m_radius;
			CRect rcBall(x.x-r,x.y-r,x.x+r,x.y+r);

			pDc->Ellipse(rcBall);

			b2Vec2 ax = circle->m_R.col1;
			pDc->MoveTo(x.x,x.y);
			pDc->LineTo(x.x + r * ax.x, x.y + r * ax.y);
*/
			break;

		case e_polyShape:
			{
				const b2PolyShape* poly = (const b2PolyShape*)s;
				b2Vec2 vCenter(0,0);
				int32 i;
				for (i = 0; i < poly->m_vertexCount; i++)
				{
					vCenter += b2Mul(poly->m_R, poly->m_vertices[i]);
				}
				vCenter.x /= poly->m_vertexCount;
				vCenter.y /= poly->m_vertexCount;

				b2PolyDef polydef;
				polydef.density		= 1.0f;
				polydef.friction	= s->m_friction;
				polydef.restitution = s->m_restitution;
				polydef.vertexCount = 3;
				vector<b2Body *> vecPartes;
				b2BodyDef* pDef = new b2BodyDef;
				for (i = 0; i < poly->m_vertexCount; i++)
				{
					polydef.vertices[0] = vCenter;
					polydef.vertices[1] = poly->m_vertices[i];
					if(i == 0)
						polydef.vertices[2] = poly->m_vertices[poly->m_vertexCount-1];
					else
						polydef.vertices[2] = poly->m_vertices[i-1];

					pDef->position = vCenter;
					pDef->angularVelocity = 0;
					pDef->linearVelocity.Set( 0, 0);
					pDef->AddShape(&polydef);
					vecPartes.push_back(world->CreateBody(pDef));
					delete pDef;
					pDef = new b2BodyDef;
				}
				delete pDef;
			}
			break;
		}
	}
	world->DestroyBody(b);
	GetDocument()->m_Wa.ReleaseWorld();
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

void CSegundoVw::OnSimulacao(void)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->m_barObjPrm.UpdateData();
	pFrm->m_barObjPrm.m_dwStep = 1000.0/m_dwStep;
	pFrm->m_barObjPrm.UpdateData(FALSE);
	Invalidate();
}
#endif


void CSegundoVw::DrawShape(sf::RenderWindow& window, const b2Fixture* shape, sf::Color crFill, sf::Color crCont)
{
	// CBrush *pOldBrush, brushFill;
	// CPen   *pOldPen, penCont;

	// brushFill.CreateSolidBrush(crFill);
	// penCont.CreatePen(PS_SOLID,0,crCont);


	// pOldBrush = pDc->SelectObject(&brushFill);
	// pOldPen   = pDc->SelectObject(&penCont);
	switch (shape->GetType())
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
			b2Vec2 x = circle->m_p;
			float r = circle->m_radius;
			CRect rcBall(x.x-r,x.y-r,x.x+r,x.y+r);
            sf::CircleShape circle_shape(r);
            circle_shape.setPosition(x.x, x.y);
            circle_shape.setFillColor(crFill);
            circle_shape.setOutlineColor(crCont);
            circle_shape.setOutlineThickness(1);
            window.draw(circle_shape);

			// b2Vec2 ax = circle->m_R.col1;
			// pDc->MoveTo(x.x,x.y);
			// pDc->LineTo(x.x + r * ax.x, x.y + r * ax.y);
		}
		break;

	case b2Shape::Type::e_polygon:
		{
			const b2PolygonShape* poly = (const b2PolygonShape*)shape->GetShape();
            size_t vertexCount = poly->m_count;
            sf::ConvexShape convex(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i)
            {
                b2Vec2 v = poly->m_vertices[i];
                convex.setPoint(i, sf::Vector2f(v.x, v.y));
            }
			window.draw(convex);
			// // pDc->BeginPath();			
			// for (int32 i = 0; i < poly->GetVertexCount(); ++i)
			// {
			// 	b2Vec2 v = poly->m_position + b2Mul(poly->m_R, poly->m_vertices[i]);
			// 	if(i == 0)
			// 		pDc->MoveTo(v.x,v.y);
			// 	else
			// 		pDc->LineTo(v.x,v.y);
			// }
			// pDc->CloseFigure();
			// pDc->EndPath();
			// pDc->StrokeAndFillPath();
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
    // TODO: Completar aqui
	// case ToolBox::TTool::toolPointer:
	// 	OnPointer(point);
	// 	break;
	case ToolBox::TTool::toolAddBox:
		AddBox(point);
		break;
	// case ToolBox::TTool::toolAddCircle:
	// 	AddCircle(point);
	// 	break;
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

void CSegundoVw::AddBox(sf::Vector2i ptWhere)
{
    // TODO: Adicionar janela de parâmetros para pegá-los aqui.
	// CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	// pFrm->m_barObjPrm.UpdateData();
	
	b2PolygonShape boxDef;
	boxDef.SetAsBox(5.0f, 5.0f);

	b2BodyDef bodyDef;
    bodyDef.position = DeviceToWorld(ptWhere);

	cout << "Creating box at " << bodyDef.position.x << ", " << bodyDef.position.y << endl;

	b2Body* body = m_pDoc->m_pWorld->CreateBody(&bodyDef);
	b2FixtureDef fixdef;
	fixdef.shape = &boxDef;
	fixdef.density = 1.0f;
    fixdef.friction = 0.3f;
    fixdef.restitution = 0.5f;
    body->CreateFixture(&fixdef);
	body->SetAngularVelocity(0);
	body->SetLinearVelocity( b2Vec2(0, 0));
}

void CSegundoVw::OnMouseMove(bool bShift, sf::Vector2i point)
{
	if(!m_bRunning && m_toolbox.getCurrentTool() == ToolBox::TTool::toolPointer && m_pGrabbed != NULL)
	{
		// TODO: Completar aqui
		// m_pGrabbed->SetXForm(DeviceToWorld(point), m_pGrabbed->GetAngle());
	}

    // TODO: Completar aqui
	// if(bShift  && ((CMainFrame *)GetParent())->m_barObjPrm.m_fDensidade != 0.0f)
	// {
	// 	switch(m_Tool)
	// 	{
	// 	case toolAddBox:
	// 		AddBox(point);
	// 		break;
	// 	case toolAddHexagon:
	// 		AddHexagon(point);
	// 		break;
	// 	case toolAddCircle:
	// 		AddCircle(point);
	// 		break;
	// 	case toolAddPoly:
	// 		OnAddPoly(point);
	// 		break;
	// 	}	
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

#if 0
void CSegundoVw::AddCircle(CPoint ptWhere)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->m_barObjPrm.UpdateData();

	b2CircleDef sdc;
	sdc.radius = pFrm->m_barObjPrm.m_fDim[0];
	sdc.density = pFrm->m_barObjPrm.m_fDensidade;
	sdc.friction = pFrm->m_barObjPrm.m_fAtrito;
	sdc.restitution = pFrm->m_barObjPrm.m_fElasticidade;

    b2BodyDef bodyDef;
    bodyDef.position = MakeLP(ptWhere);
	bodyDef.angularVelocity = pFrm->m_barObjPrm.m_fAngular;
	bodyDef.linearVelocity.Set( pFrm->m_barObjPrm.m_fLinear[0],
								pFrm->m_barObjPrm.m_fLinear[1]);
	bodyDef.AddShape(&sdc);
	b2Body* body = GetDocument()->m_Wa.GetWorld()->CreateBody(&bodyDef);
	GetDocument()->m_Wa.ReleaseWorld();

	if(!m_bRunning)
		Invalidate();
}

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

void CSegundoVw::OnPointer(CPoint ptWhere)
{
	CPrimeiroDoc* pDoc = GetDocument();

	b2Vec2 p = MakeLP(ptWhere);

	b2AABB aabb;
	aabb.minVertex.Set(p.x-.01f,p.y-.01f);
	aabb.maxVertex.Set(p.x+.01f,p.y+.01f);
	b2Shape *shape;
	b2World *pWorld = pDoc->m_Wa.GetWorld();
	int32 count = pWorld->Query(aabb,&shape,1);
	if(count == 0 || shape->GetBody() == pWorld->GetGroundBody() || !shape->TestPoint(p))
	{
		pDoc->m_Wa.ReleaseWorld();
		m_pGrabbed = NULL;
		return;
	}

	m_pGrabbed = shape->GetBody();

	if(m_bRunning)
	{
		b2Vec2 vecAnchor;
		vecAnchor = p;
		m_pGrabbed->GetLocalPoint(p);

		b2MouseJointDef jointDef;
		jointDef.body1 = pWorld->GetGroundBody();
		jointDef.body2 = m_pGrabbed;
		jointDef.target = vecAnchor;
		jointDef.maxForce = 100000.0f * m_pGrabbed->m_mass;
		if(m_pTempJoint != NULL)
		{
			pWorld->DestroyJoint(m_pTempJoint);
		}

		m_pTempJoint = (b2MouseJoint *)pWorld->CreateJoint(&jointDef);
	}
	m_pGrabbed->WakeUp();
	SetCapture();
	pDoc->m_Wa.ReleaseWorld();
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

void CSegundoVw::OnObjetosBox()
{
	m_Tool = toolAddBox;
}

void CSegundoVw::OnObjetosCirculo()
{
	m_Tool = toolAddCircle;
}

void CSegundoVw::OnUpdateObjetosBox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolAddBox);
}

void CSegundoVw::OnUpdateObjetosCirculo(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolAddCircle);
}

void CSegundoVw::OnUpdateObjetosPointer(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolPointer);
}

void CSegundoVw::OnObjetosPointer()
{
	m_Tool = toolPointer;
}

void CSegundoVw::OnObjetosJun()
{
	m_Tool = toolJoint;
}

void CSegundoVw::OnUpdateObjetosJun(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Tool == toolJoint);
}

bool operator !=(b2Vec2 a, b2Vec2 b)
{
	return a.x != b.x || a.y != b.y;
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

BOOL CSegundoVw::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CView::OnSetCursor(pWnd, nHitTest, message);
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