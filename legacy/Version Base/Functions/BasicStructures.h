#pragma once

#ifndef _BASICSTRUCTURES_H_
	#define _BASICSTRUCTURES_H_

#include <boost/foreach.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

//using namespace std;
using namespace boost::geometry;

#define PI (4.0f*atan(1.0f))

typedef model::d2::point_xy<float>		point_2D;

typedef model::polygon<point_2D>		polygon_2D;

//typedef pcl::PointXYZ<double>			point_3D;
struct point_3D
{
	float x,y,z;
};

#endif
