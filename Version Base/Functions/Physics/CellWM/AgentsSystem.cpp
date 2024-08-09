#include "AgentsSystem.h"
#include <math.h>
#include <stdio.h>
#include <direct.h>
#include <windows.h>


AgentsSystem::AgentsSystem()
{
	_itTime             = 0;
	_numDFCActiveAgents = 0;
	_bRunning     = false;
	srand((unsigned)time(NULL));

	_radius			= 10;
	_numDFCAgents   = 25;
};
AgentsSystem::~AgentsSystem()
{

};
void	AgentsSystem::Initiate(void)
{

}; // azar
void	AgentsSystem::Initiate(EnvironmentSystem* vEnvironmentSystem)
{
	point_2D dummyP;
	_itTime             = 0;

	_numDFCActiveAgents	= 0;

	_bRunning           = false;
	_agentsWM_DFC		= new CellWM[FULLDFCS];

	for(register int i = 0; i < FULLDFCS; i++)
	{
		_agentsWM_DFC[i].SetActive(false);
	}

	_radius  = _radius;
	// Using boundaries for DFC
	point_3D posDFC_Init,posDFC_End,posDFC_Current;
	Igualar(posDFC_Init,vEnvironmentSystem->_minInitDFC);
	Igualar(posDFC_End,vEnvironmentSystem->_maxInitDFC);
	Igualar(posDFC_Current,posDFC_Init);
	
	posDFC_Current.x = posDFC_Init.x - _radius;
	posDFC_Current.y = posDFC_Init.y + _radius;
	float yMAX = 0.0f;
	for(register int i = 0; i < _numDFCAgents; i++){
		posDFC_Current.x = posDFC_Current.x + 2*_radius;
		
		if(posDFC_Current.x > posDFC_End.x - _radius)	{
			posDFC_Current.x = posDFC_Init.x + _radius;
			posDFC_Current.y = posDFC_Current.y + 2*_radius;
			dummyP.x = posDFC_Current.x;
			dummyP.y = posDFC_Current.y;
		}
		else{
			dummyP.x = posDFC_Current.x;
			dummyP.y = posDFC_Current.y;
		}

		if(posDFC_Current.y > posDFC_End.y  - _radius){
			i = _numDFCAgents;
		}
		else{
			_agentsWM_DFC[i].InitCell(dummyP,_radius);
			_agentsWM_DFC[i].SetActive(true);
			_numDFCActiveAgents++;
		}


		if(posDFC_Current.y > yMAX){
			yMAX = posDFC_Current.y;
		}
	}

	float minX = 10000.0f;
	float maxX = -10000.0f;

	for(register int i = 0; i < _numDFCAgents; i++)
	{
		if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() == yMAX))
		{
			if((_agentsWM_DFC[i].GetPosX() + _agentsWM_DFC[i].GetRad())> maxX)
			{
				maxX = (_agentsWM_DFC[i].GetPosX() + _agentsWM_DFC[i].GetRad());
			}
			if((_agentsWM_DFC[i].GetPosX() - _agentsWM_DFC[i].GetRad()) < minX)
			{
				minX = (_agentsWM_DFC[i].GetPosX() - _agentsWM_DFC[i].GetRad());
			}
		}
	}
	if( fabs(maxX - minX) < (vEnvironmentSystem->_maxInitDFC.x - vEnvironmentSystem->_minInitDFC.x))
	{
		float register delta = ((vEnvironmentSystem->_maxInitDFC.x - vEnvironmentSystem->_minInitDFC.x) - fabs(maxX - minX))/ 2.0f;
		for(register int i = 0; i < _numDFCAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() == yMAX))
			{
				_agentsWM_DFC[i].SetPosX(_agentsWM_DFC[i].GetPosX() + delta);
			}
		}
	}

	vEnvironmentSystem->_posMarginEVL = posDFC_Current.y + _radius;
} // To Set

void	AgentsSystem::UpdateState(EnvironmentSystem* vEnvironmentSystem) {
	register int i,j;
	if(_bRunning)
	{
		SaveCurrentState(vEnvironmentSystem);
		//if(vEnvironmentSystem->_useEVLInteraction)
		//{
		//	if(vEnvironmentSystem->_dataEVL_quantityKind == 1)
		//	{
		//		if(vEnvironmentSystem->_dataEVL_StepDistEVL_Current < 1 && _posRateAttachmentCurrent <= 8)
		//		{
		//			int dummyLimit;
		//			bool usingLimit;

		//			float posYMin,posYMax,posYMean = 0.0f;
		//			posYMin = 100000.0f;
		//			posYMax = -100000.0f;

		//			int numAnterior = 0;
		//			int numPosterior = 0;
		//			for(register int i = 0; i < _numDFCAgents; i++)
		//			{
		//				if(_agentsWM_DFC[i].GetActive())
		//				{
		//					if(_agentsWM_DFC[i].GetPosY() <= posYMin)
		//					{
		//						posYMin = _agentsWM_DFC[i].GetPosY();
		//					}
		//					if(_agentsWM_DFC[i].GetPosY() >= posYMax)
		//					{
		//						posYMax = _agentsWM_DFC[i].GetPosY();
		//					}
		//				}
		//			}
		//			posYMean = 0.5f*(posYMin + posYMax);
		//			for(register int i = 0; i < _numDFCActiveAgents; i++)
		//			{
		//				if(_agentsWM_DFC[i].GetActive())
		//				{
		//					if(_agentsWM_DFC[i].GetPosY() <= posYMean)
		//					{
		//						numAnterior++;
		//					}
		//					if(_agentsWM_DFC[i].GetPosY() > posYMean)
		//					{
		//						numPosterior++;
		//					}
		//				}
		//			}


		//			vEnvironmentSystem->_limit_DFC_EVL_AntAdhesion =  (int) ceil((float)numAnterior*_rateAntAttachment[_posRateAttachmentCurrent]);
		//			vEnvironmentSystem->_limit_DFC_EVL_PostAdhesion = (int) ceil((float)numPosterior*_ratePostAttachment[_posRateAttachmentCurrent]);

		//			if(vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior <= vEnvironmentSystem->_limit_DFC_EVL_AntAdhesion)
		//			{
		//				/// To set Anterior
		//				dummyLimit = 0;

		//				usingLimit= true;
		//				while(usingLimit && (vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior < vEnvironmentSystem->_limit_DFC_EVL_AntAdhesion))
		//				{
		//					for(register int i = 0; i < _numDFCActiveAgents; i++)
		//					{
		//						if(_agentsWM_DFC[i].GetActive())
		//						{
		//							if(!_agentsWM_DFC[i].GetAdhesionEVL() && (_agentsWM_DFC[i].GetPosY() <= posYMean))
		//							{
		//								if(((rand()%(int)(2)) == 0))
		//								{
		//									_agentsWM_DFC[i].SetAdhesionEVL(true);
		//									vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior++;
		//								}
		//								if(vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior >= vEnvironmentSystem->_limit_DFC_EVL_AntAdhesion)
		//								{
		//									i = _numDFCActiveAgents;
		//								}
		//							}
		//						}
		//					}
		//					if(dummyLimit >= 10000)
		//					{
		//						usingLimit = false;
		//					}
		//					dummyLimit++;
		//				}
		//			}
		//			else
		//			{
		//				for(register int i = 0; i < _numDFCActiveAgents; i++)
		//				{
		//					if((_agentsWM_DFC[i].GetActive()) && _agentsWM_DFC[i].GetAdhesionEVL())
		//					{
		//						_agentsWM_DFC[i].SetAdhesionEVL(false);
		//						vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior--;
		//						if(vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior <= vEnvironmentSystem->_limit_DFC_EVL_AntAdhesion)
		//						{
		//							i = _numDFCActiveAgents +2;
		//						}
		//					}
		//				}
		//			}

		//			if(vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior <= vEnvironmentSystem->_limit_DFC_EVL_PostAdhesion)
		//			{
		//				////// el otro
		//				dummyLimit = 0;
		//				usingLimit= true;
		//				while(usingLimit && (vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior < vEnvironmentSystem->_limit_DFC_EVL_PostAdhesion))
		//				{
		//					for(register int i = 0; i < _numDFCActiveAgents; i++)
		//					{
		//						if(_agentsWM_DFC[i].GetActive() && (!_agentsWM_DFC[i].GetAdhesionEVL() && (_agentsWM_DFC[i].GetPosY() > posYMean)))
		//						{
		//							if(((rand()%(int)(2)) == 0))
		//							{
		//								_agentsWM_DFC[i].SetAdhesionEVL(true);
		//								vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior++;
		//							}
		//							if(vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior >= vEnvironmentSystem->_limit_DFC_EVL_PostAdhesion)
		//							{
		//								i = _numDFCActiveAgents;
		//							}
		//						}
		//					}
		//					if(dummyLimit >= 10000)
		//					{
		//						usingLimit = false;
		//					}
		//					dummyLimit++;
		//				}
		//			}
		//			else
		//			{
		//				for(register int i = 0; i < _numDFCActiveAgents; i++)
		//				{
		//					if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetAdhesionEVL()))
		//					{
		//						_agentsWM_DFC[i].SetAdhesionEVL(false);
		//						vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior--;
		//						if(vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior <= vEnvironmentSystem->_limit_DFC_EVL_PostAdhesion)
		//						{
		//							i = _numDFCActiveAgents +2;
		//						}
		//					}
		//				}
		//			}

		//			
		//			vEnvironmentSystem->_dataEVL_StepDistEVL_Current =  vEnvironmentSystem->_dataEVL_ChangeStepsEVLDist;
		//			_posRateAttachmentCurrent++;
		//			if(_posRateAttachmentCurrent > 8) _posRateAttachmentCurrent = 8;
		//		}
		//		
		//		vEnvironmentSystem->_dataEVL_StepDistEVL_Current--;
		//	}
		//}


		for(i = 0; i < _numDFCAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive())
			{
				_agentsWM_DFC[i].SimulationStepPre();
				//_agentsWM_DFC[i].SimulationStepRestricted(vEnvironmentSystem,_agentsWM_DFC,_numDFCActiveAgents);

				_agentsWM_DFC[i].SimulationStepPost();
			}
		}


		AddOffspring(vEnvironmentSystem);

		// Correct shape collision
		// SemiFIX
		for(i = 0; i < _numDFCAgents; i++)
		{
			for(j = i + 1; j < _numDFCAgents; j++)
			{
				if(_agentsWM_DFC[i].GetActive() && _agentsWM_DFC[j].GetActive())
				{
					_agentsWM_DFC[i].FixOverlap(&(_agentsWM_DFC[j]),1.0f);
				}
			}
		}
		// Second FIX
		for(i = _numDFCAgents-1; i > -1; i--)
		{
			for(j = 0; j < i; j++)
			{
				if(_agentsWM_DFC[i].GetActive() && _agentsWM_DFC[j].GetActive())
				{
					_agentsWM_DFC[i].FixOverlap(&(_agentsWM_DFC[j]),0.0f);
				}
			}
		}
	}
};

void	AgentsSystem::AddOffspring(EnvironmentSystem* vEnvironmentSystem) {

};



void	AgentsSystem::SaveCurrentState(EnvironmentSystem* vEnvironmentSystem){

	FILE * pFile;

    char  filename[256];
	char* mainFolder = "OutComes";
    char* subFolder  = "OutComes/SubFolder";
    char* baseName   = "OutComes/SubFolder/Samples";
	char  time[256]; 
	char* extension  = ".dat";

	point_2D* _Poly;
	_mkdir(mainFolder);
	_mkdir(subFolder);

	strncpy(filename, baseName, sizeof(filename));
	sprintf(time, "%d", _itTime);
    strncat(filename, time, (sizeof(filename) - strlen(filename)) );
	strncat(filename, extension, (sizeof(filename) - strlen(filename)) );

	char buffer[] = { 'x' , 'y' , 'z' ,'\n'};

	pFile = fopen (filename, "wb");
	//fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
	fprintf(pFile,"%d ", _itTime);
	fprintf(pFile,"%d \r\n",_numDFCActiveAgents);

	for(int i = 0; i < _numDFCAgents; i++){
		if(_agentsWM_DFC[i].GetActive()){
			fprintf(pFile,"Cell %d %d %.5f %.5f \r\n", i,_agentsWM_DFC[i].GetNumNodesContour(),_agentsWM_DFC[i].GetPosX(),_agentsWM_DFC[i].GetPosY());
				
			_Poly = _agentsWM_DFC[i].GetPoly();
			for(int idxN = 0;idxN < _agentsWM_DFC[i].GetNumNodesContour();idxN++){
				fprintf(pFile,"    %.5f %.5f \r\n", _Poly[idxN].x, _Poly[idxN].y);

					
			}
		}
	}





	fclose (pFile);	
	
	
	
	
	_itTime++;

};

