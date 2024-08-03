#include "cellWM.h"

CellWM::CellWM()
{
	_fBusy			= false;
	_fActive		= false;
	_bRunning		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	_nCont			= 100;
	_paramNumFilo   = 4;

	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0;
		_radR0[i]			= 0;
					
		_paramA[i]			= 5.0f;
		_paramW[i]			= 0.5f;
		_paramGamma[i]		= 1.0f;
		_paramSigma[i]		= 1.0f;
	}
	_radR0[0]			= 10;
	_radR0[1]			= 5;
	_angleThetha0[0]	= -PI/2.0f;
	_angleThetha0[1]	= -PI/2.0f;


	_paramV0			= 1.0f;

	_polyMembrane		= new point_2D[_nCont];
};

CellWM::~CellWM() {

};

void	CellWM::InitCell(void){
	_fBusy			= false;
	_fActive		= false;
	_bRunning		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0;
		_radR0[i]			= 0;
					
		_paramA[i]			= 2.5f;
		_paramW[i]			= 1.5f;
		_paramGamma[i]		= 1.0f;
		_paramSigma[i]		= 1.0f;
	}
	_radR0[0]			= 5;
	_radR0[1]			= 5;
	_angleThetha0[0]	= -PI/2.0f;
	_angleThetha0[1]	= -PI/2.0f;


	_paramV0			= 0.01f;

	CreateContour();
};
void	CellWM::InitCell(point_2D vPos, float vRadio){
	_fBusy			= false;
	_fActive		= false;
	_bRunning		= false;

	_pos			= point_2D(vPos.x,vPos.y);
	_vel			= point_2D(0.0,0.0);


	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_radR0[i]				= vRadio;
					
		_angleThetha0[i]		= -(PI)* ( (float(rand())/float(RAND_MAX)) - 1.0f);
		_paramA[i]				= vRadio * ( (float(rand())/float(RAND_MAX))) + 0.5f * vRadio;
		_paramW[i]				= 0.8f * ( (float(rand())/float(RAND_MAX)));
		_paramGamma[i]			= 1.5f * ( (float(rand())/float(RAND_MAX)));
		_paramSigma[i]			= 1.8f * ( (float(rand())/float(RAND_MAX)));
	}

	_paramV0				= 0.01f;

	_adhesionToEVL			= false;
	_perturbationRandom		= false;
	_freeIssue				= false;
	_friendAttraction		= false;
	_StepsforChange			= 10;
	_ratioFriendAttraction	= 1.0f;

	_persistence			= rand()%(_StepsforChange) + 1;

	_offSpring				= false;

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
	return _radR0[index] + _paramA[index] * exp(-pow(dummyAngleDist,2)/(2 * _paramW[index] * _paramW[index]));
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
		_angleThetha0[i] = _angleThetha0[i] + _paramSigma[i] * (PI/4)* ( (float(rand())/float(RAND_MAX)) - 0.5f);
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

		tempRadius = _radR0[0];
		radius	= sqrt(tempX*tempX + tempY*tempY);

		for(register int j = 0; j < _paramNumFilo; j++)
		{
			// Check paper FASL FIX . Im not sure about it.
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


void	CellWM::FixOverlap(CellWM *vValue) {
	register int idxPi, idxPj;
	register float radius;
	float t, u;
	bool fInter = false;
	// Considering overlaping

	point_2D centerI(_pos.x,_pos.y);
	point_2D centerJ((vValue)->_pos.x,(vValue)->_pos.y);

	for(idxPi = 0;idxPi < _nCont; idxPi++) {
		point_2D pI(_polyMembrane[idxPi]);
		for(idxPj = 0;idxPj < _nCont; idxPj++) {
			point_2D pJ((vValue)->_polyMembrane[idxPj]);

			fInter = isSegmentsIntersected(centerI, pI, centerJ, pJ ,&t ,&u);

			if(fInter) {
				point_2D newNI(pI.x - centerI.x, pI.y - centerI.y);
				point_2D newNJ(pJ.x - centerJ.x, pJ.y - centerJ.y);

				radius = sqrt(newNI.x*newNI.x + newNI.y*newNI.y);
				newNI  = point_2D(newNI.x/radius , newNI.y/radius);
				radius = sqrt(newNJ.x*newNJ.x + newNJ.y*newNJ.y);
				newNJ  = point_2D(newNJ.x/radius , newNJ.y/radius);

				_polyMembrane[idxPi]		   = point_2D(centerI.x + 1*newNI.x, centerI.y + 1*newNI.y);
				(vValue)->_polyMembrane[idxPj] = point_2D(centerJ.x + 1*newNJ.x, centerJ.y + 1*newNJ.y);

			}
		}
	}



};


void	CellWM::SimulationStep(void) {
	UdpdateAngle();
	UpdateShape();

	EstimateV();
	UpdatePos();

};
