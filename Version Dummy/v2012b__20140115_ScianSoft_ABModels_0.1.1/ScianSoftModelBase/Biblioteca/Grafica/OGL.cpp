//---------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")

//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl\glaux.h>
#include <gl\glut.h>

#include "OGL.h"

using namespace System;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Windows::Forms;

const GLfloat plasticoNegroAmb[4] = {0.5f,0.0f,0.0f,1.0f};
const GLfloat plasticoNegroDif[4] = {0.31f,0.01f,0.01f,1.0f};
const GLfloat plasticoNegroSpe[4] = {0.7f,0.1f,0.2f,1.0f};
GLfloat plasticoNegroShi = 180.0f;

const GLfloat oroAmb[4] = {0.24725f,0.2245f,0.0645f,1.0f};
const GLfloat oroDif[4] = {0.34615f,0.3143f,0.0903f,1.0f};
const GLfloat oroSpe[4] = {0.797357f,0.723991f,0.208006f,1.0f};
GLfloat oroShi = 128.0f;

const GLfloat esmeraldaAmb[4] = {0.0215f,0.1745f,0.0215f,0.55f};
const GLfloat esmeraldaDif[4] = {0.07568f,0.61424f,0.07568f,0.55f};
const GLfloat esmeraldaSpe[4] = {0.633f,0.727811f,0.633f,0.55};
GLfloat esmeraldaShi = 76.8f;

const GLfloat cromoAmb[4] = {0.25f,0.25f,0.25f,1.0f};
const GLfloat cromoDif[4] = {0.4f,0.4f,0.4f,1.0f};
const GLfloat cromoSpe[4] = {0.774597f,0.774597f,0.774597f,1.0f};
GLfloat cromoShi = 76.8f;

const GLfloat totalAmb[4] = {1.0f,1.0f,1.0f,1.0f};
const GLfloat totalDif[4] = {1.0f,1.0f,1.0f,1.0f};
const GLfloat totalSpe[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat totalShi = 128.0f;

const GLfloat nadaAmb[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat nadaDif[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat nadaSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat nadaShi = 1.0f;

const GLfloat difusionAmb[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat difusionDif[4] = {1.0f,1.0f,1.0f,1.0f};
const GLfloat difusionSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat difusionShi = 1.0f;

const GLfloat ambienteAmb[4] = {1.0f,1.0f,1.0f,1.0f};
const GLfloat ambienteDif[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat ambienteSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat ambienteShi = 1.0f;

const GLfloat especularAmb[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat especularDif[4] = {0.0f,0.0f,0.0f,1.0f};
const GLfloat especularSpe[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat especularShi = 40.0f;

const GLfloat LineaAmb[4] = {0.87f,0.12f,0.1f,1.0f};
const GLfloat LineaDif[4] = {0.67f,0.12f,0.1f,1.0f};
const GLfloat LineaSpe[4] = {0.7f,0.7f,0.7f,1.0f};
GLfloat LineaShi = 103.0f;

const GLfloat NodosAmb[4] = {0.01f,0.1f,0.79f,1.0f};
const GLfloat NodosDif[4] = {0.09f,0.1f,0.97f,1.0f};
const GLfloat NodosSpe[4] = {0.17f,0.16f,0.89f,1.0f};
GLfloat NodosShi = 107.0f;

//---------------------------------------------------------------------------
void OGL::Crear(HWND vHwnd, int v)
{
	m_ventana=v;
	m_show_obb=true;
	m_show_axis=true;
	m_selected=false;
	m_ancho=m_alto=m_largo=50;
	SetFull(m_origen,0,0,0);
	Zero(center_box);
	m_radio_centro=10;

	CamaraVentana=NULL;
	TYPE_MODELS=GL_SMOOTH;

	hwnd=vHwnd;
	int PixelFormat;
	this->hdc= GetDC(vHwnd);
	int bits = 32;
	PIXELFORMATDESCRIPTOR pfd=						// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		bits,										// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	PixelFormat = ChoosePixelFormat(this->hdc,&pfd);
	SetPixelFormat(this->hdc, PixelFormat, &pfd);
	this->hrc = wglCreateContext(this->hdc);
	if(this->hrc == NULL)
	{
		MessageBox::Show("wglCreateContext Failed");
	}
	if(wglMakeCurrent(this->hdc,this->hrc)==false)
	{
		MessageBox::Show("wglMakeCurrent Failed");
	}
	InitOpenGL();
	BuildFont();
}

void OGL::Destruir()
{
	wglMakeCurrent (NULL, NULL);
	wglDeleteContext(this->hrc);
	DestruirFont();

}
void OGL::DestruirFont(void)
{
	glDeleteLists(base, 96);							// Delete All 96 Characters

}
bool OGL::SetActual()
{
	wglMakeCurrent (NULL, NULL);
	if( wglMakeCurrent(this->hdc,this->hrc) == false)
	{
		//MessageBox::Show("wglMakeCurrent Failed");
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Conjunto de funciones de dibujo ////////////////////////////////////////////////////////////

void OGL::InitOpenGL()
{
	Key_Status = KEY_T_HOME;
	Key_Status_Save= KEY_T_HOME;
	LightAmbient[0]=0.0f;LightAmbient[1]=0.0f;LightAmbient[2]=0.0f;LightAmbient[3]=1.0f;
	LightDiffuse[0]=1.0f;LightDiffuse[1]=1.0f;LightDiffuse[2]=1.0f;LightDiffuse[3]=1.0f;
	LightSpecular[0]=1.0f;LightSpecular[1]=1.0f;LightSpecular[2]=1.0f;LightSpecular[3]=1.0f;

	point_3D vector_direccion;
	//SetFull(vector_direccion,16,16,11);
	SetFull(vector_direccion,0,0,10);
	NormalizeR(vector_direccion);
	float radio = 100;
	Amplificar(pos_eye_camara,radio,vector_direccion);
	LightPosition[0]=pos_eye_camara.x;LightPosition[1]=pos_eye_camara.y;LightPosition[2]=pos_eye_camara.z;LightPosition[3]=1.0f;
	
	CamaraVentana = new GL_Camara(pos_eye_camara.x,pos_eye_camara.y,pos_eye_camara.z);

	glClearColor(0/255.0f, 0/255.0f, 0/255.0f, 1.0f);

	//Parametros de la luz 0
	GLfloat ambientlight[] =	{0.5f ,0.5f ,0.5f ,1.0f};
	GLfloat difuselight[] =		{1.0f ,1.0f ,1.0f ,1.0f};
	GLfloat specularlight[] =	{1.0f ,1.0f ,1.0f ,1.0f};
	GLfloat lightposition[] =	{1000.0f,1000.0f,1000.0f,0.0f};
	
	//Se setean los parametros
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientlight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,difuselight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularlight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightposition);

	GLfloat lightposition2[] =	{1000.0f,1000.0f,-1000.0f,0.0f};
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambientlight);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,difuselight);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specularlight);
	glLightfv(GL_LIGHT1,GL_POSITION,lightposition2);


	glEnable(GL_DEPTH_TEST);	//Test de profundidad
	glEnable(GL_LIGHTING);	    //se activa la iluminacion
	glEnable(GL_LIGHT0);	    //se activa la luz 0 que previamente se habia seteado
	glEnable(GL_LIGHT1);	    //se activa la luz 0 que previamente se habia seteado

	glEnable(GL_COLOR_MATERIAL);	//Se activa lo materiales de color
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);  //Se desean de tipo ambiente y difusión (tambien incluyen specular
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);  //Se desean de tipo ambiente y difusión (tambien incluyen specular
	glColorMaterial(GL_BACK,GL_AMBIENT_AND_DIFFUSE);  //Se desean de tipo ambiente y difusión (tambien incluyen specular

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);								
	glDepthFunc(GL_LEQUAL);
}

//-------------------------Definidas-------------------------------------
void OGL::Resize(int w, int h)
{
	m_Width_Windows_main=w;
	m_Height_Windows_main=h;
	//glViewport(0, 0, m_Width_Windows_main, m_Height_Windows_main);

	point_3D ojo;
	CamaraVentana->SetCamAt(0.0f,0.0f,0.0f);
	SetFull(ojo,0.0f,0.0f,100.0f);
	SetEyeCamara(ojo);

	CamaraVentana->SetViewport(m_Width_Windows_main,m_Height_Windows_main);
	CamaraVentana->SetGLAspectRatioCamera();
}

void OGL::Renderizar(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	CamaraVentana->SetGLCamera();
	//CamaraVentana ->GetMatrix(modelview_First,projection_First,viewport_First);
	SetLight();
	glDrawAxis();
	glFlush();
	SwapBuffers(hdc);
}

void OGL::SetLight(bool setluz)
{
	if(setluz)
	{	
		point_3D LUZP = CamaraVentana->GetCamDirView();
		NormalizeR(LUZP);
		Amplificar(LUZP,LUZP,2000);
		LightPosition[0]=LUZP.x;
		LightPosition[1]=LUZP.y;
		LightPosition[2]=LUZP.z;
		LightPosition[3]=1.0f;
	}
	glShadeModel(TYPE_MODELS);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition); // Position The Light
}

void OGL::glDrawAxis(void)
{
	point_3D cent = m_origen;
	glPushMatrix();
	glTranslatef(cent.x,cent.y,cent.z);
		glDisable (GL_LIGHTING);
		float fact_2 = 15;
		float fact_x = m_ancho+fact_2*3;
		float fact_y = m_alto+fact_2*3;
		float fact_z = m_largo+fact_2*3;
		glColor3ub(255, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0*fact_x, 0.0, 0.0);
		glVertex3f((fact_x-fact_2), 0.25*fact_2, 0.0);
		glVertex3f((fact_x-fact_2), -0.25*fact_2, 0.0);
		glVertex3f(1.0*fact_x, 0.0, 0.0);
		glVertex3f((fact_x-fact_2), 0.0, 0.25*fact_2);
		glVertex3f((fact_x-fact_2), 0.0, -0.25*fact_2);
		glVertex3f(1.0*fact_x, 0.0, 0.0);
		glEnd();
		glColor3ub(0, 255, 0);
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0*fact_y, 0.0);
		glVertex3f(0.0, (fact_y-fact_2), 0.25*fact_2);
		glVertex3f(0.0, (fact_y-fact_2), -0.25*fact_2);
		glVertex3f(0.0, 1.0*fact_y, 0.0);
		glVertex3f(0.25*fact_2, (fact_y-fact_2), 0.0);
		glVertex3f(-0.25*fact_2, (fact_y-fact_2), 0.0);
		glVertex3f(0.0, 1.0*fact_y, 0.0);
		glEnd();
		glColor3ub(0, 0, 255);
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0*fact_z);
		glVertex3f(0.25*fact_2, 0.0, (fact_z-fact_2));
		glVertex3f(-0.25*fact_2, 0.0, (fact_z-fact_2));
		glVertex3f(0.0, 0.0, 1.0*fact_z);
		glVertex3f(0.0, 0.25*fact_2, (fact_z-fact_2));
		glVertex3f(0.0, -0.25*fact_2, (fact_z-fact_2));
		glVertex3f(0.0, 0.0, 1.0*fact_z);
		glEnd();
	    
		glColor3ub(255, 255, 0);
		glRasterPos3f(1.1*fact_x, 0.0, 0.0);
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
		glRasterPos3f(0.0, 1.1*fact_y, 0.0);
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
		glRasterPos3f(0.0, 0.0, 1.1*fact_z);
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
		glEnable (GL_LIGHTING);
	glPopMatrix();
}

void OGL::KeyBoard(int key)
{
	if(CamaraVentana!=NULL)
		CamaraVentana->KeyBoard(key);
	Key_Status = key;
	if(Key_Status==KEY_T_F1)
		Key_Status_Save=Key_Status;
}
void OGL::MouseMove(int x, int y)
{
	if(CamaraVentana!=NULL)
		CamaraVentana->MouseMove( x, y);
	if(Key_Status==KEY_T_F3)
		center_box = CamaraVentana->GetCamViewCenter();
}
void OGL::MouseLButtonDown(int x, int y)
{
	if(CamaraVentana!=NULL)
	{
		CamaraVentana->SetEstado(KEY_M_L_DOWN);
		CamaraVentana->ClickMouse(x,y);
	}
}
void OGL::MouseLButtonUp(int x, int y)
{
	if(CamaraVentana!=NULL)
	{
		CamaraVentana->SetEstado(KEY_M_L_UP);
		CamaraVentana->ClickMouse(x,y);
	}
}

void OGL::ViewportCircular(float factor, int angulo)
{
	int vp[4];
	float j,radio,radioExt,delta,z;
	point_3D externoIni,externoFin,puntoIni,puntoFin,centro,temp;

	factor = 0.4;
	SetFull(centro,0.5* m_Width_Windows_main,0.5* m_Height_Windows_main,0);
	radioExt = 2.0 * ((m_Width_Windows_main > m_Height_Windows_main)? m_Width_Windows_main: m_Height_Windows_main);
	radio = factor * ((m_Width_Windows_main < m_Height_Windows_main)? m_Width_Windows_main: m_Height_Windows_main);
	delta = 1.0;
	z = 1000;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT, vp);
	glOrtho(vp[0],vp[2],vp[1],vp[3],-z,z);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_BLEND);

	SetFull(puntoIni,centro.x + radio * cos(0.0f),centro.y + radio * sin(0.0f),z);
	SetFull(externoIni,centro.x + radioExt * cos(0.0f),centro.y + radioExt * sin(0.0f),z);

	for(j = 0; j < 360; j += delta)
	{
		SetFull(puntoFin,centro.x + radio * cos((j+delta) * PI/180.0f),centro.y + radio * sin((j+delta) * PI/180.0f),z);
		SetFull(externoFin,centro.x + radioExt * cos((j+delta) * PI/180.0f),centro.y + radioExt * sin((j+delta) * PI/180.0f),z);

		glBegin(GL_POLYGON);
		glVertex3f(puntoIni.x,puntoIni.y,puntoIni.z);
		glVertex3f(puntoFin.x,puntoFin.y,puntoFin.z);
		glVertex3f(externoFin.x,externoFin.y,externoFin.z);
		glVertex3f(externoIni.x,externoIni.y,externoIni.z);
		glEnd();

		puntoIni = puntoFin;
		externoIni = externoFin;
	}

	point_3D a,b,c;
	point_3D U,V,Pa,Pb,Pc;

	float angulo_r =  angulo*PI/180.0f;
	float angulo_2  = 30*PI/180.0f;

	SetFull(U,0,1,0);
	SetFull(V,1,0,0);

	Amplificar(temp,cos(angulo_r),V);
	Ray(Pa,temp,sin(angulo_r),U);
	NormalizeR(Pa);

	Amplificar(temp,cos(angulo_r+angulo_2),V);
	Ray(Pb,temp,sin(angulo_r+angulo_2),U);
	NormalizeR(Pb);

	Amplificar(temp,cos(angulo_r-angulo_2),V);
	Ray(Pc,temp,sin(angulo_r-angulo_2),U);
	NormalizeR(Pc);

	Ray(a,centro,(radio-radio*.1),Pa);
	Ray(b,a,(radio*10000),Pb);
	Ray(c,a,(radio*10000),Pc);

	glBegin(GL_POLYGON);
	glVertex3f(a.x,a.y,a.z);
	glVertex3f(b.x,b.y,b.z);
	glVertex3f(c.x,c.y,c.z);
	glEnd();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
	glPopAttrib();
}
void OGL::Print(float color[3], float x, float y, string nombre )
{
	int vp[4];
	int i;
	float z = 1001;


	//glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT, vp);
	glOrtho(vp[0],vp[2],vp[1],vp[3],-z,z);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_BLEND);
	glRasterPos3f(x, vp[3] - 15 - y,z);
	char text[256];
	for (i = 0; i < (int)nombre.length(); i++)
	{
		text[i]=nombre[i];
	}
	text[i] = '\0';

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
		glListBase(base - 32); // apunta a la Display List del primer caracter
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // dibuja las Display List correspondientes al texto
	glPopAttrib();										// Pops The Display List Bits

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
	//glPopAttrib();

} 
GLvoid OGL::BuildFont(int	dim	)								// Build Our Bitmap Font
{
	

	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters
	

#ifdef UNICODE
	font = CreateFont(	-dim,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						LPTSTR("Courier New"));			// Font Name
#else
	font = CreateFont(	-dim,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name
#endif

	if(font != NULL)
	{

		oldfont = (HFONT)SelectObject(hdc, font);           // Selects The Font We Want
		wglUseFontBitmaps(hdc, 32, 96, base);				// Builds 96 Characters Starting At Character 32
		SelectObject(hdc, oldfont);							// Selects The Font We Want
		DeleteObject(font);									// Delete The Font
	}
}
void OGL::SetCameraNearFarAngle(float n, float f, float a)
{
	CamaraVentana->SetCameraNear(n);
	CamaraVentana->SetCameraFar(f);
	CamaraVentana->SetCameraApertura(a);
}


void OGL::SetMaterial(int vMaterial)
{
	if(vMaterial == 0)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plasticoNegroAmb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plasticoNegroDif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plasticoNegroSpe);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, plasticoNegroShi);
	}
	if(vMaterial == 1)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, oroAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, oroDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, oroSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, oroShi);
	}
	if(vMaterial == 2)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, esmeraldaAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, esmeraldaDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, esmeraldaSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, esmeraldaShi);
	}
	if(vMaterial == 3)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cromoAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cromoDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cromoSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cromoShi);
	}
	if(vMaterial == 4)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, totalAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, totalDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, totalSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, totalShi);
	}
	if(vMaterial == 5)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, nadaAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, nadaDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, nadaSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, nadaShi);
	}
	if(vMaterial == 6)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, difusionAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difusionDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, difusionSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, difusionShi);
	}
	if(vMaterial == 7)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  LineaAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  LineaDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LineaSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, LineaShi);
	}
	if(vMaterial == 8)
	{
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  NodosAmb);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  NodosDif);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, NodosSpe);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, NodosShi);
	}
	if(vMaterial == 9)
	{
	}
	if(vMaterial == 10)
	{
	}
}
void OGL::primitivaCirculo(point_3D vCentro,float vRadio,float vPasos, int vTipo, int vSizePoint)
{
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
	
		float maximo,paso;
		maximo = 2*PI;
		paso = maximo/vPasos;

		glPushMatrix();
			glTranslatef(vCentro.x,vCentro.y,vCentro.z);
			glutSolidSphere(vRadio, vPasos, vPasos);
		glPopMatrix();
};