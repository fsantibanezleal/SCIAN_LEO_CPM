#pragma once

#ifndef _Sistema_Agentes_
#define _Sistema_Agentes_

#include "BasicFunctions.h"
#include <math.h>
#include <cstdlib> 
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Ambiente.h"
#include "Agente_DFC.h"
#include "Agente_DEB.h"
#include "Agente_EVL.h"

using namespace std;

class Sistema_Agentes
{
private:
public:
	bool		_busy,_fActive,_fRunning;

	int			_numEVLAgents,_numDFCAgents,_numDEBAgents,_numDFCActiveAgents; 
	int			_selectedAgente;
	int			_tipoSelectedAgente; // 0 = migradoras... 1 = las de atras... 2= EVL
	int			_posicionPorAttActual;

	float		_porcentajeAttachmentAnterior[9];
	float		_porcentajeAttachmentPosterior[9];
	Agente_DFC	*_agentesDFC;
	Agente_EVL	*_agentesEVL;
	Agente_DEB	*_agentesDEB;
public:
	Sistema_Agentes();
	~Sistema_Agentes();

	void	Inicializar(void); // azar
	void	Inicializar(Ambiente* vEnviroment, int vNumDFCs, int vNumDEBs, int vNumEVLs); // sete0
	void	UpdateEstado(Ambiente* vEnviroment);
	void	AgregarHijos(Ambiente* vEnviroment);
};

#endif
