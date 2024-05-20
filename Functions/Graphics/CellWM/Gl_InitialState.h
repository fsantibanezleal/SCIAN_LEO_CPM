#pragma once
#include "OGL.h"
#include "BasicFunctions.h"

#include "Enviroment.h"
#include "AgentsSystem.h"

class Gl_InitialState : public OGL
{
private:

public:
	Gl_InitialState(void);
	~Gl_InitialState(void);
	void Resize(unsigned vDimVentanaX, unsigned vDimVentanaY);
	//void Renderizar(Enviroment* vEnviroment, AgentsSystem* vMatrix);
	void Renderizar(Enviroment* vEnviroment, AgentsSystem* vMatrix);
	void DrawPoly(point_3D vCentro,float vRadio,polygon_2D _polyMembrane,float vPasos, int vTipo, int vSizePoint,float factorX,float factorY,float deltaX,float deltaY);

};
