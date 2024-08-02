#include "BasicFunctions.h"

double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b) // milisegundos
{
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)1000.0 * (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
};

bool IsSmall(float vValue)
{
	return (fabs(vValue) <= SMALLESTVALUE)? true:false;
};

bool IsBig(float vValue)
{
	return (fabs(vValue) >= BIGGESTVALUE)? true:false;
};

bool IsInfinite(point_3D vRetorno)
{
	return (vRetorno.x == BIGGESTVALUE)? true: false;
};

bool IsZero(point_3D vRetorno)
{
	return IsSmall(LengthVector(vRetorno));
};

float LengthVector(point_3D V)
{
	return sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
};

void Igualar(float *vRetorno,float *vValue)
{
	register unsigned i;
	for(i = 0; i < 16; i++)
	{
		vRetorno[i] = vValue[i];
	}
};

void Igualar(float *vRetorno,point_3D vValue)
{
	vRetorno[0] = vValue.x;
	vRetorno[1] = vValue.y;
	vRetorno[2] = vValue.z;
	vRetorno[3] = 1.0f;
};

void Igualar(point_3D &vRetorno,float *vValue)
{
	vRetorno.x = vValue[0];
	vRetorno.y = vValue[1];
	vRetorno.z = vValue[2];
};

void MultiplicarMatrizR(float *C, float *A, float *B)
{
	register float sum,tempA[16],tempB[16];
	register int i,j,k;

	Igualar(tempA,A);
	Igualar(tempB,B); // Es mas rapido hacer un solo for aqui... :D .. en fin esto se hace solo en cada actualizacion.. hay time
	for( i = 0 ; i < 4 ; i++)
	{
		for( j = 0 ; j < 4 ; j++)
		{
			sum = 0.0f;
			for( k = 0 ; k < 4 ; k++)
			{
				sum += tempA[ k * 4 + i] * tempB[ j * 4 + k];
			}
			C[ j * 4 + i] = sum;
		}
	}

};

// Define type of return...

int* recoverySpiralDataFromImage(int** image, int dimX, int dimY, int numPixeles)
{
	int* spiralPixels = (int*) malloc(numPixeles*sizeof(int));

	int posX = floor(dimX/2.0f);
	int posY = floor(dimY/2.0f);

	// possible directions
	// 1 : down
	// 2 : right
	// 3 : up
	// 4 : left
	int direccion        = 1;
	int persistencia     = 1;
	int stepsByDirection = 1;
	int counterChangePer = 0;

	spiralPixels[0] = image[posX][posY];
	int i = 1;
	while(1)
	{
		counterChangePer++;
		for(int idxFill = 0;idxFill <persistencia ;idxFill++)
		{
			if(direccion == 1)
			{// Bajando
				posX--;
			}
			if(direccion == 2)
			{// a la derecha
				posY++;
			}
			if(direccion == 3)
			{// subiendo
				posX++;
			}
			if(direccion == 4)
			{// izquierdo
				posY--;
			}

			if((posX < 0) || (posX > (dimX-1)) || (posY < 0) || (posY > (dimY-1)))
			{
				return spiralPixels;
			}
			else
			{
				spiralPixels[i] = image[posX][posY];
			}
			i++;
			if(i >= numPixeles)
			{
				return spiralPixels;
			}
		}
	
		// Actualizar a siguiente direccion
		direccion++;
		if(direccion > 4)
		{
			direccion = 1;
		}
		if(counterChangePer == 2)
		{
			counterChangePer = 0;
			persistencia++;
		}
	}
	return spiralPixels;
}

void MultiplicarMatrizVectorR(float *C, float *A, float *B)
{
	register int i,j;
	register float sum,tempB[4];

	Igualar(tempB,B);
	for( i = 0 ; i < 4 ; i++)
	{
		sum = 0.0f;
		for(j = 0 ; j < 4 ; j++)
		{
			sum += A[ j*4 + i] * tempB[j];
		}
		C[i] = sum;
	}
};

void MultiplicarMatrizVectorR(point_3D &C, float *A, point_3D B)
{
	register int i,j;
	register float sum,tempB[4],tempRet[4];

	Igualar(tempB,B);
	for( i = 0 ; i < 4 ; i++)
	{
		sum = 0.0f;
		for(j = 0 ; j < 4 ; j++)
		{
			sum += A[ j*4 + i] * tempB[j];
		}
		tempRet[i] = sum;
	}
	Igualar(C,tempRet);
};

void SetTraslacion(float *m,float vTx,float vTy,float vTz)
{
	m[0]=1.0f;m[4]=0.0f;m[8 ]=0.0f;m[12]=vTx;
	m[1]=0.0f;m[5]=1.0f;m[9 ]=0.0f;m[13]=vTy;
	m[2]=0.0f;m[6]=0.0f;m[10]=1.0f;m[14]=vTz;
	m[3]=0.0f;m[7]=0.0f;m[11]=0.0f;m[15]=1.0f;
};

void SetInvTraslacion(float *m,float vTx,float vTy,float vTz)
{
	m[0]=1.0f;m[4]=0.0f;m[8 ]=0.0f;m[12]=-vTx;
	m[1]=0.0f;m[5]=1.0f;m[9 ]=0.0f;m[13]=-vTy;
	m[2]=0.0f;m[6]=0.0f;m[10]=1.0f;m[14]=-vTz;
	m[3]=0.0f;m[7]=0.0f;m[11]=0.0f;m[15]=1.0f;
};

void SetTraslacion(float *m,point_3D vTras)
{
	m[0]=1.0f;m[4]=0.0f;m[8 ]=0.0f;m[12]=vTras.x;
	m[1]=0.0f;m[5]=1.0f;m[9 ]=0.0f;m[13]=vTras.y;
	m[2]=0.0f;m[6]=0.0f;m[10]=1.0f;m[14]=vTras.z;
	m[3]=0.0f;m[7]=0.0f;m[11]=0.0f;m[15]=1.0f;
};

void SetInvTraslacion(float *m,point_3D vTras)
{
	m[0]=1.0f;m[4]=0.0f;m[8 ]=0.0f;m[12]=-vTras.x;
	m[1]=0.0f;m[5]=1.0f;m[9 ]=0.0f;m[13]=-vTras.y;
	m[2]=0.0f;m[6]=0.0f;m[10]=1.0f;m[14]=-vTras.z;
	m[3]=0.0f;m[7]=0.0f;m[11]=0.0f;m[15]=1.0f;
};

void SetRotacion(float *m,float vAngulo, point_3D vDireccion)
{
	register float unoMenosCos,sinAlfa,cosAlfa;
	point_3D dir,dir2;
	Igualar(dir,vDireccion);
	MultiplicacionPuntual(dir2,dir,dir);

	NormalizeR(vDireccion);

	cosAlfa		= cos(vAngulo);
	sinAlfa		= sin(vAngulo);
	unoMenosCos = 1.0f - cosAlfa;

	m[0]=(dir2.x + (dir2.y + dir2.z)* cosAlfa);		m[4]=(dir.x*dir.y*unoMenosCos - dir.z*sinAlfa);	m[8 ]=(dir.x*dir.z*unoMenosCos + dir.y*sinAlfa);	m[12]=0.0f;
	m[1]=(dir.x*dir.y*unoMenosCos + dir.z*sinAlfa);	m[5]=(dir2.y + (dir2.x + dir2.z)* cosAlfa);		m[9 ]=(dir.y*dir.z*unoMenosCos - dir.x*sinAlfa);	m[13]=0.0f;
	m[2]=(dir.x*dir.z*unoMenosCos - dir.y*sinAlfa);	m[6]=(dir.y*dir.z*unoMenosCos + dir.x*sinAlfa);	m[10]=(dir2.z + (dir2.x + dir2.y)* cosAlfa);		m[14]=0.0f;
	m[3]=0.0f;										m[7]=0.0f;										m[11]=0.0f;											m[15]=1.0f;
};

void SetInvRotacion(float *m,float vAngulo, point_3D vDireccion)
{
	vAngulo = - vAngulo;
	SetRotacion(m,vAngulo,vDireccion);
};

void SetRotacionCos(float *m,float vCosAngulo, point_3D vDireccion)
{
	register float unoMenosCos,sinAlfa;
	point_3D dir,dir2;
	Igualar(dir,vDireccion);
	MultiplicacionPuntual(dir2,dir,dir);

	NormalizeR(vDireccion);

	unoMenosCos = 1.0f - vCosAngulo;
	sinAlfa = sqrtf(1.0f - vCosAngulo*vCosAngulo);

	m[0]=(dir2.x + (dir2.y + dir2.z)* vCosAngulo);	m[4]=(dir.x*dir.y*unoMenosCos - dir.z*sinAlfa);	m[8 ]=(dir.x*dir.z*unoMenosCos + dir.y*sinAlfa);	m[12]=0.0f;
	m[1]=(dir.x*dir.y*unoMenosCos + dir.z*sinAlfa);	m[5]=(dir2.y + (dir2.x + dir2.z)* vCosAngulo);	m[9 ]=(dir.y*dir.z*unoMenosCos - dir.x*sinAlfa);	m[13]=0.0f;
	m[2]=(dir.x*dir.z*unoMenosCos - dir.y*sinAlfa);	m[6]=(dir.y*dir.z*unoMenosCos + dir.x*sinAlfa);	m[10]=(dir2.z + (dir2.x + dir2.y)* vCosAngulo);		m[14]=0.0f;
	m[3]=0.0f;										m[7]=0.0f;										m[11]=0.0f;											m[15]=1.0f;
};

void SetInvRotacionCos(float *m,float vCosAngulo, point_3D vDireccion)
{
	vCosAngulo = - vCosAngulo;
	SetRotacionCos(m,vCosAngulo,vDireccion);
};

bool InversaLU(float *vRetorno, float *vMatriz)
{
	int filas,columnas;
	filas=columnas=4;

	int i,j,k;
	float m_origen_copy[16];
	float* b = new float[columnas];
	float factor,sum;
	for(int i=0;i<16;i++)
		m_origen_copy[i]=vMatriz[i];
	
	for(k=0;k<columnas-1;k++)
	{
		for(i = k + 1; i < columnas; i++)
		{
			if(m_origen_copy[k*4+k] == 0)
			{
				for(int pi=0;pi<4;pi++)
				{
					for(int pj=0;pj<4;pj++)
						if(pi!=pj)
							vRetorno[pi*4+pj]=0.0;
						else
							vRetorno[pi*4+pj]=1.0;
				}
				return false;
			}
			factor = m_origen_copy[i*4+k]/m_origen_copy[k*4+k];
			m_origen_copy[i*4+k] = factor;
			if(factor)
			{
				for(j = k + 1; j < columnas; j++)
				{
					m_origen_copy[i*4+j] -= factor*m_origen_copy[k*4+j];
				}
			}
		}
	}
	for(j=0;j<columnas;j++)
	{
		for (i = 0; i < columnas; i++) 
		{
			b[i] = (i==j)?1.0f:0.0f;
		}

		for(i = 1;i<columnas;i++)
		{                    
			sum=b[i];
			for(k=0;k<i;k++)
				sum=sum-m_origen_copy[i*4+k]*b[k];
			b[i]=sum;
		}

		vRetorno[(columnas-1)*4+j]=b[columnas-1]/m_origen_copy[(columnas-1)*4+(columnas-1)];
		
		for(i=columnas;i>0;i--)
		{
			sum=0;
			for(k = i; k < columnas; k++)
				sum=sum + m_origen_copy[(i-1)*4+k]*vRetorno[k*4+j];
			vRetorno[(i-1)*4+j]=(b[i-1]-sum)/m_origen_copy[(i-1)*4+(i-1)];
		}
	}
	delete b;

	return true;
};


void Zero(point_3D &vValue)
{
	vValue.x = vValue.y = vValue.z = 0.0f;
};

void Infinite(point_3D &vValue)
{
	vValue.x = vValue.y = vValue.z = BIGGESTVALUE;
};

void SetEqual(point_3D &vValue,float vChange)
{
	vValue.x = vValue.y = vValue.z = vChange;
};
void SetFull(point_3D &vValue,float vChangeX,float vChangeY,float vChangeZ)
{
	vValue.x = vChangeX;
	vValue.y = vChangeY;
	vValue.z = vChangeZ;
};

void Limpiar(point_3D &vValue)
{
	if(IsSmall(vValue.x)) vValue.x = 0.0f;
	if(IsSmall(vValue.y)) vValue.y = 0.0f;
	if(IsSmall(vValue.z)) vValue.z = 0.0f;
};

void Igualar(point_3D &vValue,point_3D v2)
{
	vValue.x = v2.x;vValue.y = v2.y;vValue.z = v2.z;
};

void Amplificar(point_3D &vValue ,float vAlfa )
{
	vValue.x = vAlfa * vValue.x;
	vValue.y = vAlfa * vValue.y;
	vValue.z = vAlfa * vValue.z;
};

void Amplificar(point_3D &vValue,point_3D v2 ,float vAlfa)
{
	vValue.x = vAlfa * v2.x;
	vValue.y = vAlfa * v2.y;
	vValue.z = vAlfa * v2.z;
};

void Amplificar(point_3D &vValue,float vAlfa,point_3D v2 )
{
	Amplificar(vValue,v2,vAlfa );
};

void Suma(point_3D &vValue ,point_3D v1, point_3D v2)
{
   vValue.x=v1.x + v2.x;
   vValue.y=v1.y + v2.y;
   vValue.z=v1.z + v2.z;
}; 

void Resta(point_3D &vValue ,point_3D v1, point_3D v2)
{
   vValue.x=v1.x - v2.x;
   vValue.y=v1.y - v2.y;
   vValue.z=v1.z - v2.z;
}; 

void MultiplicacionPuntual(point_3D &vValue ,point_3D v1, point_3D v2)
{
	vValue.x = v1.x * v2.x;
	vValue.y = v1.y * v2.y;
	vValue.z = v1.z * v2.z;
}; 


// COn retorno
point_3D Suma(point_3D v1, point_3D v2)
{
	point_3D temp;
	Suma(temp,v1,v2);
	return temp;
}; 

point_3D Resta(point_3D v1, point_3D v2)
{
	point_3D temp;
	Resta(temp,v1,v2);
	return temp;
}; 

// Othes

float Distancia(point_3D v1 ,point_3D v2)
{
	point_3D temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	temp.z = v1.z - v2.z; 
	return LengthVector(temp);
};

float NormalizeR(point_3D &vValue)
{
    float lengthsq = sqrt(vValue.x*vValue.x + vValue.y*vValue.y + vValue.z*vValue.z);

	if (IsSmall(lengthsq))
    {
        return 0.0f;
    }
    else
    {
        vValue.x /= lengthsq;
        vValue.y /= lengthsq;
        vValue.z /= lengthsq;
    }
	return lengthsq;
};

void Cross(point_3D &vValue ,point_3D v1, point_3D v2)
{
   vValue.x=v1.y*v2.z - v1.z*v2.y;
   vValue.y=v1.z*v2.x - v1.x*v2.z;
   vValue.z=v1.x*v2.y - v1.y*v2.x;
}; 

float CrossNormalize(point_3D &vValue ,point_3D v1, point_3D v2)
{
	Cross(vValue,v1,v2);
	Limpiar(vValue);
	return NormalizeR(vValue);
}; 

float Dot(point_3D v1 ,point_3D v2)
{
	return v1.x*v2.x + v1.y*v2.y +v1.z*v2.z; 
};

float DireccionByResta(point_3D &vPRayo ,point_3D vFin,point_3D vInicio)
{
	Resta(vPRayo,vFin,vInicio);
	Limpiar(vPRayo);
	return NormalizeR(vPRayo);
};

bool Perpendicular(point_3D &vPerpendicular, point_3D vVector) // perpendicular a un vector normalizado
{
	register unsigned i,indiceAdd,indiceAddAdd;
	register float *vector,*perpendicular;

	vector = new float[3];
	perpendicular = new float[3];

	Igualar(vector,vVector);
	for(i = 0; i < 3; i++)
	{
		if(fabs(vector[i]) >= (1.0f/sqrt(3.0f)))
		{
			indiceAdd = (i == 2)? 0: (i + 1);
			indiceAddAdd = (i != 0)? (i - 1): 2;
			perpendicular[indiceAdd] = 1.0f;
			perpendicular[indiceAddAdd] = 0.0f;
			perpendicular[i] = -vector[indiceAdd] / vector[i];

			Igualar(vPerpendicular,perpendicular);
			NormalizeR(vPerpendicular);
			return true;
		}
	}
	return false;
};

void Ray(point_3D &vPRayo ,point_3D vInicio,point_3D vDireccion, float vAlfa)
{
	Amplificar(vPRayo,vAlfa,vDireccion);
	Suma(vPRayo,vInicio,vPRayo);
};

void Ray(point_3D &vPRayo ,point_3D vInicio, float vAlfa, point_3D vDireccion)
{
	Amplificar(vPRayo,vAlfa,vDireccion);
	Suma(vPRayo,vInicio,vPRayo);
};

float 	DistanciaAPlano(point_3D vNormalT , point_3D vDirMov, point_3D vPuntoPlano,point_3D vPuntoInteres)
{
	// Ecuacion Plano Triangulo N*X+d=0
	//d1 = - Dot(normalT,vTriangulo[0]);
	if(!IsSmall(Dot(vNormalT,vDirMov)))
	{
		return  -(- Dot(vNormalT,vPuntoPlano) + Dot(vNormalT,vPuntoInteres)) / Dot(vNormalT,vDirMov);
	}
	return BIGGESTVALUE;
};

float 	DistanciaAPlanoN(point_3D vNormalT , point_3D vPuntoPlano,point_3D vPuntoInteres)
{
	// Ecuacion Plano Triangulo N*X+d=0
	//d1 = - Dot(normalT,vTriangulo[0]);
	return  -(- Dot(vNormalT,vPuntoPlano) + Dot(vNormalT,vPuntoInteres));
};


bool IsPointInTriangle(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3])
{
	float A[3];
	short i0,i1;
	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A[0]=fabs(vNormal.x);
	A[1]=fabs(vNormal.y);
	A[2]=fabs(vNormal.z);
	if(A[0]>A[1])
	{
		if(A[0]>A[2])  
		{
			i0=1;      /* A[0] is greatest */
			i1=2;
		}
		else
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if(A[2]>A[1])
		{
			i0=0;      /* A[2] is greatest */
			i1=1;                                           
		}
		else
		{
			i0=0;      /* A[1] is greatest */
			i1=2;
		}
	}               
	
	float point[] = {vPunto.x,vPunto.y,vPunto.z};
	float t1[] = {vTriangulo[0].x,vTriangulo[0].y,vTriangulo[0].z};
	float t2[] = {vTriangulo[1].x,vTriangulo[1].y,vTriangulo[1].z};
	float t3[] = {vTriangulo[2].x,vTriangulo[2].y,vTriangulo[2].z};
	POINT_IN_TRI(point,t1,t2,t3);
	return false;
};

bool IsPointInTriangleS(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]) // Calculo algebraico
{
	float a,b,c,pA,pB,pC;
	float semiPT,semiPPAB,semiPPAC,semiPPBC;
	float surfaceT,surfacePAB,surfacePAC,surfacePBC;
	point_3D temp;

	Resta(temp,vTriangulo[2],vTriangulo[1]);
	a = LengthVector(temp);
	Resta(temp,vTriangulo[0],vTriangulo[2]);
	b = LengthVector(temp);
	Resta(temp,vTriangulo[1],vTriangulo[0]);
	c = LengthVector(temp);

	Resta(temp,vTriangulo[0],vPunto);
	pA = LengthVector(temp);
	Resta(temp,vTriangulo[1],vPunto);
	pB = LengthVector(temp);
	Resta(temp,vTriangulo[2],vPunto);
	pC = LengthVector(temp);

	semiPT		= (a + b + c)/2.0f;
	semiPPAB	= (pA + pB + c)/2.0f;
	semiPPAC	= (pA + b + pC)/2.0f;
	semiPPBC	= (a + pB + pC)/2.0f;

	surfaceT	= sqrtf(semiPT*(semiPT-a)*(semiPT-b)*(semiPT-c));
	surfacePAB	= sqrtf(semiPPAB*(semiPPAB-pA)*(semiPPAB-pB)*(semiPPAB-c));
	surfacePAC	= sqrtf(semiPPAC*(semiPPAC-pA)*(semiPPAC-b)*(semiPPAC-pC));
	surfacePBC	= sqrtf(semiPPBC*(semiPPBC-a)*(semiPPBC-pB)*(semiPPBC-pC));
	
	if(IsSmall(surfaceT - surfacePAB - surfacePAC - surfacePBC))
		return true;

	return false;
};

bool IsPointInTriangleN(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]) // Calculo algebraico
{
	point_3D dir10,dir21,dir02,temp;

	DireccionByResta(temp,vTriangulo[1],vTriangulo[0]);
	CrossNormalize(dir10,temp,vNormal);
	DireccionByResta(temp,vTriangulo[2],vTriangulo[1]);
	CrossNormalize(dir21,temp,vNormal);
	DireccionByResta(temp,vTriangulo[0],vTriangulo[2]);
	CrossNormalize(dir02,temp,vNormal);

	Zero(temp);
	Suma(temp,temp,vTriangulo[0]);
	Suma(temp,temp,vTriangulo[1]);
	Suma(temp,temp,vTriangulo[2]);
	Amplificar(temp,(1.0f/3.0f));

	if(DistanciaAPlanoN(dir10,vTriangulo[0],temp) < 0.0f) // Hcaer que las tres normales apunten al exterior del triangulo
	{
		Amplificar(dir10,-1.0f);
		Amplificar(dir21,-1.0f);
		Amplificar(dir02,-1.0f);
	}

	if( (DistanciaAPlanoN(dir10,vTriangulo[0],vPunto) >= 0.0f)
		&& (DistanciaAPlanoN(dir21,vTriangulo[1],vPunto) >= 0.0f)
		&& (DistanciaAPlanoN(dir02,vTriangulo[2],vPunto) >= 0.0f))
	{
		return true;
	}
	return false;
};

bool IsPointInTriangleA(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3],int vTipoAnalisis) // Calculo algebraico
{
	register unsigned i;
	register float alfa,ampliacion = 10.0f;
	point_3D dir,temp;

	Zero(temp);
	for(i = 0; i < 3; i++)
	{
		Suma(temp,temp,vTriangulo[i]);
	}
	Amplificar(temp,(1.0f/3.0f)); // Baricentro

	// Expandir universo
	for(i = 0; i < 3; i++)
	{
		alfa = DireccionByResta(dir,vTriangulo[i],temp);
		Ray(vTriangulo[i],temp,alfa * ampliacion,dir);
	}
	alfa = DireccionByResta(dir,vPunto,temp);
	Ray(vPunto,temp,alfa * ampliacion,dir);

	switch(vTipoAnalisis)
	{
		case 0:
			return IsPointInTriangle(vNormal,vPunto,vTriangulo); // Daniel Test
			break;
		case 1:
			return IsPointInTriangleS(vNormal,vPunto,vTriangulo); // Surface Test
			break;
		case 2:
			return IsPointInTriangleN(vNormal,vPunto,vTriangulo); // Normal Segments Test
			break;
		default:
			return IsPointInTriangle(vNormal,vPunto,vTriangulo);
			break;
	}
};

bool IsPointInTriangleB(point_3D vNormal,point_3D vPunto,point_3D vTriangulo[3]) // Calculo Bizarro
{
	if( IsSmall(Distancia(vTriangulo[0],vPunto)) || IsSmall(Distancia(vTriangulo[1],vPunto)) || IsSmall(Distancia(vTriangulo[2],vPunto)))
	{
		return true;	
	}

	register unsigned i;
	register float radioMax;
	point_3D temp;

	Zero(temp);
	for(i = 0; i < 3; i++)
	{
		Suma(temp,temp,vTriangulo[i]);
	}
	Amplificar(temp,(1.0f/3.0f)); // Baricentro

	// Radio maximo
	radioMax = 0.0f;
	for(i = 0; i < 3; i++)
	{
		if(Distancia(temp,vTriangulo[i]) > radioMax)
		{
			radioMax = Distancia(temp,vTriangulo[i]);
		}
	}

	if(Distancia(vPunto,temp) > radioMax)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void OrdenarValoresMinMax(float* vValue,unsigned vDimension)
{
	register unsigned i,j,k;
	register float temp;

	for(i=0; i< vDimension; i++)
	{
		for(j=0; j< i; j++)
		{
			if(vValue[i] < vValue[j])// muere
			{
				temp = vValue[i];
				for(k = i; k > j ; k--)
				{
					vValue[k] = vValue[k-1];
				}
				vValue[j] = temp;
			}
		}
	}
};

int InterseccionRayoEsfera(float *vAlfas, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio) // Indica si hay colision entre un rayo y una esfera... retorna el numero de contactos 
{
	return 0;
};

bool InterseccionClosedRayoEsfera(float *vAlfa, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio) // Indica si hay colision valida (alfa > 0.0f) y entrega el valor de alfa mas cercano al origen del rayo
{
	register bool valido = false;
	register unsigned i;
	float *alfas = new float[2];
	InterseccionRayoEsfera(alfas,vOrigen,vDireccion,vCentro,vRadio);

	vAlfa[0] = BIGGESTVALUE;
	for(i = 0; i < 2; i++)
	{
		if(alfas[i] > 0.0f && alfas[i] < vAlfa[0])
		{
			valido = true;
			vAlfa[0] = alfas[i];
		}
	}

	return valido;
};

bool InterseccionFarestRayoEsfera(float *vAlfa, point_3D vOrigen, point_3D vDireccion, point_3D vCentro, float vRadio) // Indica si hay colision valida (alfa > 0.0f) y entrega el valor de alfa mas lejano al origen del rayo
{
	register bool valido = false;
	register unsigned i;
	float *alfas = new float[2];
	InterseccionRayoEsfera(alfas,vOrigen,vDireccion,vCentro,vRadio);

	vAlfa[0] = 0.0f;
	for(i = 0; i < 2; i++)
	{
		if(alfas[i] > vAlfa[0])
		{
			valido = true;
			vAlfa[0] = alfas[i];
		}
	}

	return valido;
};


double	InvertGenericLU(int n,double **vResult, double **vMatriz) 
{
	int i, j, k;

	double factor;

	double **L, *D, *X;
	
	X = new double [n]; D = new double [n];

	L = new double* [n];
	
	for (j = 0; j < n; j++) 

		L[j] = new double [n];

	for (k = 0; k < n - 1; k++) 
	{
		
		for (i = k+1; i < n;  i++) 
		{

			factor = vMatriz[i][k]/vMatriz[k][k]; 

			for (j = k+1; j < n + 1; j++) 
			{

				vMatriz[i][j] = vMatriz[i][j] - factor * vMatriz[k][j];

			}
		}
	}

	double determ = 1.;

	for (i = 0; i < n; i++) 
	{
		determ = determ * vMatriz[i][i];

	}

	if (determ != 0) 
	{
        for (i = 0; i < n; i++) 
		{
               for (j = 0; j < n; j++) 
			   {
                  if (i > j) 
				  {
                      L[i][j] = vMatriz[i][j]/ vMatriz[j][j];

                       vMatriz[i][j] = 0;
                   }
               }
        }

	    for (i = 0; i < n; i++) 
		{
            for (j = 0; j < n; j++)
			{
                  L[j][j] = 1;
            }
        }

		// Implementación de la rutina para el cálculo de la inversa


		for (k = 0; k < n; k++) 
		{
			// Esta rutina inicializa los L[i][n] para ser utilizados con la matriz L
	        for (i = 0; i < n; i++)
			{
               if (i == k) L[i][n] = 1;
               else  L[i][n] = 0;
            }

			// Esta función implementa la sustitución hacia adelante con la matriz L y los L[i][n]
			// que produce la rutina anterior

			double sum;

				
			  
			D[0] = L[0][n];

			for (i = 1; i < n; i++)
			{
		       sum = 0;
	
			   for (j = 0; j < i; j++)
			   {
		            sum = sum + L[i][j]*D[j];
		       }
		        D[i] = L[i][n] - sum;
		    }

			// Esta rutina asigna los D[i] que produce forward para ser utilizados con la matriz U

			for (i = 0; i < n; i++)
			{
	           vMatriz[i][n] = D[i];
			}

			// Rutina que aplica la sustitución hacia atras

			X[n-1] =  vMatriz[n-1][n]/ vMatriz[n-1][n-1];

			// Determinación de las raíces restantes
			for (i = n - 2; i > -1; i--)
			{
				sum = 0;

				for (j = i+1; j < n; j++)
				{
					  sum = sum +  vMatriz[i][j]*X[j];

				}
		         X[i] = ( vMatriz[i][n] - sum)/ vMatriz[i][i];

			}

			// Esta rutina asigna los X[i] que produce Sustituir como los elementos de la matriz inversa
			for (i = 0; i < n; i++)
			{
			     vResult[i][k] = X[i];
			}

		}

	}


	delete L, D, X;
	return determ;
};





bool isSegmentsIntersected(point_2D vX_Init, point_2D vX_End, point_2D vY_Init, point_2D vY_End,float *t,float*u) {
	// iM REQUIRE t
	point_2D CmP(vY_Init.x() - vX_Init.x(), vY_Init.y() - vX_Init.y());
	point_2D r(vX_End.x() - vX_Init.x(), vX_End.y() - vX_Init.y());
	point_2D s(vY_End.x() - vY_Init.x(), vY_End.y() - vY_Init.y());
 
	float CmPxr		= CmP.x() * r.y() - CmP.y() * r.x();
	float CmPxs		= CmP.x() * s.y() - CmP.y() * s.x();
	float rxs		= r.x() * s.y() - r.y() * s.x();

	*t = 1.0f;
	*u = 1.0f;
	if (CmPxr == 0.0f) {
	// Lines are collinear, and so intersect if they have any overlap
		return false;
		return ((vY_Init.x() - vX_Init.x() < 0.0f) != (vY_Init.x() - vX_End.x() < 0.0f))
			|| ((vY_Init.y() - vX_Init.y() < 0.0f) != (vY_Init.y() - vX_End.y() < 0.0f));
	}
 
	if (rxs == 0.0f)
		return false; // Lines are parallel.
 
	float rxsr = 1.0f / rxs;
	*t = CmPxs * rxsr;
	*u = CmPxr * rxsr;
 
	return (*t >= 0.0f) && (*t <= 1.0f) && (*u >= 0.0f) && (*u <= 1.0f);
}
