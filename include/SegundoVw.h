#pragma once
// SegundoVw.h : interface of the CSegundoVw class
//
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "toolbox.h"
#include "params.h"
#include "SegundoDoc.h"
#define MAX_POLY_VERTEXES 8
#define afx_msg

class CSegundoVw
{
private:
	class CComputeFillColor
	{
	private:
		float m_fMaxImpulse;
		float m_fMaxMomentum;
		b2Body* m_pGrabbed;
		bool m_bShowMomentum;
		bool m_bShowImpulses;

	private:
		float GetImpulseSum(b2Body* pBody);

	public:
		CComputeFillColor(float fMaxImpulse, float fMaxMomentum, b2Body* pGrabbed, bool bShowMomentum, bool bShowImpulses)
		: m_fMaxImpulse(fMaxImpulse)
		, m_fMaxMomentum(fMaxMomentum)
		, m_pGrabbed(pGrabbed)
		, m_bShowMomentum(bShowMomentum)
		, m_bShowImpulses(bShowImpulses)
		{
		}

		sf::Color GetFill(b2Fixture* pFixture);
		sf::Color GetStroke(b2Fixture* pFixture);
	};
private:
	void DrawTempJoint(sf::RenderWindow& window);
	sf::Color GetFillColor(b2Fixture* pFixture);
// Attributes
public:
    ParamsBox m_paramsBox;
    ToolBox m_toolbox;

	bool m_bShowDemo;
	bool m_bShowToolbox;
	bool m_bShowParamsBox;


	CSegundoDoc* m_pDoc;
	
	b2Body *m_pGrabbed;
	b2MouseJoint *m_pTempJoint;

    
	sf::Vector2i m_ptPoly[MAX_POLY_VERTEXES];
	int	 m_nCurPoly;

	bool m_bRunning;
    sf::RenderWindow& m_Window;

// Operations
public:
	void OnDraw(sf::RenderWindow& window);

protected:
	void DrawShape(sf::RenderWindow& window, 
                   const b2Fixture* shape, 
				   const b2Transform& xf,
				   b2Vec2 position,
                   sf::Color crFill = sf::Color(255U, 255U, 255U), 
                   sf::Color crCont = sf::Color(0U,0U,0U));
	void Draw(sf::RenderWindow& window);
	void OnPointer(sf::Vector2i ptWhere);
	void AddBox(sf::Vector2i ptWhere);
	void AddCircle(sf::Vector2i ptWhere);
	void AddHexagon(sf::Vector2i pt);
	// void AddJoint(sf::Vector2i pt1, sf::Vector2i pt2);
	// void AddVarBox(sf::Vector2i pt);
	void FinalizePoly(void);

// Implementation
public:
    CSegundoVw(CSegundoDoc* pDoc, sf::RenderWindow& window);
	~CSegundoVw();

public:
	afx_msg void OnLButtonUp(uint64_t nFlags, sf::Vector2i point);
	afx_msg void OnLButtonDown(uint64_t nFlags, sf::Vector2i point);
	afx_msg void OnLButtonClicked(uint64_t nFlags, sf::Vector2i point);
	afx_msg void OnMouseMove(bool bShift, sf::Vector2i point);
	afx_msg void OnRButtonDown(uint64_t nFlags, sf::Vector2i point);

	void RemoveBody(sf::Vector2i point);
    b2Fixture* QueryFixture(b2Vec2 point);

	sf::Vector2f DeviceToLogical(sf::Vector2i devicePoint);
	b2Vec2 LogicalToWorld(sf::Vector2f);
    b2Vec2 DeviceToWorld(sf::Vector2i devicePoint);

    sf::Vector2f WorldToLogical(b2Vec2 worldPoint);
	sf::Vector2i LogicalToDevice(sf::Vector2f logicalPoint);
	sf::Vector2i WorldToDevice(b2Vec2 worldPoint);

	void ProcessSounds(void);
    bool IsPointer();
	bool IsGrabbed();

};
