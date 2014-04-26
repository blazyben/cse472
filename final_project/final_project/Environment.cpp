#include "stdafx.h"
#include "gl/gl.h"
#include "Environment.h"



CEnvironment::CEnvironment(void)
{
	m_cylinder_left.SetLength(4.8);
	m_cylinder_left.SetRadius(.5);
	m_cylinder_left.SetCircumSteps(5);
	m_cylinder_left.InitRings();
	m_cylinder_right.SetLength(3);
	m_cylinder_right.SetRadius(.5);
	m_cylinder_right.SetCircumSteps(5);
	m_cylinder_right.InitRings();
}


CEnvironment::~CEnvironment(void)
{
}

void CEnvironment::Draw()
{
	if(m_wood_holder != NULL)
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, m_wood_holder->TexName()); 
	}

	Table();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, m_wood_texture->TexName()); 
	Holder();
	glDisable(GL_TEXTURE_2D);
}

void CEnvironment::Table()
{
	const double RED[] = {0.7, 0.0, 0.0};

	glPushMatrix();
	glTranslated(-2.7, 0, 0);
	
	Box(70, 2, 30, RED);

	glTranslated(0, -50, 0);
	Box(3, 50, 3, RED);
	glTranslated(0, 0, 27);
	Box(3, 50, 3, RED);
	glTranslated(67, 0, 0);
	Box(3, 50, 3, RED);
	glTranslated(0, 0, -27);
	Box(3, 50, 3, RED);

	glPopMatrix();
}

void CEnvironment::Holder()
{
	const double RED[] = {0.7, 0.0, 0.0};

	glPushMatrix();
	
	glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, m_wood_texture->TexName()); 
	glTranslated(1.5,6.5,15);
	glTranslated(-1.5,-6.4,-5);
	Box(7, 9.5, 10, RED);

	glTranslated(5.3,6.5,5);
	m_cylinder_left.Draw();
	glTranslated(1.7,-6.5,-5);
	Box(55, 3, 10, RED);
	glTranslated(55,2,0);
	Box(2, 8, 10, RED);
	glTranslated(-3,5,5);
	m_cylinder_right.Draw();

	glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

inline void CEnvironment::Quad(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4)
{

    glBegin(GL_QUADS);
    glVertex3dv(v1);
    glVertex3dv(v2);
    glVertex3dv(v3);
    glVertex3dv(v4);
    glEnd();
}


void CEnvironment::Box(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color)
{

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
    GLdouble a[] = {0., 0., p_z};
    GLdouble b[] = {p_x, 0., p_z};
    GLdouble c[] = {p_x, p_y, p_z};
    GLdouble d[] = {0., p_y, p_z};
    GLdouble e[] = {0., 0., 0.};
    GLdouble f[] = {p_x, 0., 0.};
    GLdouble g[] = {p_x, p_y, 0.};
    GLdouble h[] = {0., p_y, 0.};

	

    // Front
    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2f(0, 0);
    glVertex3dv(a);
    glTexCoord2f(1, 0);
    glVertex3dv(b);
    glTexCoord2f(1, 1);
    glVertex3dv(c);
    glTexCoord2f(0, 1);
    glVertex3dv(d);
    glEnd();

	//left
	glBegin(GL_QUADS);
    glNormal3d(-1, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3dv(e);
    glTexCoord2f(0, 0);
    glVertex3dv(a);
    glTexCoord2f(0, 1);
    glVertex3dv(d);
    glTexCoord2f(1, 1);
    glVertex3dv(h);
    glEnd();

    
    // Right
    glBegin(GL_QUADS);
    glNormal3d(1, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3dv(c);
    glTexCoord2f(0, 0);
    glVertex3dv(b);
    glTexCoord2f(1, 0);
    glVertex3dv(f);
    glTexCoord2f(1, 1);
    glVertex3dv(g);
    glEnd();

    

    // Back
    glBegin(GL_QUADS);
	    glNormal3d(0, 0, -1);
		glTexCoord2f(0, 1);
        glVertex3dv(g);
		glTexCoord2f(0, 0);
        glVertex3dv(f);
		glTexCoord2f(1, 0);
        glVertex3dv(e);
		glTexCoord2f(1, 1);
        glVertex3dv(h);
    glEnd();


    // Top
    glBegin(GL_QUADS);
	    glNormal3d(0, 1, 0);
		glTexCoord2f(0,1);
        glVertex3dv(h);
		glTexCoord2f(0, 0);
        glVertex3dv(d);
		glTexCoord2f(1, 0);
        glVertex3dv(c);
		glTexCoord2f(1, 1);
        glVertex3dv(g);
    glEnd();

    // Bottom
    glBegin(GL_QUADS);
	    glNormal3d(0, -1, 0);
		glTexCoord2f(0, 0);
        glVertex3dv(b);
		glTexCoord2f(0, 0);
        glVertex3dv(a);
		glTexCoord2f(0, 0);
        glVertex3dv(e);
		glTexCoord2f(0, 0);
        glVertex3dv(f);
    glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}
