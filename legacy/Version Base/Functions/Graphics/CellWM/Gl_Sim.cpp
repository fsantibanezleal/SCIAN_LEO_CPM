#include "Gl_Sim.h"

Gl_Sim::Gl_Sim (void)
{
	_numVectorParam1 = -1.0f;
	_maxVectorParam1 = 0.0f;
	_contador = 0;
	_stepsContador = 5;

	_numVectorParam2 = -1.0f;
	_maxVectorParam2 = 0.0f;

	_numVectorParam3 = -1.0f;
	_maxVectorParam3 = 0.0f;

	_numVectorParam4 = -1.0f;
	_maxVectorParam4 = 0.0f;

	_numVectorParam5 = -1.0f;
	_maxVectorParam5 = 0.0f;
	_numVectorParam6 = -1.0f;
	_maxVectorParam6 = 0.0f;
	_numVectorParam7 = -1.0f;
	_maxVectorParam7 = 0.0f;
	_numVectorParam8 = -1.0f;
	_maxVectorParam8 = 0.0f;
	_numVectorParam9 = -1.0f;
	_maxVectorParam9 = 0.0f;

}

Gl_Sim::~Gl_Sim (void)
{
}

void Gl_Sim::Resize(unsigned vDimVentanaX, unsigned vDimVentanaY)
{
	m_Width_Windows_main = vDimVentanaX;
	m_Height_Windows_main = vDimVentanaY;
	glViewport(0, 0, m_Width_Windows_main, m_Height_Windows_main);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	//glOrtho(-m_Width_Windows_main/2.0f,m_Width_Windows_main/2.0f,-m_Height_Windows_main/2.0f,m_Height_Windows_main/2.0f,-3000,3000);
	glOrtho(0.0f,m_Width_Windows_main,0,m_Height_Windows_main,-3000,3000);

	point_3D ojo;
	SetFull(ojo,0.0f,0.0f,1.0f);

	gluLookAt(	ojo.x, ojo.y, ojo.z, 
				ojo.x, ojo.y, 0.0f,
				0.0f,  1.0f,  0.0f);

	glMatrixMode( GL_MODELVIEW );	// *** GL_MODELVIEW ***
	glLoadIdentity();
};

void Gl_Sim::Renderizar(bool vEstimarTriangulo, EnvironmentSystem* vEnvironmentSystem, AgentsSystem* vMatrix)
{
	if(vEnvironmentSystem->_busy) return;
	glLineWidth(1);
	//glFlush();
	SwapBuffers(hdc);

};

