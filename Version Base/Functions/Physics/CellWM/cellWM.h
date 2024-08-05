#pragma once

#ifndef _CellWM_H_
#define _CellWM_H_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAXFILO 10

using namespace std;

class CellWM
{
public:
	bool			_bActive;

	float			_radR0,_angleThetha0[MAXFILO];
	float			_paramA[MAXFILO],_paramW[MAXFILO],_paramGamma[MAXFILO],_paramSigma[MAXFILO],_paramV0;
	point_2D		_pos,_vel;

	int				_nCont,_index,_paramNumFilo;

	point_2D		*_polyMembrane;
public:
	bool			_bOffSpring,_bStochasticMov,_bDFCsInteraction,_bEVL_Interaction;
	float			_adhesionDistance;
	
	CellWM();
	~CellWM();

	void		InitCell(void);
	void		InitCell(point_2D vPos, float vRadio);

	// Setters
	int			GetNumNodesContour(void){return _nCont;};
	
	bool		GetActive(void){return _bActive;};
	void		SetActive(bool vValue){_bActive = vValue;};

	bool		GetEVL_Interaction(void){return _bEVL_Interaction;};
	void		SetEVL_Interaction(bool vValue){_bEVL_Interaction = vValue;};
	
	point_2D	GetPos(void){return _pos;};
	float		GetPosX(void){return _pos.x;};
	float		GetPosY(void){return _pos.y;};
	void		SetPos(point_2D vValue){_pos.x = vValue.x;_pos.y = vValue.y;};
	void		SetPosX(float vValue){_pos.x = vValue;};
	void		SetPosY(float vValue){_pos.y = vValue;};

	float		GetRad(){return _radR0;};
	void		SetRad(float vValue){_radR0 = vValue;};

	point_2D*	GetPoly(void){return _polyMembrane;};

	CellWM&     CellWM::operator=(const CellWM &vValue) {
					if (this == &vValue) return *this;  

					_bActive		= false;

					_pos			= vValue._pos;
					_vel			= vValue._vel;

					_radR0			= vValue._radR0;
					for(register int i = 0; i < MAXFILO ; i++)
					{
						_angleThetha0[i]	= vValue._angleThetha0[i];

						_paramA[i]			= vValue._paramA[i];
						_paramW[i]			= vValue._paramW[i];
						_paramGamma[i]		= vValue._paramGamma[i];
						_paramSigma[i]		= vValue._paramSigma[i];
					}
					_paramV0		= vValue._paramV0;

					_nCont			= vValue._nCont;

					CreateContour();
					return *this; 
				}
	// Updating contour
	float	FunP(float vValue, int index);
	void	CreateContour(void);

	// Base de Calculo
	void	UdpdateAngle(void);
	void	UpdateShape(void);

	float	EstimateV(void);
	void	UpdatePos(void);

	void	FixOverlap(CellWM *vValue);


	void	SimulationStep(void);

};

#endif
