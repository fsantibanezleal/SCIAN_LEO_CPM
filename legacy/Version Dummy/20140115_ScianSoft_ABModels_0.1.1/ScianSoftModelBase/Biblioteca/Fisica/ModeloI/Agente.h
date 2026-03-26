#pragma once

#ifndef _Agente_
#define _Agente_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Ambiente.h"

using namespace std;

class Agente
{
private:
public:
	bool		_invalido;
	bool		_hija;
	bool		_perturbacionAzar,_liberarEstancado,_friendAttraction;
	bool		_busy,_fActive,_fRunning,_adhesionToEVL;
	int			_tipoAgente; // 0 = migradoras... 1 = las de atras... 2= EVL
	int			_persistence,_StepsforChange;

	point_3D	_color;
	point_3D	_posicion;
	point_3D	_velocidad;
	point_3D	_aceleracion;

	point_3D	_posicion_last;
	point_3D	_velocidad_last;
	point_3D	_aceleracion_last;

	float		_radio,_ratioFriendAttraction;
	
	virtual void	DireccionAzarosa(void);
	virtual	void	ReglasAmbiente(Ambiente vEnviroment);
public:
	Agente();
	~Agente();

	virtual	void	Inicializar(void); // azar
	virtual	void	Inicializar(point_3D vPosicion, float vRadio); // sete0
	virtual	void	UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix);
	virtual	void	UpdateEstadoBase(void);
};

#endif
