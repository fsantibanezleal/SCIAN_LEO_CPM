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

#include "EnvironmentSystem.h"

using namespace std;

class AgentsSystem
{
private:
public:
	bool		_bRunning,_fActive;
	int			_numEVLAgents,_numDFCAgents,_numDEBAgents,_numDFCActiveAgents; 
	int			_selectedAgente;
	int			_tipoSelectedAgente; // 0 = migradoras... 1 = las de atras... 2= EVL
	int			_posRateAttachmentCurrent;

	float		_rateAntAttachment[9];
	float		_ratePostAttachment[9];

	CellWM		*_agentsWM_DFC;
public:
	AgentsSystem();
	~AgentsSystem();

	void	Initiate(void); // azar
	void	Initiate(EnvironmentSystem* vEnvironmentSystem, int vNumDFCs, int vNumDEBs, int vNumEVLs); // sete0
	void	UpdateState(EnvironmentSystem* vEnvironmentSystem);
	void	AddOffspring(EnvironmentSystem* vEnvironmentSystem);
};

#endif
