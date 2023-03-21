#pragma once
#include <Box2D.h>

class CPrimeiroDoc
{
// Attributes
public:
	CWorldAccess m_Wa;
	COLORREF	m_crBack;
// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CPrimeiroDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	afx_msg void OnEditarPrefer();
};
