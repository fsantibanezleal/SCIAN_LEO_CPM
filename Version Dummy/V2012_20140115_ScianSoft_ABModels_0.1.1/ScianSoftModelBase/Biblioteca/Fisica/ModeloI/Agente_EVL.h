#pragma once

#include "Agente.h"

class Agente_EVL : public Agente
{
private:
public:
	void		ReglasAmbiente(Ambiente vEnviroment);
public:
	Agente_EVL(void);
	~Agente_EVL(void);

	void		Inicializar(void); // azar
	void		Inicializar(point_3D vPosicion, float vRadio); // sete0
	void		UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix);
};
