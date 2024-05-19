#include "AgentsSystem.h"
#include <math.h>

AgentsSystem::AgentsSystem()
{
	_porcentajeAttachmentAnterior[0] = 7.0f/21.0f;
	_porcentajeAttachmentAnterior[1] = 8.0f/21.0f;
	_porcentajeAttachmentAnterior[2] = 10.0f/22.0f;
	_porcentajeAttachmentAnterior[3] = 10.0f/22.0f;
	_porcentajeAttachmentAnterior[4] = 7.0f/24.0f;
	_porcentajeAttachmentAnterior[5] = 6.0f/24.0f;
	_porcentajeAttachmentAnterior[6] = 1.0f/32.0f;
	_porcentajeAttachmentAnterior[7] = 1.0f/32.0f;
	_porcentajeAttachmentAnterior[8] = 0.0f/32.0f;

	_porcentajeAttachmentPosterior[0] = 10.0f/21.0f;
	_porcentajeAttachmentPosterior[1] = 9.0f/21.0f;
	_porcentajeAttachmentPosterior[2] = 8.0f/22.0f;
	_porcentajeAttachmentPosterior[3] = 8.0f/22.0f;
	_porcentajeAttachmentPosterior[4] = 6.0f/24.0f;
	_porcentajeAttachmentPosterior[5] = 7.0f/24.0f;
	_porcentajeAttachmentPosterior[6] = 4.0f/32.0f;
	_porcentajeAttachmentPosterior[7] = 4.0f/32.0f;
	_porcentajeAttachmentPosterior[8] = 0.0f/32.0f;
	_posPorAttActual = 0;

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
void	AgentsSystem::Inicializar(void)
{

}; // azar
void	AgentsSystem::Inicializar(Enviroment* vEnviroment, int vNumDFCs, int vNumDEBs, int vNumEVLs)
{
	vEnviroment->_numDFCActiveAgentsWanted = vNumDFCs;
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

	_posPorAttActual = 0;
	vEnviroment->_num_DFC_EVL_AdhesionAnterior = 0;
	vEnviroment->_num_DFC_EVL_AdhesionPosterior = 0;
	vEnviroment->_dataEVL_actualStepDistEVL = 0;
	// Using boundaries for DFC
	point_3D inicioDFC,finDFC,actualPos;
	Igualar(inicioDFC,vEnviroment->_minInitDFC);
	//inicioDFC.y = inicioDFC.y + 0.05f*float(vEnviroment->_dimY);
	Igualar(finDFC,vEnviroment->_maxInitDFC);
	Igualar(actualPos,inicioDFC);
	float radio = 10;

	actualPos.x = inicioDFC.x - radio;
	actualPos.y = inicioDFC.y + radio;
	float yMAX = 0.0f;
	for(register int i = 0; i < vEnviroment->_numDFCActiveAgentsWanted; i++)
	{
		actualPos.x = actualPos.x + 2*radio;
		
		if(actualPos.x > finDFC.x)
		{
			actualPos.x = inicioDFC.x + radio;
			actualPos.y = actualPos.y + 2*radio;
			_agentsWM_DFC[i].InitCell(point_2D(actualPos.x,actualPos.y),radio);
			_agentsWM_DFC[i].SetActive(true);
			_numDFCActiveAgents++;
		}
		else
		{
			_agentsWM_DFC[i].InitCell(point_2D(actualPos.x,actualPos.y),radio);
			_agentsWM_DFC[i].SetActive(true);

			_numDFCActiveAgents++;
		}
		if(actualPos.y > yMAX)
		{
			yMAX = actualPos.y;
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
	if( fabs(maxX - minX) < (vEnviroment->_maxInitDFC.x - vEnviroment->_minInitDFC.x))
	{
		float register delta = ((vEnviroment->_maxInitDFC.x - vEnviroment->_minInitDFC.x) - fabs(maxX - minX))/ 2.0f;
		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() == yMAX))
			{
				_agentsWM_DFC[i].SetPosX(_agentsWM_DFC[i].GetPosX() + delta);
			}
		}
	}
}; // sete0
void	AgentsSystem::UpdateState(Enviroment* vEnviroment) {
	register int i,j;
	if(_bRunning)
	{
		for(i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentsWM_DFC[i].GetActive())
			{
				_agentsWM_DFC[i].SimulationStep();
				//_agentsWM_DFC[i].SimulationStepRestricted(vEnviroment,_agentsWM_DFC,_numDFCActiveAgents);
			}
		}

		//if(vEnviroment->_useEVLInteraction)
		//{
		//	if( (vEnviroment->_dataEVL_FieldKind == 1) && (vEnviroment->_dataEVL_LineWidth > 20)  && (vEnviroment->_dataEVL_currentLineStep < 1))
		//	{
		//		vEnviroment->_dataEVL_LineWidth =vEnviroment->_dataEVL_LineWidth--;
		//		if(vEnviroment->_dataEVL_LineWidth < 20)
		//		{
		//			vEnviroment->_dataEVL_LineWidth = 20;
		//		}
		//		vEnviroment->_dataEVL_currentLineStep = vEnviroment->_dataEVL_LineStep;
		//	}
		//	vEnviroment->_dataEVL_currentLineStep--;
		//	if(vEnviroment->_dataEVL_quantityKind == 1)
		//	{
		//		if(vEnviroment->_dataEVL_actualStepDistEVL < 1 && _posPorAttActual <= 8)
		//		{
		//			int dummyLimit;
		//			bool usingMisericordia;

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


		//			vEnviroment->_limit_DFC_EVL_AntAdhesion =  (int) ceil((float)numAnterior*_porcentajeAttachmentAnterior[_posPorAttActual]);
		//			vEnviroment->_limit_DFC_EVL_PostAdhesion = (int) ceil((float)numPosterior*_porcentajeAttachmentPosterior[_posPorAttActual]);

		//			if(vEnviroment->_num_DFC_EVL_AdhesionAnterior <= vEnviroment->_limit_DFC_EVL_AntAdhesion)
		//			{
		//				/// Asignar Anterior
		//				dummyLimit = 0;

		//				usingMisericordia= true;
		//				while(usingMisericordia && (vEnviroment->_num_DFC_EVL_AdhesionAnterior < vEnviroment->_limit_DFC_EVL_AntAdhesion))
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
		//									vEnviroment->_num_DFC_EVL_AdhesionAnterior++;
		//								}
		//								if(vEnviroment->_num_DFC_EVL_AdhesionAnterior >= vEnviroment->_limit_DFC_EVL_AntAdhesion)
		//								{
		//									i = _numDFCActiveAgents;
		//								}
		//							}
		//						}
		//					}
		//					if(dummyLimit >= 10000)
		//					{
		//						usingMisericordia = false;
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
		//						vEnviroment->_num_DFC_EVL_AdhesionAnterior--;
		//						if(vEnviroment->_num_DFC_EVL_AdhesionAnterior <= vEnviroment->_limit_DFC_EVL_AntAdhesion)
		//						{
		//							i = _numDFCActiveAgents +2;
		//						}
		//					}
		//				}
		//			}

		//			if(vEnviroment->_num_DFC_EVL_AdhesionPosterior <= vEnviroment->_limit_DFC_EVL_PostAdhesion)
		//			{
		//				////// el otro
		//				dummyLimit = 0;
		//				usingMisericordia= true;
		//				while(usingMisericordia && (vEnviroment->_num_DFC_EVL_AdhesionPosterior < vEnviroment->_limit_DFC_EVL_PostAdhesion))
		//				{
		//					for(register int i = 0; i < _numDFCActiveAgents; i++)
		//					{
		//						if(_agentsWM_DFC[i].GetActive() && (!_agentsWM_DFC[i].GetAdhesionEVL() && (_agentsWM_DFC[i].GetPosY() > posYMean)))
		//						{
		//							if(((rand()%(int)(2)) == 0))
		//							{
		//								_agentsWM_DFC[i].SetAdhesionEVL(true);
		//								vEnviroment->_num_DFC_EVL_AdhesionPosterior++;
		//							}
		//							if(vEnviroment->_num_DFC_EVL_AdhesionPosterior >= vEnviroment->_limit_DFC_EVL_PostAdhesion)
		//							{
		//								i = _numDFCActiveAgents;
		//							}
		//						}
		//					}
		//					if(dummyLimit >= 10000)
		//					{
		//						usingMisericordia = false;
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
		//						vEnviroment->_num_DFC_EVL_AdhesionPosterior--;
		//						if(vEnviroment->_num_DFC_EVL_AdhesionPosterior <= vEnviroment->_limit_DFC_EVL_PostAdhesion)
		//						{
		//							i = _numDFCActiveAgents +2;
		//						}
		//					}
		//				}
		//			}

		//			
		//			vEnviroment->_dataEVL_actualStepDistEVL =  vEnviroment->_dataEVL_ChangeStepsEVLDist;
		//			_posPorAttActual++;
		//			if(_posPorAttActual > 8) _posPorAttActual = 8;
		//		}
		//		
		//		vEnviroment->_dataEVL_actualStepDistEVL--;
		//	}
		//}

		////////////// actualizar los parametros del Enviroment,,, XD
		vEnviroment->Update();

		//// verificar si el arrastre del Enviroment afecto el sistema
		//// primera iteracion para asignar los afectados por el borde superior
		//register bool bInvalideState = true;
		//for(i = 0; i < _numDFCActiveAgents; i++)
		//{
		//	if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() < (vEnviroment->_posMarginDEB.y + _agentsWM_DFC[i].GetRad(0))))
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
		//	if(_agentsWM_DFC[i].GetActive() && (_agentsWM_DFC[i].GetPosY() > (vEnviroment->_dataEVL_PosY)))
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

		//AddOffspring(vEnviroment);


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
void	AgentsSystem::AddOffspring(Enviroment* vEnviroment) {
	if(vEnviroment->_StepsProliferationCurrent < 1 && vEnviroment->_posProliferation < 7)
	{
		vEnviroment->_posProliferation++;
		vEnviroment->_numDFCActiveAgentsWanted = (int) vEnviroment->_numDFCActiveAgentsWanted + (int) ceil(_numDFCActiveAgents*(vEnviroment->_vProliferation[vEnviroment->_posProliferation]));

		vEnviroment->_StepsProliferationCurrent = vEnviroment->_stepsProliferacion;
	}

	bool dummyLimit = true;
	int  intentos = 0;
	point_3D miPosicion;
	register int largo = (int) vEnviroment->_maxInitDFC.x - (int) vEnviroment->_minInitDFC.x;
	register float distance;

	if(deadTime > 200)
	{
		deadTime = 0;
	}
	if((deadTime == 0) &&(_numDFCActiveAgents < vEnviroment->_numDFCActiveAgentsWanted) && dummyLimit)
	{
		int limiteIzquierdo =   100000;//vEnviroment->_minInitDFC.x + (rand()%(int)(largo));
		int limiteDerecho   =   -100000;//vEnviroment->_maxInitDFC.x - _agentsWM_DFC[0].GetRad(0);

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentsWM_DFC[k].GetActive())
			{
				if(_agentsWM_DFC[k].GetPosX() < limiteIzquierdo)
				{
					limiteIzquierdo = (int) _agentsWM_DFC[k].GetPosX();
					if(limiteIzquierdo < vEnviroment->_minEnviroment.x + _agentsWM_DFC[k].GetRad(0))
					{
						limiteIzquierdo = (int) vEnviroment->_minEnviroment.x + (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
				if(_agentsWM_DFC[k].GetPosX()> limiteDerecho)
				{
					limiteDerecho = (int) _agentsWM_DFC[k].GetPosX();
					if(limiteDerecho > vEnviroment->_maxEnviroment.x - _agentsWM_DFC[k].GetRad(0))
					{
						limiteDerecho = (int) vEnviroment->_maxEnviroment.x - (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
			}
		}
		float dimmm = 0.15f*(limiteDerecho-limiteIzquierdo);
		limiteIzquierdo = limiteIzquierdo +  (int) dimmm  ;//(rand()%(int)dimmm);
		limiteDerecho = limiteDerecho     -  (int) dimmm  ;//(rand()%(int)dimmm);

		bool soyValido = true;
		bool entreaqui = false;
		Zero(miPosicion);

		int inicioY =   100000;//vEnviroment->_minInitDFC.x + (rand()%(int)(largo));
		int finY   =   -100000;//vEnviroment->_maxInitDFC.x - _agentsWM_DFC[0].GetRad(0);

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentsWM_DFC[k].GetActive())
			{
				if(_agentsWM_DFC[k].GetPosY()< inicioY)
				{
					inicioY = (int) _agentsWM_DFC[k].GetPosY();
					if(inicioY < vEnviroment->_posMarginDEB.y + _agentsWM_DFC[k].GetRad(0))
					{
						inicioY = (int) vEnviroment->_posMarginDEB.y + (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
				if(_agentsWM_DFC[k].GetPosY() > finY)
				{
					finY = (int) _agentsWM_DFC[k].GetPosY();
					if(finY > vEnviroment->_dataEVL_PosY - _agentsWM_DFC[k].GetRad(0))
					{
						finY = (int) vEnviroment->_dataEVL_PosY - (int) _agentsWM_DFC[k].GetRad(0);
					}
				}
			}
		}
		finY = finY - (int) (0.25f*(finY-inicioY));
		//inicioY = inicioY + (rand()%(int)((finY-inicioY)/2));
		for(register int i = limiteIzquierdo ; i < limiteDerecho; i++)
		{
			for(register int j = inicioY; j < finY; j++)
			{
				soyValido = true;
				miPosicion.x = float(i);
				miPosicion.y = float(j);
				miPosicion.z = 0.0f;
				for(register int k = 0; k < _numDFCActiveAgents; k++)
				{
					if(_agentsWM_DFC[k].GetActive())
					{
						entreaqui = true;
						point_3D p3D_1;
						SetFull(p3D_1,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
						distance = Distance(miPosicion,p3D_1);
						if(distance <= 2.0f*_agentsWM_DFC[k].GetRad(0))
						{
							soyValido = false;
						}
					}
				}
				if(soyValido)
				{
					i = 1000000;
					j = 1000000;
				}
			}
		}

		if(soyValido && entreaqui)
		{
			int minimaDistance = 10000;
			point_3D direccion;
			for(register int k = 0; k < _numDFCActiveAgents; k++)
			{
				if(_agentsWM_DFC[k].GetActive())
				{
					point_3D p3D_1;
					SetFull(p3D_1,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
					distance = Distance(miPosicion,p3D_1);
					if(distance < minimaDistance)
					{
						minimaDistance = (int) distance;

						point_3D p3D_2;
						SetFull(p3D_2,_agentsWM_DFC[k].GetPosX(),_agentsWM_DFC[k].GetPosY(),0.0);
						Resta(direccion,p3D_2,miPosicion);
						NormalizeR(direccion);
					}
				}
			}

			if((minimaDistance != 10000) && (minimaDistance > 2*_agentsWM_DFC[0].GetRad(0)))
			{
				distance = distance - 2*_agentsWM_DFC[0].GetRad(0);
				Amplificar(direccion,distance,direccion);
				//Suma(miPosicion,miPosicion,direccion);
			}
			
			point_2D p2D_1;
			p2D_1.x(miPosicion.x);
			p2D_1.y(miPosicion.y);

			_agentsWM_DFC[_numDFCActiveAgents].InitCell(p2D_1,10);
			_agentsWM_DFC[_numDFCActiveAgents]._hija					= true;
			_agentsWM_DFC[_numDFCActiveAgents]._perturbacionAzar		= _agentsWM_DFC[0]._perturbacionAzar;
			_agentsWM_DFC[_numDFCActiveAgents]._friendAttraction		= _agentsWM_DFC[0]._friendAttraction;
			_agentsWM_DFC[_numDFCActiveAgents]._liberarEstancado		= _agentsWM_DFC[0]._liberarEstancado;
			_agentsWM_DFC[_numDFCActiveAgents]._adhesionToEVL			= false;
			_agentsWM_DFC[_numDFCActiveAgents]._ratioFriendAttraction	= _agentsWM_DFC[0]._ratioFriendAttraction;

			_numDFCActiveAgents++;
		}
		intentos++;
		if(intentos >= 1)
		{
			dummyLimit = false;
		}
	}
	vEnviroment->_StepsProliferationCurrent--;
};