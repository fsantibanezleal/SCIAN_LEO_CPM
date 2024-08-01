#include "cellWM.h"

CellWM::CellWM()
{
	_fBusy			= false;
	_fActive		= false;
	_fRunning		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	_nCont			= 100;
	_paramNumFilo   = 1;

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


};

CellWM::~CellWM() {

};

void	CellWM::InitCell(void){
	_fBusy			= false;
	_fActive		= false;
	_fRunning		= false;

	_pos			= point_2D(0.0,0.0);
	_vel			= point_2D(0.0,0.0);

	_nCont			= 100;
	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0;
		_radR0[i]			= 0;
					
		_paramA[i]			= 1.5f;
		_paramW[i]			= 0.5f;
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
	_fRunning		= false;

	_pos			= point_2D(vPos.x(),vPos.y());
	_vel			= point_2D(0.0,0.0);

	_nCont			= 100;

	for(register int i = 0; i <_paramNumFilo; i++)
	{
		_angleThetha0[i]	= 0.0f;
		_radR0[i]			= vRadio/2;
					
		_paramA[i]			= 0.0f;
		_paramW[i]			= 1.5f;
		_paramGamma[i]		= 0.5f;
		_paramSigma[i]		= 0.5f;
	}
	_angleThetha0[0]	= PI/2.0f + 0.1f;
	//_angleThetha0[1]	= PI/2.0f - 0.1f;

	_paramA[0]			= vRadio/2;
	//_paramA[1]			= vRadio/2;

	_paramGamma[0]		= 1.0f;
	_paramSigma[0]		= 1.0f;

	_paramV0			= 0.01f;

	_adhesionToEVL	  = false;
	_perturbacionAzar = false;
	_liberarEstancado = false;
	_friendAttraction = false;
	_StepsforChange   = 10;
	_ratioFriendAttraction = 1.0f;

	_persistence	  = rand()%(_StepsforChange) + 1;

	_hija			  = false;

	CreateContour();
};

float	CellWM::FunP(float vValue, int index) {

	return _radR0[index] + _paramA[index] * exp(-pow(vValue - _angleThetha0[index],2)/(2 * _paramW[index] * _paramW[index]));
};

void	CellWM::CreateContour(void) {
	register float angle	= 0.0f;
	register float radius=0.0f, tempR;
		
	for (register int i = 0; i < _nCont; i++) {
		angle	= i*(2*PI/_nCont) - PI;

		for (register int j = 0; j < _paramNumFilo; j++)
		{
			tempR	= FunP(angle,j);
			if(tempR > radius)
			{
				radius = tempR;
			}
		}
		point_2D newp(_pos.x() + radius*cos(angle), _pos.y() + radius*sin(angle));
		append(_polyMembrane.outer(), newp);
	}
};

	// Base de Calculo
void	CellWM::UdpdateAngle(void) {
	for(register int i = 0; i < _paramNumFilo; i++)
	{
		_angleThetha0[i] = _angleThetha0[i] + _paramSigma[i] * (PI/4)* ( (float(rand())/float(RAND_MAX)) - 0.5f);
		if( _angleThetha0[i] > PI){
			_angleThetha0[i] = _angleThetha0[i] - PI;
		} 
		if( _angleThetha0[i] < -PI){
			_angleThetha0[i] = _angleThetha0[i] + PI;
		} 
	}
};

void	CellWM::UpdateShape(void) {
	register float angle		= 0.0f;
	register float radius		= 0.0f;
	register float tempRadius	= 0.0f;
	register float tempX,tempY;

	register int i = 0;
	BOOST_FOREACH(point_2D const& pp, _polyMembrane.outer()) {
		angle	= i*(2*PI/_nCont) - PI;
		tempX	= pp.x() - _pos.x();
		tempY	= pp.y() - _pos.y();

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
		(_polyMembrane.outer())[i] = point_2D(_pos.x() + radius*cos(angle), _pos.y() + radius*sin(angle));
		
		i++;
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
	//	point_2D newp(_vel.x() + radius*cos(angle), _vel.y() + radius*sin(angle));
	//	_vel = newp;
	//}

	// Considering overlaping
	point_2D newp(0.0f,0.0f);
	_vel = newp;
	register int i = 0;

	BOOST_FOREACH(point_2D const& pp, _polyMembrane.outer()) {
		angle	= i*(2*PI/_nCont) - PI;
		tempX	= pp.x() - _pos.x();
		tempY	= pp.y() - _pos.y();

		radius	= sqrt(tempX*tempX + tempY*tempY);
		point_2D newp(_vel.x() + radius*cos(angle), _vel.y() + radius*sin(angle));
		_vel = newp;
		i++;
	}
	_vel.x(_paramV0 * _vel.x());
	_vel.y(_paramV0 * _vel.y());
	return _paramV0 * velocity;
};

void	CellWM::UpdatePos(void) {
	point_2D tempV(_vel.x(),_vel.y());
	point_2D tempP(_pos.x(),_pos.y());

	point_2D newP(tempP.x() + tempV.x(), tempP.y() + tempV.y());
	_pos = newP;
};


void	CellWM::FixOverlap(CellWM *vValue) {
	register int i,j;
	register float radius;
	float t, u;
	bool fInter = false;
	// Considering overlaping

	point_2D centerI(_pos.x(),_pos.y());
	point_2D centerJ((vValue)->_pos.x(),(vValue)->_pos.y());

	i = 0;
	BOOST_FOREACH(point_2D const& pI, _polyMembrane.outer()) {
		j = 0;
		BOOST_FOREACH(point_2D const& pJ, (vValue)->_polyMembrane.outer()) {

			fInter = isSegmentsIntersected(centerI, pI, centerJ, pJ ,&t ,&u);

			if(fInter) {
				point_2D newNI(pI.x() - centerI.x(), pI.y() - centerI.y());
				radius = sqrt(newNI.x()*newNI.x() + newNI.y()*newNI.y());

				point_2D newNJ(pJ.x() - centerJ.x(), pJ.y() - centerJ.y());

				//newN = point_2D(newN.x()/radius , newN.y()/radius);

				(_polyMembrane.outer())[i]           = point_2D(centerI.x() + t*newNI.x(), centerI.y() + t*newNI.y());
				((vValue)->_polyMembrane.outer())[j] = point_2D(centerJ.x() + u*newNJ.x(), centerJ.y() + u*newNJ.y());

				//(_polyMembrane.outer())[i] = point_2D(centerI.x() + t*newN.x(), centerI.y() + t*newN.y());
			}
			j++;
		}
		i++;
	}



};


void	CellWM::SimulationStep(void) {
	UdpdateAngle();
	UpdateShape();

	EstimateV();
	UpdatePos();

};
