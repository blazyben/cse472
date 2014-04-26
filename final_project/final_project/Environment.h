#pragma once
#include "Cylinder.h"
#include "graphics/GrTexture.h"

class CEnvironment
{
public:
	CEnvironment(void);
	virtual ~CEnvironment(void);

	void SetPosition(double position) { m_position = position; }
	void Draw();
	void Table();
	void Holder();
	inline void Quad(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4);
	void Box(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color);
	void SetTexture(CGrTexture *texture) {m_wood_texture = texture;}
	void SetTexture_Leg(CGrTexture *texture) {m_wood_texture_leg = texture;}
	void SetTexture_Holder(CGrTexture *texture) {m_wood_holder = texture;}
    CGrTexture *GetTexture() {return m_wood_texture;}

private:
	double m_position;
	CGrTexture* m_wood_texture;
	CGrTexture* m_wood_holder;
	CGrTexture* m_wood_texture_leg;

	CCylinder   m_cylinder_left;
	CCylinder   m_cylinder_right;
};

