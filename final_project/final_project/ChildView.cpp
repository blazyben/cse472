// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "lab.h"
#include "ChildView.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_wood.LoadFile(L"textures/wood.bmp");
	m_wood_leg.LoadFile(L"textures/wood_leg.bmp");
	m_wood_holder.LoadFile(L"textures/wood_holder.bmp");
	m_environment.SetTexture(&m_wood);
	m_environment.SetTexture_Leg(&m_wood_leg);
	m_environment.SetTexture_Holder(&m_wood_holder);

	m_wireframe = false;
    m_camera.Set(15, 10, 30, 0, 0, 0, 0, 1, 0);

    m_spinangle = 0;
    m_spintimer = 0;
	m_spinindex = 0;

    SetDoubleBuffer(true);
	m_woodgrain.LoadFile(L"textures/woodgrain.jpg");
 	m_cylinder.SetTexture(&m_woodgrain);
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView,COpenGLWnd)
    //{{AFX_MSG_MAP(CChildView)
	ON_WM_KEYDOWN()
    ON_COMMAND(ID_STEP_SPIN, OnStepSpin)
    ON_UPDATE_COMMAND_UI(ID_STEP_SPIN, OnUpdateStepSpin)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_VIEW_WIREFRAME, &CChildView::OnViewWireframe)
    ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, &CChildView::OnUpdateViewWireframe)
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!COpenGLWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

    return TRUE;
}

void CChildView::OnGLDraw(CDC *pDC)
{
    GLfloat gray = 0.3f;
    glClearColor(gray, gray, gray, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    // Set up the camera
    //

    int wid, hit;
    GetSize(wid, hit);
    m_camera.Apply(wid, hit);

    //
    // Some standard parameters
    //

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Cull backfacing polygons
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

	// Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	GLfloat lightpos[] = {0.5, 2.0, 1.0, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	
	glEnable(GL_LIGHT1);
	GLfloat secondlightcolor[] = {0.2f, 0.8f, 0.2f, 1.0f};
	GLfloat secondlightpos[] = {-5.0f, -5.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, secondlightcolor);
    glLightfv(GL_LIGHT1, GL_SPECULAR, secondlightcolor);
	glLightfv(GL_LIGHT1, GL_POSITION, secondlightpos);

    // Enable or disable the wireframe mode
	glPolygonMode(GL_FRONT, m_wireframe ? GL_LINE : GL_FILL);

	glPushMatrix();
	DrawChisel();
	glPopMatrix();

    glPushMatrix();
	DrawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10,-7, -15);
	DrawEnvironment();
	glPopMatrix();

    glFlush();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_SPACE:
		// Reset
		m_chisel.SetDepth(3);
		m_chisel.SetPosition(0);
		Invalidate();
		break;
	case VK_UP:
		m_chisel.SetDepth(m_chisel.Depth()-.1);
		m_cylinder.Deform(m_chisel.Position(),m_chisel.Depth(),m_spinindex);
		Invalidate();
		break;
	case VK_DOWN:
		m_chisel.SetDepth(m_chisel.Depth()+.1);
		m_cylinder.Deform(m_chisel.Position(),m_chisel.Depth(),m_spinindex);
		Invalidate();
		break;
	case VK_LEFT:
		m_chisel.SetPosition(m_chisel.Position()-1);
		m_cylinder.Deform(m_chisel.Position(),m_chisel.Depth(),m_spinindex);
		Invalidate();
		break;
	case VK_RIGHT:
		m_chisel.SetPosition(m_chisel.Position()+1);
		m_cylinder.Deform(m_chisel.Position(),m_chisel.Depth(),m_spinindex);
		Invalidate();
		break;
	}
	COpenGLWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnViewWireframe()
{
    m_wireframe = !m_wireframe;
    Invalidate();
}

void CChildView::OnUpdateViewWireframe(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_wireframe);
}

void CChildView::DrawCylinder()
{
    glPushMatrix();

    glRotated(m_spinangle, 1, 0, 0);

    m_cylinder.Draw();

    glPopMatrix();
}

void CChildView::DrawChisel()
{
	glPushMatrix();
	m_chisel.Draw();
	glPopMatrix();
}

void CChildView::DrawEnvironment()
{
	m_environment.Draw();
}
void CChildView::OnStepSpin() 
{
    if(m_spintimer)
    {
        KillTimer(m_spintimer);
        m_spintimer = 0;
    }
    else
    {
        m_spintimer = SetTimer(1, 30, NULL);
    }
}

void CChildView::OnUpdateStepSpin(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_spintimer != 0);	
}

void CChildView::OnTimer(UINT nIDEvent) 
{
    m_spinangle += 360./m_cylinder.CircumSteps();
	m_spinindex++;
	if (m_spinindex == m_cylinder.CircumSteps()) {
		m_spinangle = 0;
		m_spinindex = 0;
	}
	m_cylinder.Deform(m_chisel.Position(),m_chisel.Depth(),m_spinindex);
    Invalidate();

    COpenGLWnd ::OnTimer(nIDEvent);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y);

    COpenGLWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_camera.MouseMove(point.x, point.y, nFlags))
        Invalidate();

    COpenGLWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y, 2);

    COpenGLWnd::OnRButtonDown(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_camera.MouseWheel(zDelta);
    Invalidate();

    return COpenGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}