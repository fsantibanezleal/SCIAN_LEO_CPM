#include "EnvironmentSystem.h"

EnvironmentSystem::EnvironmentSystem()
{
	_dimWS		= point_3D(400,350,0);

	_dimDFCs	= point_3D(250,100,0);

	_gapMargin			  = 20;

	_velMarginEVL		  = 1.5;
	_numItbyMin           = 10;

	_dataEVL_PosRef[0]       = 0.0f;
	_dataEVL_RatioAttach[0]  = 0.8f;
	_dataEVL_RatioRing[0]    = 0.75f;
	_dataEVL_PosRef[1]       = 100.0f;
	_dataEVL_RatioAttach[1]  = 0.8f;
	_dataEVL_RatioRing[1]    = 0.75f;
	_dataEVL_PosRef[2]       = 200.0f;
	_dataEVL_RatioAttach[2]  = 0.45f;
	_dataEVL_RatioRing[2]    = 0.65f;
	_dataEVL_PosRef[3]       = 300.0f;
	_dataEVL_RatioAttach[3]  = 0.0f;
	_dataEVL_RatioRing[3]    = 0.0f;
	_dataEVL_PosRef[4]       = 350.0f;
	_dataEVL_RatioAttach[4]  = 0.0f;
	_dataEVL_RatioRing[4]    = 0.0f;


	_minEnvironmentSystem = point_3D(-_dimWS.x/2.0f,-_dimWS.y/2.0f,0.0f);
	_maxEnvironmentSystem = point_3D(_dimWS.x/2.0f,_dimWS.y/2.0f,0);

	_minInitDFC			  = point_3D(-_dimDFCs.x/2.0f, _minEnvironmentSystem.y + _gapMargin ,0.0f);
	_maxInitDFC			  = point_3D(_dimDFCs.x/2.0f , _minInitDFC.y + _dimDFCs.y,0.0f);

	_posMarginDEB         = _minEnvironmentSystem.y;
	_posMarginEVL         = _maxInitDFC.y;

	_busy				  = false;
	_bShowInitialRect     = true;
	_bShowMargin_EVL      = true;
	_bShowMargin_DEB      = true;
	_bRunning             = false;

	_stateEVL		      = 0;
};

EnvironmentSystem::~EnvironmentSystem()
{

};


void	EnvironmentSystem::Initiate(void)
{

};

void	EnvironmentSystem::Reinitiate(void){

	_minEnvironmentSystem = point_3D(-_dimWS.x/2.0f,-_dimWS.y/2.0f,0.0f);
	_maxEnvironmentSystem = point_3D(_dimWS.x/2.0f,_dimWS.y/2.0f,0);

	_minInitDFC			  = point_3D(-_dimDFCs.x/2.0f, _minEnvironmentSystem.y + _gapMargin ,0.0f);
	_maxInitDFC			  = point_3D(_dimDFCs.x/2.0f , _minInitDFC.y + _dimDFCs.y,0.0f);

	_posMarginDEB         = _minEnvironmentSystem.y;
	_posMarginEVL         = _maxInitDFC.y;

	_busy				  = false;
	_bShowInitialRect     = true;
	_bShowMargin_EVL      = true;
	_bShowMargin_DEB      = true;
	_bRunning	          = false;

	_stateEVL		      = 0;
}

void	EnvironmentSystem::Update(void)
{
	if(_bRunning){
		// Use velocity and conversion criteria to estimate new EVL margin position
		_posMarginEVL = _posMarginEVL + _velMarginEVL/(float)_numItbyMin;

		// Update current state and respective values
		if(_posMarginEVL >= _dataEVL_PosRef[_stateEVL+1]){
			_stateEVL++;
		}
	
		if(_stateEVL == NUMSTAGES){
			_bRunning = false;
		}
	}
};