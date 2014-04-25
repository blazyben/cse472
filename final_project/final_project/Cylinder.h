#pragma once

#include <vector>

#include "graphics/GrTexture.h"
#include "common.h"

class CCylinder
{
public:
	CCylinder(void);
	virtual ~CCylinder(void);

	void SetTexture(CGrTexture *texture) {m_texture = texture;}
    CGrTexture *GetTexture() {return m_texture;}
	double CircumSteps() {return m_circumSteps;}

	void Draw();
	void InitRings();
	void SetLength(double length) {m_length = length;}
	void SetRadius(double radius) {m_radius = radius;}
	void Deform(double position, double depth, int vertex = 0);

private:
	void CylinderVertex(double angle, double radius, double *vertex, double *normal);

	double m_radius;
	double m_length;
	double m_lengthSteps;
	double m_circumSteps;

	std::vector<std::vector<segment>> rings;

	CGrTexture* m_texture;
};

