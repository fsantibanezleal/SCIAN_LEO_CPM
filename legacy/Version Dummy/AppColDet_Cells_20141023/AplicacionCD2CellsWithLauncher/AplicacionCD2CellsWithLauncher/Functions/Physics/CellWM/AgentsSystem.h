#pragma once

#ifndef _AgentsSystem_
#define _AgentsSystem_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>


#include "cellWM.h"

#include "Enviroment.h"

using namespace std;

class AgentsSystem
{
private:
public:
	bool		_fRunning,_fActive;
	int			_numEVLAgents,_numDFCAgents,_numDEBAgents,_numDFCActiveAgents; 
	int			_selectedAgente;
	int			_tipoSelectedAgente; // 0 = migradoras... 1 = las de atras... 2= EVL
	int			_posPorAttActual;

	float		_porcentajeAttachmentAnterior[9];
	float		_porcentajeAttachmentPosterior[9];

	CellWM		*_agentsWM_DFC;
public:
	AgentsSystem();
	~AgentsSystem();

	void	Inicializar(void); // azar
	void	Inicializar(Enviroment* vEnviroment, int vNumDFCs, int vNumDEBs, int vNumEVLs); // sete0
	void	UpdateEstado(Enviroment* vEnviroment);
	void	AgregarHijos(Enviroment* vEnviroment);
};

#endif
