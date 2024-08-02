#pragma once
#include "OGL.h"
#include "BasicFunctions.h"

//#include "Ambiente.h"
//#include "Sistema_Agentes.h"

#include "Enviroment.h"
#include "AgentsSystem.h"

class GL_EstadoInicial : public OGL
{
private:

public:
	GL_EstadoInicial(void);
	~GL_EstadoInicial(void);
	void Resize(unsigned vDimVentanaX, unsigned vDimVentanaY);
	//void Renderizar(Ambiente* vEnviroment, Sistema_Agentes* vMatrix);
	void Renderizar(Enviroment* vEnviroment, AgentsSystem* vMatrix);
	void DrawPoly(point_3D vCentro,float vRadio,polygon_2D _polyMembrane,float vPasos, int vTipo, int vSizePoint,float factorX,float factorY,float deltaX,float deltaY);

};
