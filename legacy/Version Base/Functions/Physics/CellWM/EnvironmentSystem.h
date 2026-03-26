#pragma once

#ifndef _EnvironmentSystem_
#define _EnvironmentSystem_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NUMSTAGES 5

using namespace std;
class EnvironmentSystem
{
private:
public:
	bool		_busy,_fActive,_bRunning ,_bShowInitialRect, _bShowMargin_EVL,_bShowMargin_DEB;

	float		_gapMargin,_posMarginDEB, _posMarginEVL, _numItbyMin;
	point_3D	_dimWS, _dimDFCs;
	point_3D	_minEnvironmentSystem, _maxEnvironmentSystem;
	point_3D	_minInitDFC, _maxInitDFC;

	float		_velMarginEVL;
	int			_stateEVL;
	float       _dataEVL_PosRef[NUMSTAGES];
	float       _dataEVL_RatioAttach[NUMSTAGES];
	float       _dataEVL_RatioRing[NUMSTAGES];

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void		Initiate(void);
	void		Reinitiate(void);
	void		Update(void);
};

#endif
