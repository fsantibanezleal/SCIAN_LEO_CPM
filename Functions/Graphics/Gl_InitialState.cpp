#include "Gl_InitialState.h"

Gl_InitialState::Gl_InitialState(void)
{
};

Gl_InitialState::~Gl_InitialState(void)
{
};

void Gl_InitialState::Resize(unsigned vDimVentanaX, unsigned vDimVentanaY)
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


void Gl_InitialState::Renderizar(EnvironmentSystem* vEnvironmentSystem, AgentsSystem* vMatrix)
{
	if(vEnvironmentSystem->_busy) return;
	register int i;
	register float factorX,factorY,factorZ,deltaX,deltaY;
	point_3D posicionDibujo;

	factorX = float(m_Width_Windows_main)/vEnvironmentSystem->_dimX;
	factorY = float(m_Height_Windows_main)/vEnvironmentSystem->_dimY;
	deltaX	= (factorX > 1.0f)? ((m_Width_Windows_main - vEnvironmentSystem->_dimX)/2.0f): 0.0f;

	deltaY	= (factorY > 1.0f)? ((m_Height_Windows_main - vEnvironmentSystem->_dimY)/2.0f): 0.0f;
	deltaY  = vEnvironmentSystem->_dimY + deltaY;
 
	factorX = (factorX > 1.0f)? 1.0f :factorX;
	factorY = (factorY > 1.0f)? 1.0f :factorY;
	factorZ = 1.0f;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(i = 0;i < vMatrix->_numDFCActiveAgents; i++)
	{
		if(vMatrix->_fActive)
		{
			point_3D p1;
			SetFull(p1,vMatrix->_agentsWM_DFC[i].GetPosX(),vMatrix->_agentsWM_DFC[i].GetPosY(),0.0);
			Igualar(posicionDibujo,p1);

			SetMaterial(2);
			DrawPoly(posicionDibujo,vMatrix->_agentsWM_DFC[i].GetRad(0),vMatrix->_agentsWM_DFC[i].GetPoly(),40.0f, 1, 1,factorX,factorY,deltaX,deltaY);

			SetMaterial(1);
			posicionDibujo.x = factorX*(posicionDibujo.x + deltaX);
			posicionDibujo.y = factorY*(deltaY - posicionDibujo.y);
			primitivaCirculo(posicionDibujo,vMatrix->_agentsWM_DFC[i].GetRad(0),40.0f, 2, 1);


		}
	}

	SetMaterial(1);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
	glBegin(GL_POLYGON);
		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_minEnvironmentSystem.x,vEnvironmentSystem->_minEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_maxEnvironmentSystem.x,vEnvironmentSystem->_minEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_maxEnvironmentSystem.x,vEnvironmentSystem->_maxEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_minEnvironmentSystem.x,vEnvironmentSystem->_maxEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
		
		//glVertex3f(deltaX + vEnvironmentSystem->_minEnvironmentSystem..x,vEnvironmentSystem->_minEnvironmentSystem..y,vEnvironmentSystem->_minEnvironmentSystem..z);

	glEnd();

	SetMaterial(0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		//glVertex3f(vPosicionCampo.x,vPosicionCampo.y,vPosicionCampo.z);
	glEnd();
	//glFlush();
	SwapBuffers(hdc);
};

void Gl_InitialState::DrawPoly(point_3D vCentro,float vRadio,polygon_2D _polyMembrane,float vPasos, int vTipo, int vSizePoint,float factorX,float factorY,float deltaX,float deltaY)
{
	point_3D posicionDibujo;

	glPointSize(vSizePoint);
	if(vTipo == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK ,GL_POINT);
	}
	if(vTipo == 1)
	{
		
		glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
	}
	if(vTipo == 2)
	{
		glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL);
	}
	
		glPushMatrix();
			glBegin(GL_POLYGON);
				BOOST_FOREACH(point_2D const& pp, _polyMembrane.outer()) {

					posicionDibujo.x = factorX*(pp.x() + deltaX);
					posicionDibujo.y = factorY*(deltaY - pp.y());
		
					glVertex3f(posicionDibujo.x,posicionDibujo.y,0.0);
		
				}

			glEnd();

			float maximo,paso;
			maximo = 2*PI;
			paso = maximo/vPasos;

			posicionDibujo.x = factorX*(vCentro.x + deltaX);
			posicionDibujo.y = factorY*(deltaY    - vCentro.y);

			//glTranslatef(posicionDibujo.x,posicionDibujo.y,vCentro.z);
			//glutSolidSphere(vRadio, vPasos, vPasos);
			//glutWireSphere(vRadio, vPasos, vPasos);
		glPopMatrix();

};

//void Gl_InitialState::Renderizar(EnvironmentSystem* vEnvironmentSystem, AgentsSystem* vMatrix)
//{
//	if(vEnvironmentSystem->_busy) return;
//	register int i;
//	register float factorX,factorY,factorZ,deltaX,deltaY;
//	point_3D posicionDibujo;
//
//	factorX = float(m_Width_Windows_main)/vEnvironmentSystem->_dimX;
//	factorY = float(m_Height_Windows_main)/vEnvironmentSystem->_dimY;
//	deltaX	= (factorX > 1.0f)? ((m_Width_Windows_main - vEnvironmentSystem->_dimX)/2.0f): 0.0f;
//
//	deltaY	= (factorY > 1.0f)? ((m_Height_Windows_main - vEnvironmentSystem->_dimY)/2.0f): 0.0f;
//	deltaY  = vEnvironmentSystem->_dimY + deltaY;
// 
//	factorX = (factorX > 1.0f)? 1.0f :factorX;
//	factorY = (factorY > 1.0f)? 1.0f :factorY;
//	factorZ = 1.0f;
//
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	SetMaterial(2);
//	for(i = 0;i < vMatrix->_numDFCActiveAgents; i++)
//	{
//		if(vMatrix->_fActive)
//		{
//			Igualar(posicionDibujo,vMatrix->_agentsWM_DFC[i]._pos);
//			posicionDibujo.x = factorX*(posicionDibujo.x + deltaX);
//			posicionDibujo.y = factorY*(deltaY - posicionDibujo.y);
//			primitivaCirculo(posicionDibujo,vMatrix->_agentsWM_DFC[i]._radius,40.0f, 2, 1);
//		}
//	}
//
//	SetMaterial(1);
//	glPointSize(3.0f);
//	glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
//	glBegin(GL_POLYGON);
//		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_minEnvironmentSystem.x,vEnvironmentSystem->_minEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
//		posicionDibujo.x = factorX * posicionDibujo.x;
//		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
//		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
//
//		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_maxEnvironmentSystem.x,vEnvironmentSystem->_minEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
//		posicionDibujo.x = factorX * posicionDibujo.x;
//		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
//		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
//
//		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_maxEnvironmentSystem.x,vEnvironmentSystem->_maxEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
//		posicionDibujo.x = factorX * posicionDibujo.x;
//		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
//		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
//
//		SetFull(posicionDibujo,deltaX + vEnvironmentSystem->_minEnvironmentSystem.x,vEnvironmentSystem->_maxEnvironmentSystem.y,vEnvironmentSystem->_minEnvironmentSystem.z);
//		posicionDibujo.x = factorX * posicionDibujo.x;
//		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
//		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
//		
//		//glVertex3f(deltaX + vEnvironmentSystem->_minEnvironmentSystem..x,vEnvironmentSystem->_minEnvironmentSystem..y,vEnvironmentSystem->_minEnvironmentSystem..z);
//
//	glEnd();
//
//	SetMaterial(0);
//	glPointSize(10.0f);
//	glBegin(GL_POINTS);
//		//glVertex3f(vPosicionCampo.x,vPosicionCampo.y,vPosicionCampo.z);
//	glEnd();
//	//glFlush();
//	SwapBuffers(hdc);
//};
