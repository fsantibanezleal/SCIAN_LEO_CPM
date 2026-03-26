#include "Agente.h"

Agente::Agente()
{
	_invalido		  = false;
	_adhesionToEVL	  = false;
	_perturbacionAzar = false;
	_liberarEstancado = false;
	_friendAttraction = false;
	_StepsforChange   = 10;
	_ratioFriendAttraction = 1.0f;

	_persistence	  = rand()%(_StepsforChange) + 1;
};
Agente::~Agente()
{

};

void    Agente::DireccionAzarosa(void)
{
	int numVars = 8;
	int miDireccion;
	float x,y,z;

	x = 0.0f;
	y = 0.0f; 
	z = 0.0f;

	miDireccion = rand()%(numVars);
	if(miDireccion == 0)
	{
		x = 1.0f;
	}
	if(miDireccion == 1)
	{
		x = 1.0f;
		y = 1.0f;
	}
	if(miDireccion == 2)
	{
		y = 1.0f;
	}
	if(miDireccion == 3)
	{
		y = 1.0f;
		x = -1.0f;
	}
	if(miDireccion == 4)
	{
		x = -1.0f;
	}
	if(miDireccion == 5)
	{
		x = -1.0f;
		y = -1.0f;
	}
	if(miDireccion == 6)
	{
		y = -1.0f;
	}
	if(miDireccion == 7)
	{
		x = 1.0f;
		y = -1.0f;
	}

	SetFull(_velocidad,x,y,z);
	NormalizeR(_velocidad);
};

void	Agente::ReglasAmbiente(Ambiente vEnviroment)
{

};

void	Agente::Inicializar(void)
{

}; // azar

void	Agente::Inicializar(point_3D vPosicion, float vRadio)
{

}; // sete0

void	Agente::UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix)
{
};

void	Agente::UpdateEstadoBase(void)
{
	Igualar(_posicion_last,_posicion);
	Igualar(_velocidad_last,_velocidad);
	Igualar(_aceleracion_last,_aceleracion);
};
