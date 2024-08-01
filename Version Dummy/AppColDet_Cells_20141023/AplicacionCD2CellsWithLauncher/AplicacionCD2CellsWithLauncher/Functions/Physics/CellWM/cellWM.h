#pragma once

#ifndef _CellWM_H_
#define _CellWM_H_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

class CellWM
{
public:
	bool			_fBusy,_fActive,_fRunning;

	float			_radR0[10],_angleThetha0[10];
	float			_paramA[10],_paramW[10],_paramGamma[10],_paramSigma[10],_paramV0;
	point_2D		_pos,_vel;

	int				_nCont,_index,_paramNumFilo;

	polygon_2D		_polyMembrane;
public:
	bool			_hija,_perturbacionAzar,_friendAttraction,_liberarEstancado,_adhesionToEVL;
	float			_ratioFriendAttraction;
	int				_persistence,_StepsforChange;
	
	CellWM();
	~CellWM();

	void		InitCell(void);
	void		InitCell(point_2D vPos, float vRadio);

	// Setters
	bool		GetRunning(void){return _fRunning;};
	void		SetRunning(bool vValue){_fRunning = vValue;};

	bool		GetBusy(void){return _fBusy;};
	void		SetBusy(bool vValue){_fBusy = vValue;};

	int			GetNumNodesContour(void){return _nCont;};
	
	bool		GetActive(void){return _fActive;};
	void		SetActive(bool vValue){_fActive = vValue;};

	bool		GetAdhesionEVL(void){return _adhesionToEVL;};
	void		SetAdhesionEVL(bool vValue){_adhesionToEVL = vValue;};
	
	point_2D	GetPos(void){return _pos;};
	float		GetPosX(void){return _pos.x();};
	float		GetPosY(void){return _pos.y();};
	void		SetPos(point_2D vValue){_pos = point_2D(vValue.x(),vValue.y());};
	void		SetPosX(float vValue){_pos.x(vValue);};
	void		SetPosY(float vValue){_pos.y(vValue);};

	float		GetRad(int index){return _radR0[index];};
	void		SetRad(float vValue,int index){_radR0[index] = vValue;};

	polygon_2D	GetPoly(void){return _polyMembrane;};

	CellWM&     CellWM::operator=(const CellWM &vValue) {
					if (this == &vValue) return *this;  

					_fBusy			= false;
					_fActive		= false;
					_fRunning		= false;

					_pos			= vValue._pos;
					_vel			= vValue._vel;

					for(register int i = 0; i < 10 ; i++)
					{
						_radR0[i]			= vValue._radR0[i];
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
