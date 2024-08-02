#pragma once
#include "OGL.h"

#include "Sistema_Agentes.h"
#include "Ambiente.h"

class GL_Simulacion  : public OGL
{
private:

public:
	int   _contador,_stepsContador;
	float _vectorParam1[10000000];
	float _numVectorParam1,_maxVectorParam1;

	float _vectorParam2[10000000];
	float _numVectorParam2,_maxVectorParam2;

	float _vectorParam3[10000000];
	float _numVectorParam3,_maxVectorParam3;

	float _vectorParam4[10000000];
	float _numVectorParam4,_maxVectorParam4;

	float _vectorParam5[10000000];
	float _numVectorParam5,_maxVectorParam5;

	float _vectorParam6[10000000];
	float _numVectorParam6,_maxVectorParam6;

	float _vectorParam7[10000000];
	float _numVectorParam7,_maxVectorParam7;

	float _vectorParam8[10000000];
	float _numVectorParam8,_maxVectorParam8;

	float _vectorParam9[10000000];
	float _numVectorParam9,_maxVectorParam9;

	GL_Simulacion (void);
	~GL_Simulacion (void);
	void Resize(unsigned vDimVentanaX, unsigned vDimVentanaY);
	void Renderizar(bool vEstimarTriangulo, Ambiente* vEnviroment, Sistema_Agentes* vMatrix);
};
