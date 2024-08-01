#pragma once

#include "Agente.h"
#include "Ambiente.h"

class Agente_DEB : public Agente
{
private:
public:
	void		ReglasAmbiente(Ambiente vEnviroment);
public:
	Agente_DEB(void);
	~Agente_DEB(void);

	void		Inicializar(void); // azar
	void		Inicializar(point_3D vPosicion, float vRadio); // sete0
	void		UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix);
};
