#include "cellWM.h"

CellWM::CellWM()
{
	_bActive		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	_nCont			= 100;
	_paramNumFilo   = 4;

	_radR0 			= 15;
	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0;
					
		_paramA[i]			= 5.0f;
		_paramW[i]			= 0.5f;
		_paramGamma[i]		= 1.0f;
		_paramSigma[i]		= 1.0f;
	}

	_paramV0			= 1.0f;

	_polyMembrane		= new point_2D[_nCont];

	_bEVL_Interaction		= false;
	_bStochasticMov			= false;
	_bDFCsInteraction		= false;
	_bOffSpring				= false;

	_adhesionDistance		= 1.0f;
};

CellWM::~CellWM() {

};

void	CellWM::InitCell(void){
	_bActive		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	_radR0 			= 15;
	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0;

		_paramA[i]			= 2.5f;
		_paramW[i]			= 1.5f;
		_paramGamma[i]		= 1.0f;
		_paramSigma[i]		= 1.0f;
	}

	_paramV0			= 0.01f;

	CreateContour();
};
void	CellWM::InitCell(point_2D vPos, float vRadio){
	_bActive		= false;

	_pos			= point_2D(vPos.x,vPos.y);
	_vel			= point_2D(0.0,0.0);

	_radR0 			= vRadio;
	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]		= -(PI)* ( (float(rand())/float(RAND_MAX)) - 1.0f);
		_paramA[i]				= 0.3f* vRadio * ( (float(rand())/float(RAND_MAX)));
		_paramW[i]				= 0.8f * ( (float(rand())/float(RAND_MAX)));
		_paramGamma[i]			= 1.5f * ( (float(rand())/float(RAND_MAX)));
		_paramSigma[i]			= 1.8f * ( (float(rand())/float(RAND_MAX)));
	}

	_paramV0				= 0.01f;

	_bEVL_Interaction		= false;
	_bStochasticMov			= false;
	_bDFCsInteraction		= false;
	_bOffSpring				= false;

	_adhesionDistance		= 1.0f;

	CreateContour();
};

float	CellWM::FunP(float vValue, int index) {
	float dummyAngleDist = vValue - _angleThetha0[index];
	if( dummyAngleDist > PI){
		dummyAngleDist = dummyAngleDist - 2.0f*PI;
	} 
	if( dummyAngleDist < -PI){
		dummyAngleDist = dummyAngleDist + 2.0f*PI;
	}
	return _radR0 + _paramA[index] * exp(-pow(dummyAngleDist,2)/(2 * _paramW[index] * _paramW[index]));
};

void	CellWM::CreateContour(void) {
	register float angle	= 0.0f;
	register float radius=0.0f, tempR;
		
	for (register int i = 0; i < _nCont; i++) {
		radius  = 0.0f;
		angle	= i*(2*PI/_nCont) - PI;

		for (register int j = 0; j < _paramNumFilo; j++)
		{
			tempR	= FunP(angle,j);
			if(tempR > radius)
			{
				radius = tempR;
			}
		}
		point_2D newp(_pos.x + radius*cos(angle), _pos.y + radius*sin(angle));
		_polyMembrane[i].SetPoint(newp);
	}
};

	// Base de Calculo
void	CellWM::UdpdateAngle(void) {
	for(register int i = 0; i < _paramNumFilo; i++)
	{
		if(_bStochasticMov){
			_angleThetha0[i] = _angleThetha0[i] + _paramSigma[i] * (PI/4)* ( (float(rand())/float(RAND_MAX)) - 0.5f);
		}
		else{
			_angleThetha0[i] = _angleThetha0[i];
		}

		if( _angleThetha0[i] > PI){
			_angleThetha0[i] = _angleThetha0[i] - 2.0f*PI;
		} 
		if( _angleThetha0[i] < -PI){
			_angleThetha0[i] = _angleThetha0[i] + 2.0f*PI;
		} 
	}
};

void	CellWM::UpdateShape(void) {
	register int   idxP;
	register float angle		= 0.0f;
	register float radius		= 0.0f;
	register float tempRadius	= 0.0f;
	register float tempX,tempY;

	for(idxP = 0;idxP < _nCont; idxP++) {
		point_2D pp(_polyMembrane[idxP]);
		angle	= idxP*(2*PI/_nCont) - PI;
		tempX	= pp.x - _pos.x;
		tempY	= pp.y - _pos.y;

		tempRadius = _radR0;
		radius	= sqrt(tempX*tempX + tempY*tempY);

		for(register int j = 0; j < _paramNumFilo; j++)
		{
			if(radius - _paramGamma[j] * (radius - FunP(angle,j)) > tempRadius)
			{
				tempRadius = radius - _paramGamma[j] * (radius - FunP(angle,j)); 
			}
		}
		radius	= tempRadius; 
		_polyMembrane[idxP] = point_2D(_pos.x + radius*cos(angle), _pos.y + radius*sin(angle));
	}

};


float	CellWM::EstimateV(void){
	register float velocity = 0.0f;
	register float angle	= 0.0f;
	register float radius	= 0.0f;
	register float tempX,tempY;

	// By definition
	//for (register int i = 0; i < _nCont; i++) {
	//	angle	= i*(2*PI/_nCont) - PI;
	//	radius	= FunP(angle)  - _radR0; 
	//	point_2D newp(_vel.x + radius*cos(angle), _vel.y + radius*sin(angle));
	//	_vel = newp;
	//}

	// Considering overlaping
	point_2D newp(0.0f,0.0f);
	_vel = newp;
	register int idxP = 0;

	for(idxP = 0;idxP < _nCont; idxP++) {
		point_2D pp(_polyMembrane[idxP]);
		angle	= idxP*(2*PI/_nCont) - PI;
		tempX	= pp.x - _pos.x;
		tempY	= pp.y - _pos.y;

		radius	= sqrt(tempX*tempX + tempY*tempY);
		point_2D newp(_vel.x + radius*cos(angle), _vel.y + radius*sin(angle));
		_vel = newp;
	}
	_vel.x = _paramV0 * _vel.x;
	_vel.y = _paramV0 * _vel.y;
	return _paramV0 * velocity;
};

void	CellWM::UpdatePos(void) {
	point_2D tempV(_vel.x,_vel.y);
	point_2D tempP(_pos.x,_pos.y);

	point_2D newP(tempP.x + tempV.x, tempP.y + tempV.y);
	_pos = newP;
};


void	CellWM::FixOverlap(CellWM *vValue, float vFactor) {
	register int idxPi, idxPj, idxPk;
	float t, u, radJ,radK;
	bool fInter = false;
	// Considering overlaping

	point_2D centerI(_pos);
	point_2D centerJ((vValue)->_pos);

	point_2D pI,pJ,pK,pZ;
	point_2D newNI,newNJ,newNK,newNZ;
	for(idxPi = 0;idxPi < _nCont; idxPi++) {
		pI	  = point_2D(_polyMembrane[idxPi]);
		newNI = point_2D(pI.x - centerI.x, pI.y - centerI.y);

		for(idxPj = 0;idxPj < _nCont; idxPj++) {
			if(idxPj == _nCont - 1){
				idxPk = 0;
			}
			else{
				idxPk = idxPj+1;
			}

	        pJ = point_2D((vValue)->_polyMembrane[idxPj]);
			pK = point_2D((vValue)->_polyMembrane[idxPk]);

			fInter = isSegmentsIntersected(centerI, pI, pJ, pK ,&t ,&u);

			if(fInter) {
				//t = t + (1-t)*0.5f= (2t+(1-t) )/2= (1+t)/2	    // for half intersection  == vFactor = 1
			    //t = t				                        	// for full separation ==> vFactor = 0
				t = t + vFactor * (1-t)*0.5f;  
				_polyMembrane[idxPi].SetPoint(centerI.x + t*newNI.x, centerI.y + t*newNI.y);
			}
		}
	}



};


void	CellWM::SimulationStepPre(void) {
	EstimateV();
	UpdatePos();
};

void	CellWM::SimulationStepPost(void) {
	UdpdateAngle();
	UpdateShape();
};

