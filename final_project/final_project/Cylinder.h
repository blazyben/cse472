#pragma once

#include <vector>

#include "graphics/GrTexture.h"
#include "common.h"

class CCylinder
{
public:
	CCylinder(void);
	virtual ~CCylinder(void);

	void SetCapTexture(CGrTexture *texture) {m_captexture = texture;}
	void SetCylinderTexture(CGrTexture *texture) {m_cylindertexture = texture;}
	double CircumSteps() {return m_circumSteps;}

	void Draw();
	void InitRings();
	void SetLength(double length) {m_length = length;}
	void SetRadius(double radius) {m_radius = radius;}
	void SetCircumSteps(double steps) {m_circumSteps = steps;}
	void Deform(double position, double depth, int vertex = 0);

private:
	void CylinderVertex(double angle, double radius, double *vertex, double *normal);

	double m_radius;
	double m_length;
	double m_lengthSteps;
	double m_circumSteps;

	std::vector<std::vector<segment>> rings;

	CGrTexture* m_captexture;
	CGrTexture* m_cylindertexture;
};

