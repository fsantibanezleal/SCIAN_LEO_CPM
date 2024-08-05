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
#define  FULLDFCS 1000

using namespace std;

class AgentsSystem
{
private:
public:
	bool		_bRunning,_fActive,_useAttachment,_useAdhesion;
	int			_numDFCAgents,_numDFCActiveAgents; 
	int			_selectedAgente;

	float       _radius;

	CellWM		*_agentsWM_DFC;
public:
	AgentsSystem();
	~AgentsSystem();

	void	Initiate(void); // random
	void	Initiate(EnvironmentSystem* vEnvironmentSystem); // setter
	void	UpdateState(EnvironmentSystem* vEnvironmentSystem);
	void	AddOffspring(EnvironmentSystem* vEnvironmentSystem);
};

#endif
