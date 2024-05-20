#pragma once

//#define TIME_TASKS // Comentar para eliminar la evaluacion temporal... TIME TIEMPO CUENTA RENDIMIENTO FRAME

#ifndef _BASICSFUNCTIONS_H_
	#define _BASICSFUNCTIONS_H_

#include "time.h"
#include "windows.h"
#include "BasicStructures.h"
#include "BasicComplex.h"

#define OPENANGLE 0.0f
#define FACTORESPACIO	(5.0f/100000000.0f)
#define FACTORESPACIOI	(1.0f / FACTORESPACIO)
#define SMALLESTVALUE 0.000001f
#define BIGGESTVALUE 100000.0f

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
	float a,b,c,d0,d1,d2;                   \
	/* is T1 completly inside T2? */        \
	/* check if V0 is inside tri(U0,U1,U2) */\
	a=U1[i1]-U0[i1];                        \
	b=-(U1[i0]-U0[i0]);                     \
	c=-a*U0[i0]-b*U0[i1];                   \
	d0=a*V0[i0]+b*V0[i1]+c;                 \
											\
	a=U2[i1]-U1[i1];                        \
	b=-(U2[i0]-U1[i0]);                     \
	c=-a*U1[i0]-b*U1[i1];                   \
	d1=a*V0[i0]+b*V0[i1]+c;                 \
											\
	if(d0*d1>0.0)                           \
	{										\
		a=U0[i1]-U2[i1];                    \
		b=-(U0[i0]-U2[i0]);                 \
		c=-a*U2[i0]-b*U2[i1];               \
		d2=a*V0[i0]+b*V0[i1]+c;             \
											\
		if(d0*d2>0.0) return true;          \
	}                                       \
}

//Evaluacion de tiempo
double	performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b);

bool 	IsSmall(float vValue);
bool 	IsBig(float vValue);

bool	IsInfinite(point_3D vRetorno);
bool	IsZero(point_3D vRetorno);

float	LengthVector(point_3D V);
void	Igualar(float *vRetorno,float *vValue);
void	Igualar(float *vRetorno,point_3D vValue);
void	Igualar(point_3D &vRetorno,float *vValue);
void	MultiplicarMatrizR(float *C, float *A, float *B);
int* recoverySpiralDataFromImage(int** image, int dimX, int dimY, int numPixeles);
void	MultiplicarMatrizVectorR(float *C, float *A, float *B);
void	MultiplicarMatrizVectorR(point_3D &C, float *A, point_3D B);
void	SetTraslacion(float *m,float vTx,float vTy,float vTz);
void	SetInvTraslacion(float *m,float vTx,float vTy,float vTz);
void	SetTraslacion(float *m,point_3D vTras);
void	SetInvTraslacion(float *m,point_3D vTras);
void	SetRotacion(float *m,float vAngulo, point_3D vDireccion);
void	SetInvRotacion(float *m,float vAngulo, point_3D vDireccion);
void	SetRotacionCos(float *m,float vCosAngulo, point_3D vDireccion);
void	SetInvRotacionCos(float *m,float vCosAngulo, point_3D vDireccion);

bool	InversaLU(float *vRetorno, float *vMatriz);

void 	Zero(point_3D &vValue);
void 	Infinite(point_3D &vValue);
void 	SetEqual(point_3D &vValue,float vChange);
void 	SetFull(point_3D &vValue,float vChangeX,float vChangeY,float vChangeZ);
void 	Limpiar(point_3D &vValue);
void 	Igualar(point_3D &vValue,point_3D v2);
void 	Amplify(point_3D &vValue ,float vAlfa );
void 	Amplify(point_3D &vValue,point_3D v2 ,float vAlfa );
void 	Amplify(point_3D &vValue,float vAlfa,point_3D v2  );
void 	Addition(point_3D &vValue ,point_3D v1, point_3D v2 );
void 	Subtraction(point_3D &vValue ,point_3D v1, point_3D v2 );
void	MultiplicacionPuntual(point_3D &vValue ,point_3D v1, point_3D v2);

point_3D	Subtraction(point_3D v1, point_3D v2 );
point_3D	Addition(point_3D v1, point_3D v2 );

float 	Distance(point_3D v1 ,point_3D v2);

float	NormalizeR(point_3D &vValue);
void 	Cross(point_3D &vValue ,point_3D v1, point_3D v2);
float	CrossNormalize(point_3D &vValue ,point_3D v1, point_3D v2);
float 	Dot(point_3D v1 ,point_3D v2);
float 	DireccionBySubtraction(point_3D &vPRayo ,point_3D vFin,point_3D vInicio);
bool	Perpendicular(point_3D &vPerpendicular, point_3D vVector);

void 	Ray(point_3D &vPRayo ,point_3D vInicio,point_3D vDireccion, float vAlfa);
void 	Ray(point_3D &vPRayo ,point_3D vInicio,float vAlfa,point_3D vDireccion);

float 	DistanceAPlano(point_3D vNormalT , point_3D vDirMov, point_3D vPuntoPlano,point_3D vPuntoInteres);
float 	DistanceAPlanoN(point_3D vNormalT , point_3D vPuntoPlano,point_3D vPuntoInteres);

bool 	IsPointInTriangle(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]);
bool	IsPointInTriangleS(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]);
bool	IsPointInTriangleN(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]);
bool	IsPointInTriangleA(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3],int vTipoAnalisis);
bool	IsPointInTriangleB(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]);
void	OrdenarValoresMinMax(float* vValue,unsigned vDimension);

int		InterseccionRayoEsfera(float *vAlfas, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio);
bool	InterseccionClosedRayoEsfera(float *vAlfa, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio);
bool	InterseccionFarestRayoEsfera(float *vAlfa, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio);

double	InvertGenericLU(int n,double **vResult, double **vMatriz); 


bool    isSegmentsIntersected(point_2D vX_Init, point_2D vX_End, point_2D vY_Init, point_2D vY_End,float *t,float*u);


#endif
