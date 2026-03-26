#include "Agente_DFC.h"

Agente_DFC::Agente_DFC()
{
	_fRunning = 0;
	_tipoAgente = 0;
	_radio = 0;
	SetFull(_posicion,0,0,0);
	SetFull(_color,0,255,0);

	_fActive = 1;
};
Agente_DFC::~Agente_DFC()
{

};

void	Agente_DFC::ReglasAmbiente(Ambiente vEnviroment)
{

};

void	Agente_DFC::Inicializar(void)
{

}; // azar

void	Agente_DFC::Inicializar(point_3D vPosicion, float vRadio)
{
	_hija			  = false;
	_fActive			  = true;
	_adhesionToEVL	  = false;

	_persistence	  = rand()%(_StepsforChange) + 1;
	DireccionAzarosa();
	Igualar(_posicion,vPosicion);
	_radio = vRadio;
}; // sete0

void	Agente_DFC::UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix, int numAgentes)
{
	UpdateEstadoBase();
	// Perturbacion browniana
	if(_perturbacionAzar)
	{
		_persistence--;
		if(_persistence < 1)
		{
			_persistence = _StepsforChange;//(rand()%_StepsforChange) + 1;
			DireccionAzarosa();
		}
	}

	if(vEnviroment->_useDEBAttraction || vEnviroment->_useEVLInteraction || _friendAttraction)
	{
		Zero(_velocidad);
	}
	// Efecto ambiente
	if(vEnviroment->_useDEBAttraction && !(vEnviroment->_useEVLInteractionAdhesion))
	{
		_velocidad.y = - 1;
		float deltaX = (( float(rand()%100) + 1.0f)/100.0f) - 0.5f;
		deltaX = (fabs(deltaX) < 0.4)? 0.0f: deltaX;
		deltaX *= 0.5;
		_velocidad.x = deltaX;
	}

	// Efecto otros agentes
	InteraccionEVL(vEnviroment,numAgentes,vMatrix);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Efecto de los agentes
	// centroide de agentes..

	if(_friendAttraction && (!(vEnviroment->_useEVLInteraction) || (vEnviroment->_useEVLInteraction && !_adhesionToEVL)))
	{
		point_3D centroide ;
		Zero(centroide);
		register float distancia;
		register int numCells = 0;
		for(register int i = 0; i < numAgentes; i++)
		{
			distancia = Distancia(vMatrix[i]._posicion,_posicion) - vMatrix[i]._radio;
			if( (distancia > _radio) && (distancia <= _radio*_ratioFriendAttraction))
			{
				if(vMatrix[i]._tipoAgente == 0) // Otras DFCs
				{
					Suma(centroide,centroide,vMatrix[i]._posicion);
					numCells++;
				}
			}
		}

		float deltaX = (( float(rand()%100) + 1.0f)/100.0f) - 0.5f;
		deltaX = (fabs(deltaX) < 0.4)? 0.0f: deltaX;
		float deltaY = (( float(rand()%100) + 1.0f)/100.0f) - 0.5f;
		deltaY = (fabs(deltaY) < 0.4)? 0.0f: deltaY;

		if(numCells > 0)
		{
			Amplificar(centroide,1.0f/numCells);
			Resta(centroide,centroide,_posicion);
			NormalizeR(centroide);
			Amplificar(centroide,vEnviroment->_factorDFC_DEB_Action);

			centroide.x = centroide.x + deltaX;
			centroide.y = centroide.y + deltaY;
			Suma(_velocidad,_velocidad,centroide);
		}
		else
		{
			SetFull(_velocidad,deltaX,deltaY,0.0f);
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ver si me puedo mover :D
	point_3D miMovimientoDeseado;
	Zero(miMovimientoDeseado);

	if(_velocidad.x > 0)
	{
		miMovimientoDeseado.x = 1;
	}
	if(_velocidad.x < 0)
	{
		miMovimientoDeseado.x = -1;
	}

	if(_velocidad.y > 0)
	{
		miMovimientoDeseado.y = 1;
	}
	if(_velocidad.y < 0)
	{
		miMovimientoDeseado.y = -1;
	}

	Igualar(_velocidad,miMovimientoDeseado);

	bool freeD, freeX, freeY;
	freeD = true;
	freeX = true;
	freeY = true;

	point_3D nuevaPosicionDeseada;
	
	// Verificar no chocar con vecinos
	for(register int i = 0; i < numAgentes; i++)
	{
		if(Distancia(vMatrix[i]._posicion,_posicion) > 1)
		{
			if(vMatrix[i]._tipoAgente == 0) // Otras DFCs
			{
				if(freeD)
				{
					Igualar(nuevaPosicionDeseada,_posicion);
					nuevaPosicionDeseada.x = nuevaPosicionDeseada.x + miMovimientoDeseado.x;
					nuevaPosicionDeseada.y = nuevaPosicionDeseada.y + miMovimientoDeseado.y;
					if(LengthVector(Resta(vMatrix[i]._posicion,nuevaPosicionDeseada)) < (_radio + vMatrix[i]._radio))
					{
						freeD = false;
					}
				}
				if(!freeD && freeX)
				{
					Igualar(nuevaPosicionDeseada,_posicion);
					nuevaPosicionDeseada.x = nuevaPosicionDeseada.x + miMovimientoDeseado.x;
					if(LengthVector(Resta(vMatrix[i]._posicion,nuevaPosicionDeseada)) < (_radio + vMatrix[i]._radio))
					{
						freeX = false;
					}
					if(_adhesionToEVL)
					{
						//freeY = true;
					}
				}
				if(!freeD && freeY)
				{
					Igualar(nuevaPosicionDeseada,_posicion);
					nuevaPosicionDeseada.y = nuevaPosicionDeseada.y + miMovimientoDeseado.y;
					//if(!_adhesionToEVL && 
					if(LengthVector(Resta(vMatrix[i]._posicion,nuevaPosicionDeseada)) < (_radio + vMatrix[i]._radio))
					{
						freeY = false;
					}
				}
			}
			if(vMatrix[i]._tipoAgente == 1)
			{
			
			}
			if(vMatrix[i]._tipoAgente == 2)
			{
			
			}
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Verificar no salir de la grilla y mover
	if(freeD)
	{
		Suma(nuevaPosicionDeseada,_posicion,miMovimientoDeseado);
		if( (nuevaPosicionDeseada.y >= (vEnviroment->_posMarginDEB.y + _radio) ) && (nuevaPosicionDeseada.y <= (vEnviroment->_maxEnviroment.y - _radio) ) )
		{
			_posicion.y = _posicion.y + miMovimientoDeseado.y;
		}
		else
		{
			if((nuevaPosicionDeseada.y > (vEnviroment->_maxEnviroment.y - _radio) ))
			{
				_velocidad.y  = -_velocidad.y;
			}

			if(nuevaPosicionDeseada.y < (vEnviroment->_posMarginDEB.y + _radio) )
			{
				if(vEnviroment->_useDEBAttraction)
				{
					_velocidad.x  = 0.0f;	
				}
				else
				{
					_velocidad.y  = -_velocidad.y;
				}
			}
		}

		if( (nuevaPosicionDeseada.x >= (vEnviroment->_posMarginDEB.x + _radio) ) && (nuevaPosicionDeseada.x <= (vEnviroment->_maxEnviroment.x - _radio) ) )
		{
			if(_velocidad.x != 0.0f)
			{
				_posicion.x  = _posicion.x + miMovimientoDeseado.x;
			}
		}
		else
		{
			_velocidad.x  = -_velocidad.x;
		}

		if(!freeX || !freeY)
		{
			freeD = false;
		}
	}
	else if(freeX)
	{
		Suma(nuevaPosicionDeseada,_posicion,miMovimientoDeseado);
		if( (nuevaPosicionDeseada.x >= (vEnviroment->_posMarginDEB.x + _radio) ) && (nuevaPosicionDeseada.x <= (vEnviroment->_maxEnviroment.x - _radio) ) )
		{
			_posicion.x = _posicion.x + miMovimientoDeseado.x;
		}
		else
		{
			//freeX = false;
			_velocidad.x  = -_velocidad.x;
		}
	}
	else if(freeY)
	{
		Suma(nuevaPosicionDeseada,_posicion,miMovimientoDeseado);
		if( (nuevaPosicionDeseada.y >= (vEnviroment->_posMarginDEB.y + _radio) ) && (nuevaPosicionDeseada.y <= (vEnviroment->_maxEnviroment.y - _radio) ) )
		{
			if(!vEnviroment->_useEVLInteraction || (vEnviroment->_useEVLInteraction && (_posicion.y + miMovimientoDeseado.y < vEnviroment->_dataEVL_PosY)))
			{	
				_posicion.y = _posicion.y + miMovimientoDeseado.y;
			}
		}
		else if(nuevaPosicionDeseada.y <= (vEnviroment->_maxEnviroment.y - _radio) )
		{
			//freeY = false;
			if(!vEnviroment->_useDEBAttraction)
			{
				_velocidad.y  = -_velocidad.y;
			}
			else
			{
				_velocidad.x  = 0;
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// SI ya no hay movimiento valido.... asignar aleatorio
	if(!freeD && (!freeX || (miMovimientoDeseado.x == 0)) && (!freeY || (miMovimientoDeseado.y == 0)))
	{
		if(_liberarEstancado)
		{
			DireccionAzarosa();
		}
	}
};

void	Agente_DFC::InteraccionEVL(Ambiente* vEnviroment, int vNumAgentes, Agente* vMatrix)
{
	if(vEnviroment->_useEVLInteraction)
	{
		point_3D centroide;
		Zero(centroide);
		//!(vEnviroment->_useEVLInteractionAdhesion) || 
		if((vEnviroment->_dataEVL_tipoCantidad== 0) || ((vEnviroment->_dataEVL_IncludedCampos == 0 || vEnviroment->_dataEVL_IncludedCampos == 2) && _adhesionToEVL))
		{
			if(vEnviroment->_dataEVL_tipoCampo == 0)
			{
				centroide.y = (float) vEnviroment->_dataEVL_PosY;
				centroide.x = (float) (vEnviroment->_dimX)/2.0f;
				Resta(centroide,centroide,_posicion);
				NormalizeR(centroide);
				Amplificar(centroide, vEnviroment->_dataEVL_FactorEVL);
				//if(vEnviroment->_dataEVL_tipoCantidad == 0)
				{
					Suma(_velocidad,_velocidad,centroide);
				}
				//else
				{
				//	Igualar(_velocidad,centroide);
				}
			}
			if(vEnviroment->_dataEVL_tipoCampo == 1)
			{
				centroide.y = (float) vEnviroment->_dataEVL_PosY;
				centroide.x = (float) (vEnviroment->_dimX)/2.0f;

				float distancia = _posicion.x - centroide.x;

				if(distancia == 0.0f)
				{
				}
				if(distancia > 0.0f)
				{
					if(fabs(distancia) < (vEnviroment->_dataEVL_AnchoLinea/2.0f))
					{
						centroide.x = centroide.x + distancia; 
					}
					else
					{
						centroide.x = centroide.x + (vEnviroment->_dataEVL_AnchoLinea/2.0f); 
					}
				}
				if(distancia < 0.0f)
				{
					if(fabs(distancia) < (vEnviroment->_dataEVL_AnchoLinea/2.0f))
					{
						centroide.x = centroide.x - distancia; 
					}
					else
					{
						centroide.x = centroide.x - (vEnviroment->_dataEVL_AnchoLinea/2.0f); 
					}
				}
				Resta(centroide,centroide,_posicion);
				NormalizeR(centroide);
				Amplificar(centroide, vEnviroment->_dataEVL_FactorEVL);

				//if(vEnviroment->_dataEVL_tipoCantidad == 0)
				{
					Suma(_velocidad,_velocidad,centroide);
				}
				//else
				{
				//	Igualar(_velocidad,centroide);
				}

				/// Disminucion Linea...
			}
			if(vEnviroment->_dataEVL_tipoCampo == 2)
			{
			}		
		}
		else if(vEnviroment->_dataEVL_IncludedCampos == 1 || vEnviroment->_dataEVL_IncludedCampos == 2)
		{ /// Verificar si soy nodo del final
			bool losoy = true;
			for(register int i = 0; i < vNumAgentes; i++)
			{
				if(Distancia(vMatrix[i]._posicion,_posicion) > 1)
				{
					if(vMatrix[i]._tipoAgente == 0) // Otras DFCs
					{
						if( fabs(_posicion.x - vMatrix[i]._posicion.x) < (vMatrix[i]._radio+ _radio))
						{
							if(_posicion.y > vMatrix[i]._posicion.y)
							{
							}
							else
							{
								losoy = false;
							}
						}
					}
				}
			}
			
			if(losoy)
			{
				losoy = false;
				if(vEnviroment->_dataEVL_tipoCampo == 0)
				{
					if(fabs(_posicion.x- float(vEnviroment->_dimX)/2.0f) < 1.5f* _radio)
					{
						losoy = true;	
					}
				}
				if(vEnviroment->_dataEVL_tipoCampo == 1)
				{
					if(fabs(_posicion.x- float(vEnviroment->_dimX)/2.0f) < (_radio + (vEnviroment->_dataEVL_AnchoLinea/2.0f)))
					{
						losoy = true;	
					}
				}
			}

			if(losoy && _adhesionToEVL)
			{

				if(vEnviroment->_dataEVL_tipoCampo == 0)
				{
					centroide.y = (float) vEnviroment->_dataEVL_PosY;
					centroide.x = (float) (vEnviroment->_dimX)/2.0f;
					Resta(centroide,centroide,_posicion);
					NormalizeR(centroide);
					Amplificar(centroide, vEnviroment->_dataEVL_FactorEVL);
					//if(vEnviroment->_dataEVL_tipoCantidad == 0)
					{
						Suma(_velocidad,_velocidad,centroide);
					}
					//else
					{
					//	Igualar(_velocidad,centroide);
					}
				}
				if(vEnviroment->_dataEVL_tipoCampo == 1)
				{
					centroide.y = (float) vEnviroment->_dataEVL_PosY;
					centroide.x = (float) (vEnviroment->_dimX)/2.0f;

					float distancia = _posicion.x - centroide.x;

					if(distancia == 0.0f)
					{
					}
					if(distancia > 0.0f)
					{
						if(fabs(distancia) < (vEnviroment->_dataEVL_AnchoLinea/2.0f))
						{
							centroide.x = centroide.x + distancia; 
						}
						else
						{
							centroide.x = centroide.x + (vEnviroment->_dataEVL_AnchoLinea/2.0f); 
						}
					}
					if(distancia < 0.0f)
					{
						if(fabs(distancia) < (vEnviroment->_dataEVL_AnchoLinea/2.0f))
						{
							centroide.x = centroide.x - distancia; 
						}
						else
						{
							centroide.x = centroide.x - (vEnviroment->_dataEVL_AnchoLinea/2.0f); 
						}
					}
					Resta(centroide,centroide,_posicion);
					NormalizeR(centroide);
					Amplificar(centroide, vEnviroment->_dataEVL_FactorEVL);

					//if(vEnviroment->_dataEVL_tipoCantidad == 0)
					{
						Suma(_velocidad,_velocidad,centroide);
					}
					//else
					{
						Igualar(_velocidad,centroide);
					}

					/// Disminucion Linea...
				}
				if(vEnviroment->_dataEVL_tipoCampo == 2)
				{
				}		



			}
		}
		float deltaX = (( float(rand()%100) + 1.0f)/100.0f) - 0.5f;
		deltaX = (fabs(deltaX) < 0.4)? 0.0f: deltaX;
		float deltaY = (( float(rand()%100) + 1.0f)/100.0f) - 0.5f;
		deltaY = (fabs(deltaY) < 0.4)? 0.0f: deltaY;
		deltaX *= 0.5;
		deltaY *= 0.5;
		_velocidad.x = _velocidad.x + deltaX;
		_velocidad.y = _velocidad.y + deltaY;
	}
};