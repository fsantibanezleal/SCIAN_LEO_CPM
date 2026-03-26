#include "Sistema_Agentes.h"
#include <math.h>

Sistema_Agentes::Sistema_Agentes()
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
	_posicionPorAttActual = 0;

	_fRunning    = 0;
	_numEVLAgents = 0;
	_numDFCActiveAgents = 0;
	_numDEBAgents = 0;
	
	srand((unsigned)time(NULL));
	_agentesDFC = new Agente_DFC[_numDFCActiveAgents];
};
Sistema_Agentes::~Sistema_Agentes()
{

};
void	Sistema_Agentes::Inicializar(void)
{

}; // azar
void	Sistema_Agentes::Inicializar(Ambiente* vEnviroment, int vNumDFCs, int vNumDEBs, int vNumEVLs)
{
	vEnviroment->_numDFCActiveAgentsWanted = vNumDFCs;
	_numDFCAgents	= 1000;
	_numDFCActiveAgents	= 0;
	_numDEBAgents	= vNumDEBs;
	_numEVLAgents	= vNumEVLs;

	//_agentesDFC = new Agente_DFC[_numDFCAgents];
	_agentesDFC = new Agente_DFC[_numDFCAgents];
	//_agentesDFC = new CellWM[_numDFCAgents];

	for(register int i = 0; i < _numDFCAgents; i++)
	{
		//_agentesDFC[i]._fActive = false;
	}

	_agentesDEB = new Agente_DEB[_numDEBAgents];
	_agentesEVL = new Agente_EVL[_numEVLAgents];

	_posicionPorAttActual = 0;
	vEnviroment->_num_DFC_EVL_AdhesionAnterior = 0;
	vEnviroment->_num_DFC_EVL_AdhesionPosterior = 0;
	vEnviroment->_dataEVL_actualStepDistEVL = 0;
	// Using boundaries for DFC
	point_3D inicioDFC,finDFC,actualPOS;
	Igualar(inicioDFC,vEnviroment->_minInitDFC);
	//inicioDFC.y = inicioDFC.y + 0.05f*float(vEnviroment->_dimY);
	Igualar(finDFC,vEnviroment->_maxInitDFC);
	Igualar(actualPOS,inicioDFC);
	float radio = 10;

	actualPOS.x = inicioDFC.x - radio;
	actualPOS.y = inicioDFC.y + radio;
	float yMAX = 0.0f;
	for(register int i = 0; i < vEnviroment->_numDFCActiveAgentsWanted; i++)
	{
		actualPOS.x = actualPOS.x + 2*radio;
		
		if(actualPOS.x > finDFC.x)
		{
			actualPOS.x = inicioDFC.x + radio;
			actualPOS.y = actualPOS.y + 2*radio;
			_agentesDFC[i].Inicializar(actualPOS, radio);

			_numDFCActiveAgents++;
		}
		else
		{
			_agentesDFC[i].Inicializar(actualPOS, radio);
			_numDFCActiveAgents++;
		}
		if(actualPOS.y > yMAX)
		{
			yMAX = actualPOS.y;
		}
	}

	float minX = 10000.0f;
	float maxX = -10000.0f;

	for(register int i = 0; i < _numDFCActiveAgents; i++)
	{
		if(_agentesDFC[i]._fActive && (_agentesDFC[i]._posicion.y == yMAX))
		{

			if((_agentesDFC[i]._posicion.x + _agentesDFC[i]._radio)> maxX)
			{
				maxX = (_agentesDFC[i]._posicion.x + _agentesDFC[i]._radio);
			}
			if((_agentesDFC[i]._posicion.x - _agentesDFC[i]._radio) < minX)
			{
				minX = (_agentesDFC[i]._posicion.x - _agentesDFC[i]._radio);
			}
		}
	}
	if( fabs(maxX - minX) < (vEnviroment->_maxInitDFC.x - vEnviroment->_minInitDFC.x))
	{
		float register delta = ((vEnviroment->_maxInitDFC.x - vEnviroment->_minInitDFC.x) - fabs(maxX - minX))/ 2.0f;
		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentesDFC[i]._fActive && (_agentesDFC[i]._posicion.y == yMAX))
			{
				_agentesDFC[i]._posicion.x = _agentesDFC[i]._posicion.x + delta;
			}
		}
	}
}; // sete0
void	Sistema_Agentes::UpdateEstado(Ambiente* vEnviroment)
{
	if(_fRunning)
	{
		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentesDFC[i]._fActive)
			{
				_agentesDFC[i].UpdateEstado(vEnviroment,_agentesDFC,_numDFCActiveAgents);
			}
		}

		if(vEnviroment->_useEVLInteraction)
		{
			if( (vEnviroment->_dataEVL_tipoCampo == 1) && (vEnviroment->_dataEVL_AnchoLinea > 20)  && (vEnviroment->_dataEVL_actualStepsLinea < 1))
			{
				vEnviroment->_dataEVL_AnchoLinea =vEnviroment->_dataEVL_AnchoLinea--;
				if(vEnviroment->_dataEVL_AnchoLinea < 20)
				{
					vEnviroment->_dataEVL_AnchoLinea = 20;
				}
				vEnviroment->_dataEVL_actualStepsLinea = vEnviroment->_dataEVL_StepsLinea;
			}
			vEnviroment->_dataEVL_actualStepsLinea--;
			if(vEnviroment->_dataEVL_tipoCantidad == 1)
			{
				if(vEnviroment->_dataEVL_actualStepDistEVL < 1 && _posicionPorAttActual <= 8)
				{
					int misericordia;
					bool usingMisericordia;

					float minPosY,maxPosY,posYMedia = 0.0f;
					minPosY = 100000.0f;
					maxPosY = -100000.0f;

					int numAnterior = 0;
					int numPosterior = 0;
					for(register int i = 0; i < _numDFCActiveAgents; i++)
					{
						if(_agentesDFC[i]._fActive)
						{
							if(_agentesDFC[i]._posicion.y <= minPosY)
							{
								minPosY = _agentesDFC[i]._posicion.y;
							}
							if(_agentesDFC[i]._posicion.y >= maxPosY)
							{
								maxPosY = _agentesDFC[i]._posicion.y;
							}
						}
					}
					posYMedia = 0.5f*(minPosY + maxPosY);
					for(register int i = 0; i < _numDFCActiveAgents; i++)
					{
						if(_agentesDFC[i]._fActive)
						{
							if(_agentesDFC[i]._posicion.y <= posYMedia)
							{
								numAnterior++;
							}
							if(_agentesDFC[i]._posicion.y > posYMedia)
							{
								numPosterior++;
							}
						}
					}


					vEnviroment->_limite_DFC_EVL_AdhesionAnterior =  (int) ceil((float)numAnterior*_porcentajeAttachmentAnterior[_posicionPorAttActual]);
					vEnviroment->_limite_DFC_EVL_AdhesionPosterior = (int) ceil((float)numPosterior*_porcentajeAttachmentPosterior[_posicionPorAttActual]);

					if(vEnviroment->_num_DFC_EVL_AdhesionAnterior <= vEnviroment->_limite_DFC_EVL_AdhesionAnterior)
					{
						/// Asignar Anterior
						misericordia = 0;

						usingMisericordia= true;
						while(usingMisericordia && (vEnviroment->_num_DFC_EVL_AdhesionAnterior < vEnviroment->_limite_DFC_EVL_AdhesionAnterior))
						{
							for(register int i = 0; i < _numDFCActiveAgents; i++)
							{
								if(_agentesDFC[i]._fActive)
								{
									if(!_agentesDFC[i]._adhesionToEVL && (_agentesDFC[i]._posicion.y <= posYMedia))
									{
										if(((rand()%(int)(2)) == 0))
										{
											_agentesDFC[i]._adhesionToEVL = true;
											vEnviroment->_num_DFC_EVL_AdhesionAnterior++;
										}
										if(vEnviroment->_num_DFC_EVL_AdhesionAnterior >= vEnviroment->_limite_DFC_EVL_AdhesionAnterior)
										{
											i = _numDFCActiveAgents;
										}
									}
								}
							}
							if(misericordia >= 10000)
							{
								usingMisericordia = false;
							}
							misericordia++;
						}
					}
					else
					{
						for(register int i = 0; i < _numDFCActiveAgents; i++)
						{
							if((_agentesDFC[i]._fActive) && _agentesDFC[i]._adhesionToEVL)
							{
								_agentesDFC[i]._adhesionToEVL = false;
								vEnviroment->_num_DFC_EVL_AdhesionAnterior--;
								if(vEnviroment->_num_DFC_EVL_AdhesionAnterior <= vEnviroment->_limite_DFC_EVL_AdhesionAnterior)
								{
									i = _numDFCActiveAgents +2;
								}
							}
						}
					}

					if(vEnviroment->_num_DFC_EVL_AdhesionPosterior <= vEnviroment->_limite_DFC_EVL_AdhesionPosterior)
					{
						////// el otro
						misericordia = 0;
						usingMisericordia= true;
						while(usingMisericordia && (vEnviroment->_num_DFC_EVL_AdhesionPosterior < vEnviroment->_limite_DFC_EVL_AdhesionPosterior))
						{
							for(register int i = 0; i < _numDFCActiveAgents; i++)
							{
								if(_agentesDFC[i]._fActive && (!_agentesDFC[i]._adhesionToEVL && (_agentesDFC[i]._posicion.y > posYMedia)))
								{
									if(((rand()%(int)(2)) == 0))
									{
										_agentesDFC[i]._adhesionToEVL = true;
										vEnviroment->_num_DFC_EVL_AdhesionPosterior++;
									}
									if(vEnviroment->_num_DFC_EVL_AdhesionPosterior >= vEnviroment->_limite_DFC_EVL_AdhesionPosterior)
									{
										i = _numDFCActiveAgents;
									}
								}
							}
							if(misericordia >= 10000)
							{
								usingMisericordia = false;
							}
							misericordia++;
						}
					}
					else
					{
						for(register int i = 0; i < _numDFCActiveAgents; i++)
						{
							if(_agentesDFC[i]._fActive && (_agentesDFC[i]._adhesionToEVL))
							{
								_agentesDFC[i]._adhesionToEVL = false;
								vEnviroment->_num_DFC_EVL_AdhesionPosterior--;
								if(vEnviroment->_num_DFC_EVL_AdhesionPosterior <= vEnviroment->_limite_DFC_EVL_AdhesionPosterior)
								{
									i = _numDFCActiveAgents +2;
								}
							}
						}
					}

					
					vEnviroment->_dataEVL_actualStepDistEVL =  vEnviroment->_dataEVL_StepsCambioEVLDist;
					_posicionPorAttActual++;
					if(_posicionPorAttActual > 8) _posicionPorAttActual = 8;
				}
				
				vEnviroment->_dataEVL_actualStepDistEVL--;
			}
		}

		//////////// actualizar los parametros del ambiente,,, XD
		vEnviroment->Update();

		// verificar si el arrastre del ambiente afecto el sistema
		// primera iteracion para asignar los afectados por el borde superior
		register bool estadoInvalido = true;
		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentesDFC[i]._fActive && (_agentesDFC[i]._posicion.y < (vEnviroment->_posMarginDEB.y + _agentesDFC[i]._radio)))
			{
				_agentesDFC[i]._posicion.y++;
			}
		}
		////////////// arreglar -... hasta validar---
		register float distancia = 0.0f;
		while(estadoInvalido)
		{
			estadoInvalido = false;
			for(register int i = 0; i < _numDFCActiveAgents; i++)
			{
				if(_agentesDFC[i]._fActive)
				{
					for(register int j = 0; j < _numDFCActiveAgents; j++)
					{
						if(_agentesDFC[j]._fActive && (i != j))
						{
							distancia = Distancia(_agentesDFC[i]._posicion,_agentesDFC[j]._posicion);
							if(distancia < (_agentesDFC[i]._radio +  _agentesDFC[j]._radio))
							{
								estadoInvalido = true;
								if(_agentesDFC[i]._posicion.y >= _agentesDFC[j]._posicion.y)
								{
									_agentesDFC[i]._posicion.y++;
								}
								else
								{
									_agentesDFC[j]._posicion.y++;
								}
							}
						}
					}
				}
			}
		}
		//// Fin fixed....

		/// arreglar desde abajo

		for(register int i = 0; i < _numDFCActiveAgents; i++)
		{
			if(_agentesDFC[i]._fActive && (_agentesDFC[i]._posicion.y > (vEnviroment->_dataEVL_PosY)))
			{
				_agentesDFC[i]._posicion.y--;
			}
		}

		estadoInvalido = true;
		while(estadoInvalido)
		{
			estadoInvalido = false;
			for(register int i = 0; i < _numDFCActiveAgents; i++)
			{
				if(_agentesDFC[i]._fActive)
				{
					for(register int j = 0; j < _numDFCActiveAgents; j++)
					{
						if(_agentesDFC[j]._fActive && (i != j))
						{
							distancia = Distancia(_agentesDFC[i]._posicion,_agentesDFC[j]._posicion);
							if(distancia < (_agentesDFC[i]._radio +  _agentesDFC[j]._radio))
							{
								estadoInvalido = true;
								if(_agentesDFC[i]._posicion.y <= _agentesDFC[j]._posicion.y)
								{
									_agentesDFC[i]._posicion.y--;
								}
								else
								{
									_agentesDFC[j]._posicion.y--;
								}
							}
						}
					}
				}
			}
		}

		AgregarHijos(vEnviroment);

	}
};
int tiempomuerto = 0;
void	Sistema_Agentes::AgregarHijos(Ambiente* vEnviroment)
{
	if(vEnviroment->_actualStepsProliferacion < 1 && vEnviroment->_posProliferation < 7)
	{
		vEnviroment->_posProliferation++;
		vEnviroment->_numDFCActiveAgentsWanted = (int) vEnviroment->_numDFCActiveAgentsWanted + (int) ceil(_numDFCActiveAgents*(vEnviroment->_vectorProliferacion[vEnviroment->_posProliferation]));

		vEnviroment->_actualStepsProliferacion = vEnviroment->_stepsProliferacion;
	}

	bool misericordia = true;
	int  intentos = 0;
	point_3D miPosicion;
	register int largo = (int) vEnviroment->_maxInitDFC.x - (int) vEnviroment->_minInitDFC.x;
	register float distancia;

	if(tiempomuerto > 200)
	{
		tiempomuerto = 0;
	}
	if((tiempomuerto == 0) &&(_numDFCActiveAgents < vEnviroment->_numDFCActiveAgentsWanted) && misericordia)
	{
		int limiteIzquierdo =   100000;//vEnviroment->_minInitDFC.x + (rand()%(int)(largo));
		int limiteDerecho   =   -100000;//vEnviroment->_maxInitDFC.x - _agentesDFC[0]._radio;

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentesDFC[k]._fActive)
			{
				if(_agentesDFC[k]._posicion.x < limiteIzquierdo)
				{
					limiteIzquierdo = (int) _agentesDFC[k]._posicion.x;
					if(limiteIzquierdo < vEnviroment->_minEnviroment.x + _agentesDFC[k]._radio)
					{
						limiteIzquierdo = (int) vEnviroment->_minEnviroment.x + (int) _agentesDFC[k]._radio;
					}
				}
				if(_agentesDFC[k]._posicion.x> limiteDerecho)
				{
					limiteDerecho = (int) _agentesDFC[k]._posicion.x;
					if(limiteDerecho > vEnviroment->_maxEnviroment.x - _agentesDFC[k]._radio)
					{
						limiteDerecho = (int) vEnviroment->_maxEnviroment.x - (int) _agentesDFC[k]._radio;
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
		int finY   =   -100000;//vEnviroment->_maxInitDFC.x - _agentesDFC[0]._radio;

		for(register int k = 0; k < _numDFCActiveAgents; k++)
		{
			if(_agentesDFC[k]._fActive)
			{
				if(_agentesDFC[k]._posicion.y< inicioY)
				{
					inicioY = (int) _agentesDFC[k]._posicion.y;
					if(inicioY < vEnviroment->_posMarginDEB.y + _agentesDFC[k]._radio)
					{
						inicioY = (int) vEnviroment->_posMarginDEB.y + (int) _agentesDFC[k]._radio;
					}
				}
				if(_agentesDFC[k]._posicion.y > finY)
				{
					finY = (int) _agentesDFC[k]._posicion.y;
					if(finY > vEnviroment->_dataEVL_PosY - _agentesDFC[k]._radio)
					{
						finY = (int) vEnviroment->_dataEVL_PosY - (int) _agentesDFC[k]._radio;
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
					if(_agentesDFC[k]._fActive)
					{
						entreaqui = true;
						distancia = Distancia(miPosicion,_agentesDFC[k]._posicion);
						if(distancia <= 2.0f*_agentesDFC[k]._radio)
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
			int minimaDistancia = 10000;
			point_3D direccion;
			for(register int k = 0; k < _numDFCActiveAgents; k++)
			{
				if(_agentesDFC[k]._fActive)
				{
					distancia = Distancia(miPosicion,_agentesDFC[k]._posicion);
					if(distancia < minimaDistancia)
					{
						minimaDistancia = (int) distancia;
						Resta(direccion,_agentesDFC[k]._posicion,miPosicion);
						NormalizeR(direccion);
					}
				}
			}

			if((minimaDistancia != 10000) && (minimaDistancia > 2*_agentesDFC[0]._radio))
			{
				distancia = distancia - 2*_agentesDFC[0]._radio;
				Amplificar(direccion,distancia,direccion);
				//Suma(miPosicion,miPosicion,direccion);
			}

			_agentesDFC[_numDFCActiveAgents].Inicializar(miPosicion,10);
			_agentesDFC[_numDFCActiveAgents]._hija = true;
			_agentesDFC[_numDFCActiveAgents]._perturbacionAzar = _agentesDFC[0]._perturbacionAzar;
			_agentesDFC[_numDFCActiveAgents]._friendAttraction = _agentesDFC[0]._friendAttraction;
			_agentesDFC[_numDFCActiveAgents]._liberarEstancado = _agentesDFC[0]._liberarEstancado;
			_agentesDFC[_numDFCActiveAgents]._adhesionToEVL = false;
			_agentesDFC[_numDFCActiveAgents]._ratioFriendAttraction =_agentesDFC[0]._ratioFriendAttraction;

			_numDFCActiveAgents++;
		}
		intentos++;
		if(intentos >= 1)
		{
			misericordia = false;
		}
	}
	vEnviroment->_actualStepsProliferacion--;
};