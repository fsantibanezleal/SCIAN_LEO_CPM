#pragma once

#include "Agente.h"

class Agente_DFC : public Agente
{
private:
public:
	void		ReglasAmbiente(Ambiente vEnviroment);
public:
	Agente_DFC(void);
	~Agente_DFC(void);

	void		Inicializar(void); // azar
	void		Inicializar(point_3D vPosicion, float vRadio); // sete0
	void		InteraccionEVL(Ambiente* vEnviroment, int vNumAgentes, Agente* vMatrix);
	void		UpdateEstado(Ambiente* vEnviroment, Agente* vMatrix, int numAgentes);
};
