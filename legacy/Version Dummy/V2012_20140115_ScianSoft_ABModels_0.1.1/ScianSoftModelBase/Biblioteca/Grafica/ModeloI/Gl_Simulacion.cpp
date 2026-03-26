#include "GL_Simulacion.h"

GL_Simulacion::GL_Simulacion (void)
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

GL_Simulacion::~GL_Simulacion (void)
{
}

void GL_Simulacion::Resize(unsigned vDimVentanaX, unsigned vDimVentanaY)
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

void GL_Simulacion::Renderizar(bool vEstimarTriangulo, Ambiente* vEnviroment, Sistema_Agentes* vMatrix)
{
	if(vEnviroment->_busy) return;
	register int i;
	register float factorX,factorY,factorZ,deltaX,deltaY;
	point_3D posicionDibujo,direccionDibujo;

	factorX = float(m_Width_Windows_main)/vEnviroment->_dimX;
	factorY = float(m_Height_Windows_main)/vEnviroment->_dimY;
	deltaX	= (factorX > 1.0f)? ((m_Width_Windows_main - vEnviroment->_dimX)/2.0f): 0.0f;

	deltaY	= (factorY > 1.0f)? ((m_Height_Windows_main - vEnviroment->_dimY)/2.0f): 0.0f;
	deltaY  = vEnviroment->_dimY + deltaY;
 
	factorX = (factorX > 1.0f)? 1.0f :factorX;
	factorY = (factorY > 1.0f)? 1.0f :factorY;
	factorZ = 1.0f;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetMaterial(2);
	for(i = 0;i < vMatrix->_numDFCActiveAgents; i++)
	{
		if(vMatrix->_fActive)
		{
			if(vMatrix->_agentesDFC[i]._hija)
			{
				SetMaterial(1);
			}
			else
			{
				SetMaterial(2);
			}
			Igualar(posicionDibujo,vMatrix->_agentesDFC[i]._posicion);
			posicionDibujo.x = factorX*(posicionDibujo.x + deltaX);
			posicionDibujo.y = factorY*(deltaY - posicionDibujo.y);
			primitivaCirculo(posicionDibujo,vMatrix->_agentesDFC[i]._radio,40.0f, 2, 1);

			if(vMatrix->_agentesDFC[i]._adhesionToEVL)
			{
				SetMaterial(0);
			}
			else
			{
				SetMaterial(8);
			}
			Igualar(posicionDibujo,vMatrix->_agentesDFC[i]._posicion);
			Igualar(direccionDibujo,vMatrix->_agentesDFC[i]._velocidad);
			NormalizeR(direccionDibujo);
			float miniRadio = 0.3f * vMatrix->_agentesDFC[i]._radio;
			Amplificar(direccionDibujo,vMatrix->_agentesDFC[i]._radio - 0.9f*miniRadio);
			Suma(posicionDibujo,posicionDibujo,direccionDibujo);

			posicionDibujo.x = factorX*(posicionDibujo.x + deltaX);
			posicionDibujo.y = factorY*(deltaY - posicionDibujo.y);
			posicionDibujo.z = 100.0f;
			primitivaCirculo(posicionDibujo,miniRadio,40.0f, 2, 1);
		}
	}

	SetMaterial(1);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
	glBegin(GL_POLYGON);

		SetFull(posicionDibujo,deltaX + vEnviroment->_minEnviroment.x,vEnviroment->_minEnviroment.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnviroment->_maxEnviroment.x,vEnviroment->_minEnviroment.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnviroment->_maxEnviroment.x,vEnviroment->_maxEnviroment.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnviroment->_minEnviroment.x,vEnviroment->_maxEnviroment.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
		
		//glVertex3f(deltaX + vEnviroment->_minEnviroment..x,vEnviroment->_minEnviroment..y,vEnviroment->_minEnviroment..z);

	glEnd();

	SetMaterial(4);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
	glBegin(GL_LINES);
		SetFull(posicionDibujo,deltaX + vEnviroment->_minEnviroment.x,vEnviroment->_posMarginDEB.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,deltaX + vEnviroment->_maxEnviroment.x,vEnviroment->_posMarginDEB.y,vEnviroment->_minEnviroment.z);
		posicionDibujo.x = factorX * posicionDibujo.x;
		posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
	glEnd();


	if(vEnviroment->_dataEVL_tipoCampo == 0)
	{
		SetMaterial(1);
		glPointSize(10.0f);
		glBegin(GL_POINTS);
			Zero(posicionDibujo);
			posicionDibujo.x = float(vEnviroment->_dimX) / 2.0f;
			posicionDibujo.y = vEnviroment->_dataEVL_PosY;
			posicionDibujo.x = factorX * (posicionDibujo.x + deltaX);
			posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
			posicionDibujo.z = 100.0f;
			glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
		glEnd();
	}
	if(vEnviroment->_dataEVL_tipoCampo == 1)
	{
		SetMaterial(1);
		glPointSize(10.0f);
		glBegin(GL_LINES);
			Zero(posicionDibujo);
			posicionDibujo.x = float(vEnviroment->_dimX) / 2.0f;
			posicionDibujo.y = vEnviroment->_dataEVL_PosY;
			posicionDibujo.x = factorX * (posicionDibujo.x + deltaX);
			posicionDibujo.y = factorY * (deltaY - posicionDibujo.y);
			posicionDibujo.z = 100.0f;
			glVertex3f(posicionDibujo.x - (float(vEnviroment->_dataEVL_AnchoLinea)/2.0f),posicionDibujo.y,posicionDibujo.z);
			glVertex3f(posicionDibujo.x + (float(vEnviroment->_dataEVL_AnchoLinea)/2.0f),posicionDibujo.y,posicionDibujo.z);
		glEnd();
	}

	//////////////// Ventana de parametros
		float ancho = 100.0f;
		float alto  = 105.0f;
		float superalto  = 200.0f;

	SetMaterial(4);
	glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL);
	glBegin(GL_POLYGON);
		SetFull(posicionDibujo,ancho,alto,-100.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,m_Width_Windows_main - ancho,alto,-100.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,m_Width_Windows_main - ancho,superalto,-100.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,ancho,superalto,-100.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
		
		//glVertex3f(deltaX + vEnviroment->_minEnviroment..x,vEnviroment->_minEnviroment..y,vEnviroment->_minEnviroment..z);

	glEnd();

	ancho = ancho + 10;
	alto  = alto  + 10;
	superalto = superalto -10;
	SetMaterial(5);
	glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
	glBegin(GL_LINES);
		SetFull(posicionDibujo,ancho,alto,-50.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,m_Width_Windows_main - ancho,alto,-50.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
	glEnd();
	glBegin(GL_LINES);
		SetFull(posicionDibujo,ancho,alto,-50.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);

		SetFull(posicionDibujo,ancho,superalto,-50.0f);
		glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
	glEnd();

    /// Agregar parametros
// 				SetMaterial(8); azul
//          	SetMaterial(2); verde
//          	SetMaterial(1); dorado
// //          	SetMaterial(0); rojo
//              SetMaterial(4); blanco
//              SetMaterial(5); negro
	glLineWidth(2);
	alto  = alto  + 5;
	float altura = superalto - alto;

	if( vMatrix->_fRunning)
	{
				glPointSize(50.0f);
			/////////////////////////////////  Adhesion Margen posterior....... ancho linea
				SetMaterial(1);
				if(_contador < 1)
				{
					if(_numVectorParam1 != -1.0)
					{
						_vectorParam1[int(_numVectorParam1)] = _vectorParam1[int(_numVectorParam1)]/ float(_stepsContador+1);
					}
					_numVectorParam1++;
					_vectorParam1[int(_numVectorParam1)] = vEnviroment->_dataEVL_AnchoLinea;
				}
				else
				{
					_vectorParam1[int(_numVectorParam1)] = _vectorParam1[int(_numVectorParam1)] + vEnviroment->_dataEVL_AnchoLinea;
				}

				if(vEnviroment->_dataEVL_AnchoLinea > _maxVectorParam1)
				{
					_maxVectorParam1 = vEnviroment->_dataEVL_AnchoLinea;
				}
				glBegin(GL_LINE_STRIP);
					for(register double i = 0.0f; i < _numVectorParam1-1;i = i +1.0f)
					{
						SetFull(posicionDibujo,ancho+i,alto+(altura*(_vectorParam1[int(i)]/_maxVectorParam1)),-10.0f);
						glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
					}
				glEnd();
				
			/////////////////////////////////  CoA
				SetMaterial(8);
				if(_contador < 1)
				{
					if(_numVectorParam2 != -1.0)
					{
						_vectorParam2[int(_numVectorParam2)] = _vectorParam2[int(_numVectorParam2)]/ float(_stepsContador+1);
					}
					_numVectorParam2++;

					_vectorParam2[int(_numVectorParam2)] = vMatrix->_agentesDFC[0]._friendAttraction;
				}
				else
				{
					_vectorParam2[int(_numVectorParam2)] = _vectorParam2[int(_numVectorParam2)] + vMatrix->_agentesDFC[0]._friendAttraction;
				}

				_maxVectorParam2 = 1;
				glBegin(GL_LINE_STRIP);
					for(register double i = 0.0f; i < _numVectorParam2-1;i = i +1.0f)
					{
						SetFull(posicionDibujo,ancho+i,alto+(altura*(_vectorParam2[int(i)]/_maxVectorParam2)),-10.0f);
						glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
					}
				glEnd();


			/////////////////////////////////  Attach Anterior
				SetMaterial(0);
				if(_contador < 1)
				{
					if(_numVectorParam3 != -1.0)
					{
						_vectorParam3[int(_numVectorParam3)] = _vectorParam3[int(_numVectorParam3)]/ float(_stepsContador+1);
					}
					_numVectorParam3++;

					_vectorParam3[int(_numVectorParam3)] = vMatrix->_porcentajeAttachmentAnterior[(vMatrix->_posicionPorAttActual<8)?vMatrix->_posicionPorAttActual:7];
				}
				else
				{
					_vectorParam3[int(_numVectorParam3)] = _vectorParam3[int(_numVectorParam3)] + vMatrix->_porcentajeAttachmentAnterior[(vMatrix->_posicionPorAttActual<8)?vMatrix->_posicionPorAttActual:7];
				}

				_maxVectorParam3 = 1;
				glBegin(GL_LINE_STRIP);
					for(register double i = 0.0f; i < _numVectorParam3-1;i = i +1.0f)
					{
						SetFull(posicionDibujo,ancho+i,alto+(altura*(_vectorParam3[int(i)]/_maxVectorParam3)),-10.0f);
						glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
					}
				glEnd();

			/////////////////////////////////  Attach Posterior
				SetMaterial(3);
				if(_contador < 1)
				{
					if(_numVectorParam4 != -1.0)
					{
						_vectorParam4[int(_numVectorParam4)] = _vectorParam4[int(_numVectorParam4)]/ float(_stepsContador+1);
					}
					_numVectorParam4++;

					_vectorParam4[int(_numVectorParam4)] = vMatrix->_porcentajeAttachmentPosterior[(vMatrix->_posicionPorAttActual<8)?vMatrix->_posicionPorAttActual:7];
				}
				else
				{
					_vectorParam4[int(_numVectorParam4)] = _vectorParam4[int(_numVectorParam4)] + vMatrix->_porcentajeAttachmentPosterior[(vMatrix->_posicionPorAttActual<8)?vMatrix->_posicionPorAttActual:7];
				}

				_maxVectorParam4 = 1;
				glBegin(GL_LINE_STRIP);
					for(register double i = 0.0f; i < _numVectorParam3-1;i = i +1.0f)
					{
						SetFull(posicionDibujo,ancho+i,alto+(altura*(_vectorParam4[int(i)]/_maxVectorParam4)),-10.0f);
						glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
					}
				glEnd();

			/////////////////////////////////  Proliferacion---- numero de celulas
				SetMaterial(2);
				if(_contador < 1)
				{
					if(_numVectorParam5 != -1.0)
					{
						_vectorParam5[int(_numVectorParam5)] = _vectorParam5[int(_numVectorParam5)]/ float(_stepsContador+1);
					}
					_numVectorParam5++;

					_vectorParam5[int(_numVectorParam5)] = vMatrix->_numDFCActiveAgents;
				}
				else
				{
					_vectorParam5[int(_numVectorParam5)] = _vectorParam5[int(_numVectorParam5)] + vMatrix->_numDFCActiveAgents;
				}

				_maxVectorParam5 = 50;
				glBegin(GL_LINE_STRIP);
					for(register double i = 0.0f; i < _numVectorParam3-1;i = i +1.0f)
					{
						SetFull(posicionDibujo,ancho+i,alto+(altura*(_vectorParam5[int(i)]/_maxVectorParam5)),-10.0f);
						glVertex3f(posicionDibujo.x,posicionDibujo.y,posicionDibujo.z);
					}
				glEnd();

			////////////////////////////////// FIN 
				if(_contador < 1)
				{
					_contador = _stepsContador;	
				}
				else
				{
					_contador--;
				}

	}
	glLineWidth(1);
	//glFlush();
	SwapBuffers(hdc);

};

