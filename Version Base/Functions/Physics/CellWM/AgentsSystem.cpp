#include "AgentsSystem.h"
#include <math.h>

AgentsSystem::AgentsSystem()
{
	_rateAntAttachment[0] = 7.0f/21.0f;
	_rateAntAttachment[1] = 8.0f/21.0f;
	_rateAntAttachment[2] = 10.0f/22.0f;
	_rateAntAttachment[3] = 10.0f/22.0f;
	_rateAntAttachment[4] = 7.0f/24.0f;
	_rateAntAttachment[5] = 6.0f/24.0f;
	_rateAntAttachment[6] = 1.0f/32.0f;
	_rateAntAttachment[7] = 1.0f/32.0f;
	_rateAntAttachment[8] = 0.0f/32.0f;

	_ratePostAttachment[0] = 10.0f/21.0f;
	_ratePostAttachment[1] = 9.0f/21.0f;
	_ratePostAttachment[2] = 8.0f/22.0f;
	_ratePostAttachment[3] = 8.0f/22.0f;
	_ratePostAttachment[4] = 6.0f/24.0f;
	_ratePostAttachment[5] = 7.0f/24.0f;
	_ratePostAttachment[6] = 4.0f/32.0f;
	_ratePostAttachment[7] = 4.0f/32.0f;
	_ratePostAttachment[8] = 0.0f/32.0f;
	_posRateAttachmentCurrent = 0;

	_numEVLAgents = 0;
	_numDFCActiveAgents = 0;
	_numDEBAgents = 0;
	_bRunning     = false;
	srand((unsigned)time(NULL));
	//_agentsWM_DFC = new Agente_DFC[_numDFCActiveAgents];
};
AgentsSystem::~AgentsSystem()
{

};
void	AgentsSystem::Initiate(void)
{

}; // azar
void	AgentsSystem::Initiate(EnvironmentSystem* vEnvironmentSystem, int vNumDFCs, int vNumDEBs, int vNumEVLs)
{
	point_2D dummyP;
	vEnvironmentSystem->_numDFCActiveAgentsWanted = vNumDFCs;
	_numDFCAgents		= 1000;
	_numDFCActiveAgents	= 0;
	_numDEBAgents		= vNumDEBs;
	_numEVLAgents		= vNumEVLs;

	_bRunning           = false;
	_agentsWM_DFC		= new CellWM[_numDFCAgents];

	for(register int i = 0; i < _numDFCAgents; i++)
	{
		_agentsWM_DFC[i].SetActive(false);
	}

	_posRateAttachmentCurrent = 0;
	vEnvironmentSystem->_num_DFC_EVL_AdhesionAnterior = 0;
	vEnvironmentSystem->_num_DFC_EVL_AdhesionPosterior = 0;
	vEnvironmentSystem->_dataEVL_StepDistEVL_Current = 0;
	// Using boundaries for DFC
	point_3D posDFC_Init,posDFC_End,posDFC_Current;
	Igualar(posDFC_Init,vEnvironmentSystem->_minInitDFC);
	//posDFC_Init.y = posDFC_Init.y + 0.05f*float(vEnvironmentSystem->_dimY);
	Igualar(posDFC_End,vEnvironmentSystem->_maxInitDFC);
	Igualar(posDFC_Current,posDFC_Init);
	float radius = 10;

	posDFC_Current.x = posDFC_Init.x - radius;
	posDFC_Current.y = posDFC_Init.y + radius;
	float yMAX = 0.0f;
	for(register int i = 0; i < vEnvironmentSystem->_numDFCActiveAgentsWanted; i++)
	{
		posDFC_Current.x = posDFC_Current.x + 2*radius;
		
		if(posDFC_Current.x > posDFC_End.x)
		{
			posDFC_Current.x = posDFC_Init.x + radius;
			posDFC_Current.y = posDFC_Current.y + 2*radius;
			dummyP.x = posDFC_Current.x;
			dummyP.y = posDFC_Current.y;
			_agentsWM_DFC[i].InitCell(dummyP,radius);
			_agentsWM_DFC[i].SetActive(true);
			_numDFCActiveAgents++;
		}
		else
		{
			dummyP.x = posDFC_Current.x;
			dummyP.y = posDFC_Current.y;
			_agentsWM_DFC[i].InitCell(dummyP,radius);
			_agentsWM_DFC[i].SetActive(true);

			_numDFCActiveAgents++;
		}
		if(posDFC_Current.y > yMAX)
		{
			yMAX = posDFC_Current.y;
		}
	}

	float minX = 10000.0f;
	float maxX = -10000.0f;

	for(register int i = 0; i < _numDFCActiveAgents; i++)
	{
		if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() == yMAX))
		{
			if((_agentsWM_DFC[i].GetPosX() + _agentsWM_DFC[i].GetRad(0))> maxX)
			{
				maxX = (_agentsWM_DFC[i].GetPosX() + _agentsWM_DFC[i].GetRad(0));
			}
			if((_agentsWM_DFC[i].GetPosX() - _agentsWM_DFC[i].GetRad(0)) < minX)
			{
				minX = (_agentsWM_DFC[i].GetPosX() - _agentsWM_DFC[i].GetRad(0));
			}
		}
	}
	if( fabs(maxX - minX) < (vEnvironmentSystem->_maxInitDFC.x - vEnvironmentSystem->_minInitDFC.x))
	{
		float register delta = ((vEnvironmentSystem->_maxInitDFC.x - vEnvironmentSystem->_minInitDFC.x) - fabs(maxX - minX))/ 2.0f;
		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() == yMAX))
			{
				_agentsWM_DFC[i].SetPosX(_agentsWM_DFC[i].GetPosX() + delta);
			}
		}
	}
}; // To Set
void	AgentsSystem::UpdateState(EnvironmentSystem* vEnvironmentSystem) {
	register int i,j;
	if(_bRunning)
	{
		for(i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive())
			{
				_agentsWM_DFC[i].SimulationStep();
				//_agentsWM_DFC[i].SimulationStepRestricted(vEnvironmentSystem,_agentsWM_DFC,_numDFCActiveAgents);
			}
		}

		//if(vEnvironmentSystem->_useEVLInteraction)
		//{
		//	if( (vEnvironmentSystem->_dataEVL_FieldKind == 1) && (vEnvironmentSystem->_dataEVL_LineWidth > 20)  && (vEnvironmentSystem->_dataEVL_currentLineStep < 1))
		//	{
		//		vEnvironmentSystem->_dataEVL_LineWidth =vEnvironmentSystem->_dataEVL_LineWidth--;
		//		if(vEnvironmentSystem->_dataEVL_LineWidth < 20)
		//		{
		//			vEnvironmentSystem->_dataEVL_LineWidth = 20;
		//		}
		//		vEnvironmentSystem->_dataEVL_currentLineStep = vEnvironmentSystem->_dataEVL_LineStep;
		//	}
		//	vEnvironmentSystem->_dataEVL_currentLineStep--;
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
		//			for(register int i = 0; i < _numDFCActiveAgents; i++)
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

		////////////// actualizar los parametros del EnvironmentSystem,,, XD
		vEnvironmentSystem->Update();

		//// verificar si el arrastre del EnvironmentSystem afecto el sistema
		//// primera iteracion para asignar los afectados por el borde superior
		//register bool bInvalideState = true;
		//for(i = 0; i < _numDFCActiveAgents; i++)
		//{
		//	if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() < (vEnvironmentSystem->_posMarginDEB.y + _agentsWM_DFC[i].GetRad(0))))
		//	{
		//		_agentsWM_DFC[i].SetPosY(_agentsWM_DFC[i].GetPosY() + 1);
		//	}
		//}
		//////////////// arreglar -... hasta validar---
		//register float distance = 0.0f;
		//while(bInvalideState)
		//{
		//	bInvalideState = false;
		//	for(i = 0; i < _numDFCActiveAgents; i++)
		//	{
		//		if(_agentsWM_DFC[i].GetActive())
		//		{
		//			for(j = i + 1; j < _numDFCActiveAgents - 1; j++)
		//			{
		//				if(_agentsWM_DFC[j].GetActive())
		//				{
		//					point_3D p3D_1,p3D_2;
		//					SetFull(p3D_1,_agentsWM_DFC[i].GetPosX(),_agentsWM_DFC[i].GetPosY(), 0.0);
		//					SetFull(p3D_2,_agentsWM_DFC[j].GetPosX(),_agentsWM_DFC[j].GetPosY(), 0.0);
		//					distance = Distance(p3D_1,p3D_2);
		//					if(distance < (_agentsWM_DFC[i].GetRad(0) +  _agentsWM_DFC[j].GetRad(0)))
		//					{
		//						bInvalideState = true;
		//						if(_agentsWM_DFC[i].GetPosY() >= _agentsWM_DFC[j].GetPosY())
		//						{
		//							_agentsWM_DFC[i].SetPosY(_agentsWM_DFC[i].GetPosY() + 1);
		//						}
		//						else
		//						{
		//							_agentsWM_DFC[j].SetPosY(_agentsWM_DFC[j].GetPosY() + 1);
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
		////// Fin fixed....

		///// arreglar desde abajo

		//for(i = 0; i < _numDFCActiveAgents; i++)
		//{
		//	if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() > (vEnvironmentSystem->_dataEVL_PosY)))
		//	{
		//		_agentsWM_DFC[i].SetPosY(_agentsWM_DFC[i].GetPosY() - 1);
		//	}
		//}

		//bInvalideState = true;
		//while(bInvalideState)
		//{
		//	bInvalideState = false;
		//	for(i = 0; i < _numDFCActiveAgents; i++)
		//	{
		//		if(_agentsWM_DFC[i].GetActive())
		//		{
		//			for(j = i + 1; j < _numDFCActiveAgents - 1; j++)
		//			{
		//				if(_agentsWM_DFC[j].GetActive())
		//				{
		//					point_3D p3D_1,p3D_2;
		//					SetFull(p3D_1,_agentsWM_DFC[i].GetPosX(),_agentsWM_DFC[i].GetPosY(), 0.0);
		//					SetFull(p3D_2,_agentsWM_DFC[j].GetPosX(),_agentsWM_DFC[j].GetPosY(), 0.0);
		//					distance = Distance(p3D_1,p3D_2);
		//					if(distance < (_agentsWM_DFC[i].GetRad(0) +  _agentsWM_DFC[j].GetRad(0)))
		//					{
		//						bInvalideState = true;
		//						if(_agentsWM_DFC[i].GetPosY() <= _agentsWM_DFC[j].GetPosY())
		//						{
		//							_agentsWM_DFC[i].SetPosY(_agentsWM_DFC[i].GetPosY() - 1);
		//						}
		//						else
		//						{
		//							_agentsWM_DFC[j].SetPosY(_agentsWM_DFC[j].GetPosY() - 1);
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		//AddOffspring(vEnvironmentSystem);


		for(i = 0; i < _numDFCActiveAgents; i++)
		{
			for(j = i + 1; j < _numDFCActiveAgents - 1; j++)
			{
				if(_agentsWM_DFC[i].GetActive() && _agentsWM_DFC[j].GetActive())
				{
					//_agentsWM_DFC[i].FixOverlap(&(_agentsWM_DFC[j]));
				}
			}
		}

	}
};

int deadTime = 0;
void	AgentsSystem::AddOffspring(EnvironmentSystem* vEnvironmentSystem) {
	if(vEnvironmentSystem->_StepsProliferationCurrent < 1 && vEnvironmentSystem->_posProliferation < 7)
	{
		vEnvironmentSystem->_posProliferation++;
		vEnvironmentSystem->_numDFCActiveAgentsWanted = (int) vEnvironmentSystem->_numDFCActiveAgentsWanted + (int) ceil(_numDFCActiveAgents*(vEnvironmentSystem->_vProliferation[vEnvironmentSystem->_posProliferation]));

		vEnvironmentSystem->_StepsProliferationCurrent = vEnvironmentSystem->_stepsProliferacion;
	}

	bool dummyLimit = true;
	int  nTries = 0;
	point_3D posMine;
	register int largo = (int) vEnvironmentSystem->_maxInitDFC.x - (int) vEnvironmentSystem->_minInitDFC.x;
	register float distance;

	if(deadTime > 200)
	{
		deadTime = 0;
	}
	if((deadTime == 0) &&(_numDFCActiveAgents < vEnvironmentSystem->_numDFCActiveAgentsWanted) && dummyLimit)
	{
		int limitLeft =   100000;//vEnvironmentSystem->_minInitDFC.x + (rand()%(int)(largo));
		int limitRight   =   -100000;//vEnvironmentSystem->_maxInitDFC.x - _agentsWM_DFC[0].GetRad(0);

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentsWM_DFC[k].GetActive())
			{
				if(_agentsWM_DFC[k].GetPosX() < limitLeft)
				{
					limitLeft = (int) _agentsWM_DFC[k].GetPosX();
					if(limitLeft < vEnvironmentSystem->_minEnvironmentSystem.x + _agentsWM_DFC[k].GetRad(0))
					{
						limitLeft = (int) vEnvironmentSystem->_minEnvironmentSystem.x + (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
				if(_agentsWM_DFC[k].GetPosX()> limitRight)
				{
					limitRight = (int) _agentsWM_DFC[k].GetPosX();
					if(limitRight > vEnvironmentSystem->_maxEnvironmentSystem.x - _agentsWM_DFC[k].GetRad(0))
					{
						limitRight = (int) vEnvironmentSystem->_maxEnvironmentSystem.x - (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
			}
		}
		float dimmm = 0.15f*(limitRight-limitLeft);
		limitLeft = limitLeft +  (int) dimmm  ;//(rand()%(int)dimmm);
		limitRight = limitRight     -  (int) dimmm  ;//(rand()%(int)dimmm);

		bool bValid = true;
		bool bComeHere = false;
		Zero(posMine);

		int coordY_Init =   100000;//vEnvironmentSystem->_minInitDFC.x + (rand()%(int)(largo));
		int coordY_End   =   -100000;//vEnvironmentSystem->_maxInitDFC.x - _agentsWM_DFC[0].GetRad(0);

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentsWM_DFC[k].GetActive())
			{
				if(_agentsWM_DFC[k].GetPosY()< coordY_Init)
				{
					coordY_Init = (int) _agentsWM_DFC[k].GetPosY();
					if(coordY_Init < vEnvironmentSystem->_posMarginDEB.y + _agentsWM_DFC[k].GetRad(0))
					{
						coordY_Init = (int) vEnvironmentSystem->_posMarginDEB.y + (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
				if(_agentsWM_DFC[k].GetPosY() > coordY_End)
				{
					coordY_End = (int) _agentsWM_DFC[k].GetPosY();
					if(coordY_End > vEnvironmentSystem->_dataEVL_PosY - _agentsWM_DFC[k].GetRad(0))
					{
						coordY_End = (int) vEnvironmentSystem->_dataEVL_PosY - (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
			}
		}
		coordY_End = coordY_End - (int) (0.25f*(coordY_End-coordY_Init));
		//coordY_Init = coordY_Init + (rand()%(int)((coordY_End-coordY_Init)/2));
		for(register int i = limitLeft ; i < limitRight; i++)
		{
			for(register int j = coordY_Init; j < coordY_End; j++)
			{
				bValid = true;
				posMine.x = float(i);
				posMine.y = float(j);
				posMine.z = 0.0f;
				for(register int k = 0; k < _numDFCActiveAgents; k++)
				{
					if(_agentsWM_DFC[k].GetActive())
					{
						bComeHere = true;
						point_3D p3D_1;
						SetFull(p3D_1,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
						distance = Distance(posMine,p3D_1);
						if(distance <= 2.0f*_agentsWM_DFC[k].GetRad(0))
						{
							bValid = false;
						}
					}
				}
				if(bValid)
				{
					i = 1000000;
					j = 1000000;
				}
			}
		}

		if(bValid && bComeHere)
		{
			int distance_Min = 10000;
			point_3D direction;
			for(register int k = 0; k < _numDFCActiveAgents; k++)
			{
				if(_agentsWM_DFC[k].GetActive())
				{
					point_3D p3D_1;
					SetFull(p3D_1,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
					distance = Distance(posMine,p3D_1);
					if(distance < distance_Min)
					{
						distance_Min = (int) distance;

						point_3D p3D_2;
						SetFull(p3D_2,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
						Subtraction(direction,p3D_2,posMine);
						NormalizeR(direction);
					}
				}
			}

			if((distance_Min != 10000) && (distance_Min > 2*_agentsWM_DFC[0].GetRad(0)))
			{
				distance = distance - 2*_agentsWM_DFC[0].GetRad(0);
				Amplify(direction,distance,direction);
				//Addition(posMine,posMine,direction);
			}
			
			point_2D p2D_1;
			p2D_1.x = posMine.x;
			p2D_1.y = posMine.y;

			_agentsWM_DFC[_numDFCActiveAgents].InitCell(p2D_1,10);
			_agentsWM_DFC[_numDFCActiveAgents]._offSpring				= true;
			_agentsWM_DFC[_numDFCActiveAgents]._perturbationRandom		= _agentsWM_DFC[0]._perturbationRandom;
			_agentsWM_DFC[_numDFCActiveAgents]._friendAttraction		= _agentsWM_DFC[0]._friendAttraction;
			_agentsWM_DFC[_numDFCActiveAgents]._freeIssue				= _agentsWM_DFC[0]._freeIssue;
			_agentsWM_DFC[_numDFCActiveAgents]._adhesionToEVL			= false;
			_agentsWM_DFC[_numDFCActiveAgents]._ratioFriendAttraction	= _agentsWM_DFC[0]._ratioFriendAttraction;

			_numDFCActiveAgents++;
		}
		nTries++;
		if(nTries >= 1)
		{
			dummyLimit = false;
		}
	}
	vEnvironmentSystem->_StepsProliferationCurrent--;
};