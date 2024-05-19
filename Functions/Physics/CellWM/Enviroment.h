#pragma once

#ifndef _Enviroment_
#define _Enviroment_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

class Enviroment
{
private:
public:
	bool		_busy,_fActive,_bRunning,_useDEBAttraction,_useEVLInteraction,_useEVLInteractionAdhesion;

	int			_StepsProliferationCurrent,_posProliferation,_numDFCActiveAgentsWanted,_stepsProliferacion;
	float		_vProliferation[8];

	int			_dataEVL_quantityKind; // 0 == todo... 1 == seleccionadas
	int			_dataEVL_FieldKind; // 0 == puntual... 1 == linea.... 2 == distribuido
	int			_num_DFC_EVL_AdhesionPosterior,_num_DFC_EVL_AdhesionAnterior,_limit_DFC_EVL_AntAdhesion,_limit_DFC_EVL_PostAdhesion;
	int			_stepsMovDEB_margin,_actualStepsMovDEB_margin,_stepsMovEVL_margin,_actualStepsMovEVL_margin;


	int         _dataEVL_PosY,_dataEVL_currentLineStep,_dataEVL_LineStep,_dataEVL_LineWidth,_dataEVL_ChangeStepsEVLDist,_dataEVL_IncludedCampos;
	float		_dataEVL_FactorEVL;

	int			_dataEVL_actualStepDistEVL;
	point_3D	_minEnviroment;
	point_3D	_maxEnviroment;
	point_3D	_posMarginDEB;

	point_3D	_minInitDFC;
	point_3D	_maxInitDFC;

	point_3D	_color;

	int			_dimX,_dimY,_dimZ;
	int			**_tipoNodoInGrid;
	int			**_numNodoInGrid;

	float		_factorDFC_DEB_Action;
public:
	Enviroment();
	~Enviroment();

	void		Inicializar(void);
	void		Update(void);
};

#endif
