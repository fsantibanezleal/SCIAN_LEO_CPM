#pragma once

#ifndef _BASICSTRUCTURES_H_
	#define _BASICSTRUCTURES_H_

//using namespace std;

#define PI (4.0f*atan(1.0f))

//typedef model::d2::point_xy<float>		point_2D;
//typedef model::polygon<point_2D>		polygon_2D;
class point_2D
{
public:
	float x,y;
public:
	point_2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	point_2D(const point_2D& vP)
	{
		x = vP.x; 
		y = vP.y;
	}

	point_2D(float vX, float vY)
	{
		x = vX;
		y = vY;
	}

	void SetPoint(point_2D vP)
	{
		x = vP.x; 
		y = vP.y;
	}
	void SetPoint(float vX, float vY)
	{
		x = vX;
		y = vY;
	}

};

//typedef pcl::PointXYZ<double>			point_3D;
class point_3D
{
public:
	float x,y,z;
public:
	point_3D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	point_3D(const point_2D& vP)
	{
		x = vP.x; 
		y = vP.y;
		z = 0.0f;
	}

	point_3D(const point_3D& vP)
	{
		x = vP.x; 
		y = vP.y;
		z = vP.z;
	}
	point_3D(float vX, float vY, float vZ)
	{
		x = vX;
		y = vY;
		z = vZ;
	}

	void SetPoint(point_3D vP)
	{
		x = vP.x; 
		y = vP.y;
		z = vP.z;
	}
	void SetPoint(float vX, float vY, float vZ)
	{
		x = vX;
		y = vY;
		z = vZ;
	}

};


#endif
