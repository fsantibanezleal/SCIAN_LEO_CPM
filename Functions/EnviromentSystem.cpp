#include "EnvironmentSystem.h"

EnvironmentSystem::EnvironmentSystem()
{
	_StepsProliferationCurrent=0;
	_posProliferation = -1;
	_numDFCActiveAgentsWanted = 10;
	_stepsProliferacion    = 100;
	_vProliferation[0]= 0.0f;
	_vProliferation[1]= 0.0f;
	_vProliferation[2]= 0.1f;
	_vProliferation[3]= 0.0f;
	_vProliferation[4]= 0.2f;
	_vProliferation[5]= 0.0f;
	_vProliferation[6]= 0.0f;
	_vProliferation[7]= 0.0;

	_dataEVL_IncludedCampos		= 2;
	_stepsMovDEB_margin			= 10;
	_actualStepsMovDEB_margin	= 10;
	_stepsMovEVL_margin			= 10;
	_actualStepsMovEVL_margin	= 10;

	_num_DFC_EVL_AdhesionAnterior	 = 0;
	_num_DFC_EVL_AdhesionPosterior	 = 0;
	_limit_DFC_EVL_PostAdhesion = 11;
	_limit_DFC_EVL_AntAdhesion = 7;
	_dataEVL_StepDistEVL_Current = 0;
	_useEVLInteractionAdhesion = false;
	_useDEBAttraction = false;
	_useEVLInteraction= false;
	_factorDFC_DEB_Action = 1.0f;
	_dataEVL_FactorEVL	  = 1.0f;

	_dataEVL_quantityKind	= 0; // 0 == todo... 1 == seleccionadas
	_dataEVL_FieldKind		= 0; // 0 == puntual... 1 == linea.... 2 == distribuido

	_dataEVL_PosY			= 500;
	_dataEVL_LineStep		= 100;
	_dataEVL_currentLineStep = 100;
	_dataEVL_LineWidth		= 600;
	_dataEVL_ChangeStepsEVLDist = 100;


	float maximoTemp = 500;
	SetFull(_minEnvironmentSystem,0.0f,0.0f,0.0f);
	SetFull(_posMarginDEB,0.0f,0.0f,0.0f);
	SetFull(_maxEnvironmentSystem,maximoTemp,maximoTemp,0);

	_dimX	= int(_maxEnvironmentSystem.x - _minEnvironmentSystem.x);
	_dimY	= int(_maxEnvironmentSystem.y - _minEnvironmentSystem.y);
	_dimZ	= int(_maxEnvironmentSystem.z - _minEnvironmentSystem.z);

	int dfcTemp = int(maximoTemp / 4);
	SetFull(_minInitDFC,float(dfcTemp),0.0f,0.0f);
	SetFull(_maxInitDFC,3.0f*float(dfcTemp),maximoTemp/2,0.0f);

	SetFull(_color,0,0,0);


	_busy = 0;


	_tipoNodoInGrid = new int*[_dimX];
	_numNodoInGrid  = new int*[_dimX];

	for(register int i = 0; i < _dimX; i++)
	{
		_tipoNodoInGrid[i]	= new int(_dimY);
		_numNodoInGrid[i]	= new int(_dimY);
	}
};

EnvironmentSystem::~EnvironmentSystem()
{

};


void	EnvironmentSystem::Initiate(void)
{

};

void	EnvironmentSystem::Update(void)
{
	if(_useDEBAttraction)
	{
		_actualStepsMovDEB_margin--;
		if(_actualStepsMovDEB_margin < 1)
		{
			_actualStepsMovDEB_margin = _stepsMovDEB_margin;
			_posMarginDEB.y++;
		}
	}

	if(_useEVLInteraction)
	{
		_actualStepsMovEVL_margin--;
		if(_actualStepsMovEVL_margin < 1)
		{
			_actualStepsMovEVL_margin = _stepsMovEVL_margin;
			_dataEVL_PosY++;
		}
	}
};