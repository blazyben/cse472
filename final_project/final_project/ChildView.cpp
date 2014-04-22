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
	m_wireframe = false;
    m_camera.Set(15, 10, 30, 0, 0, 0, 0, 1, 0);

    m_spinangle = 0.;
    m_spintimer = 0;

    SetDoubleBuffer(true);

	m_marble.LoadFile(L"textures/marble10.bmp");
	m_marble2.LoadFile(L"textures/marble03.bmp");

	m_torus1.SetTexture(&m_marble2);
	m_torus2.SetTexture(&m_marble);
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView,COpenGLWnd )
    //{{AFX_MSG_MAP(CChildView)
    ON_COMMAND(ID_STEP_SPIN, OnStepSpin)
    ON_UPDATE_COMMAND_UI(ID_STEP_SPIN, OnUpdateStepSpin)
    ON_WM_TIMER()
    ON_COMMAND(ID_STEP_TORUS, OnStepTorus)
    ON_UPDATE_COMMAND_UI(ID_STEP_TORUS, OnUpdateStepTorus)
    //}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_VIEW_WIREFRAME, &CChildView::OnViewWireframe)
    ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, &CChildView::OnUpdateViewWireframe)
    ON_COMMAND(ID_STEP_TORUS2, &CChildView::OnStepTorus2)
    ON_UPDATE_COMMAND_UI(ID_STEP_TORUS2, &CChildView::OnUpdateStepTorus2)
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_STEP_FUNKY, &CChildView::OnDrawFunky)
	ON_UPDATE_COMMAND_UI(ID_STEP_FUNKY, &CChildView::OnUpdateStepFunky)
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

    switch(m_mode)
    {

    case ID_STEP_TORUS:
        m_torus1.SetSteps1(50);
        m_torus1.SetSteps2(20);
        m_torus2.SetSteps1(50);
        m_torus2.SetSteps2(20);

        DrawTori();
        break;

    case ID_STEP_TORUS2:
        m_torus1.SetSteps1(70);
        m_torus1.SetSteps2(15);
        m_torus2.SetSteps1(40);
        m_torus2.SetSteps2(15);

        DrawTori();
        break;

	case ID_STEP_FUNKY:
        glPushMatrix();
        glRotated(m_spinangle / 3., 0, 1, 0);
        DrawFunky();
        glPopMatrix();

		break;

    }

    glPopMatrix();

    glFlush();
}

double Normal3dv(double *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
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

//
// Name :         CChildView::DrawTori()
// Description :  Draw two tori linked together.
//
void CChildView::DrawTori()
{
    glPushMatrix();

    // This rotation spins the tori
    glRotated(m_spinangle / 3, 0, 1, 0);

    // The flips them down 45 degress so they look nicer.
    glRotated(45., 1, 0, 0);

    // First torus is left 2.5 cm
    glPushMatrix();
    glTranslated(-2.5, 0, 0);

    m_torus1.Draw();

    glPopMatrix();

    // Second torus is right 2.5 cm and rotated
    // around the X axis to right angles with the first
    // torus.
    glPushMatrix();

    glTranslated(2.5, 0, 0);
    glRotated(90., 1, 0, 0);

    m_torus2.Draw();

    glPopMatrix();

    glPopMatrix();
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
    m_spinangle += 10;
    Invalidate();

    COpenGLWnd ::OnTimer(nIDEvent);
}


void CChildView::OnStepTorus() 
{
    m_mode = ID_STEP_TORUS;
    Invalidate();
}

void CChildView::OnUpdateStepTorus(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_mode == ID_STEP_TORUS);
}

void CChildView::OnStepTorus2()
{
    m_mode = ID_STEP_TORUS2;
    Invalidate();
}

void CChildView::OnUpdateStepTorus2(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_mode == ID_STEP_TORUS2);
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


void CChildView::OnDrawFunky()
{
	// TODO: Add your command handler code here
    m_mode = ID_STEP_FUNKY;
    Invalidate();
}

void CChildView::OnUpdateStepFunky(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_mode == ID_STEP_FUNKY);
}


double length3dv(double *v)
{
   return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void normal(double *v1, double *v2, double *normal)
{
   GLdouble v1xv2[] = {v1[1] * v2[2] - v2[1] * v1[2], v1[2] * v2[0] - v2[2] * v1[0], v1[0] * v2[1] - v2[0] * v1[1]};
   double len = length3dv(v1xv2);
   normal[0] = v1xv2[0] / len;
   normal[1] = v1xv2[1] / len;
   normal[2] = v1xv2[2] / len;
}

//
//        Name : CChildView::DrawFunky()
// Description : Draw a funky object.
//

void CChildView::DrawFunky(void)
{
	GLdouble d[] = {0, 8, -5};
    GLdouble a[] = {5, 5.5, 2};
    GLdouble b[] = {-3.7, 2, -3.3};
    GLdouble c[] = {3.5, -2.3, 1.1};

	GLdouble ab[] = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
	GLdouble ad[] = {d[0] - a[0], d[1] - a[1], d[2] - a[2]};
	GLdouble ac[] = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
	GLdouble db[] = {b[0] - d[0], b[1] - d[1], b[2] - d[2]};
	GLdouble dc[] = {c[0] - d[0], c[1] - d[1], c[2] - d[2]};

	GLdouble abxac[3];
	normal(ab,ac,abxac);
    glNormal3d(abxac[0], abxac[1], abxac[2]);
    
	// Front
    glBegin(GL_POLYGON);
        glVertex3dv(a);
        glVertex3dv(b);
        glVertex3dv(c);
    glEnd();

	GLdouble adxab[3];
	normal(ad,ab,adxab);
	glNormal3d(adxab[0], adxab[1], adxab[2]);

    // Sides
    glBegin(GL_POLYGON);
        glVertex3dv(a);
        glVertex3dv(d);
        glVertex3dv(b);
    glEnd();

	GLdouble dcxdb[3];
	normal(dc,db,dcxdb);
	glNormal3d(dcxdb[0], dcxdb[1], dcxdb[2]);

    glBegin(GL_POLYGON);
        glVertex3dv(b);
        glVertex3dv(d);
        glVertex3dv(c);
    glEnd();

	GLdouble acxad[3];
	normal(ac,ad,acxad);
	glNormal3d(acxad[0], acxad[1], acxad[2]);

    glBegin(GL_POLYGON);
        glVertex3dv(c);
        glVertex3dv(d);
        glVertex3dv(a);
    glEnd();

}