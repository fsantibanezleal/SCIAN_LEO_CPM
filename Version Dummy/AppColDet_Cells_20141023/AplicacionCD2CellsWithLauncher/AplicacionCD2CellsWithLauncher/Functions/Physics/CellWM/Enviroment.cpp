#include "Enviroment.h"

Enviroment::Enviroment()
{
	_actualStepsProliferacion=0;
	_posProliferation = -1;
	_numDFCActiveAgentsWanted = 10;
	_stepsProliferacion    = 100;
	_vectorProliferacion[0]= 0.0f;
	_vectorProliferacion[1]= 0.0f;
	_vectorProliferacion[2]= 0.1f;
	_vectorProliferacion[3]= 0.0f;
	_vectorProliferacion[4]= 0.2f;
	_vectorProliferacion[5]= 0.0f;
	_vectorProliferacion[6]= 0.0f;
	_vectorProliferacion[7]= 0.0;

	_dataEVL_IncludedCampos		= 2;
	_stepsMovDEB_margin			= 10;
	_actualStepsMovDEB_margin	= 10;
	_stepsMovEVL_margin			= 10;
	_actualStepsMovEVL_margin	= 10;

	_num_DFC_EVL_AdhesionAnterior	 = 0;
	_num_DFC_EVL_AdhesionPosterior	 = 0;
	_limite_DFC_EVL_AdhesionPosterior = 11;
	_limite_DFC_EVL_AdhesionAnterior = 7;
	_dataEVL_actualStepDistEVL = 0;
	_useEVLInteractionAdhesion = false;
	_useDEBAttraction = false;
	_useEVLInteraction= false;
	_factorDFC_DEB_Action = 1.0f;
	_dataEVL_FactorEVL	  = 1.0f;

	_dataEVL_tipoCantidad	= 0; // 0 == todo... 1 == seleccionadas
	_dataEVL_tipoCampo		= 0; // 0 == puntual... 1 == linea.... 2 == distribuido

	_dataEVL_PosY			= 500;
	_dataEVL_StepsLinea		= 100;
	_dataEVL_actualStepsLinea = 100;
	_dataEVL_AnchoLinea		= 600;
	_dataEVL_StepsCambioEVLDist = 100;


	float maximoTemp = 500;
	SetFull(_minEnviroment,0.0f,0.0f,0.0f);
	SetFull(_posMarginDEB,0.0f,0.0f,0.0f);
	SetFull(_maxEnviroment,maximoTemp,maximoTemp,0);

	_dimX	= int(_maxEnviroment.x - _minEnviroment.x);
	_dimY	= int(_maxEnviroment.y - _minEnviroment.y);
	_dimZ	= int(_maxEnviroment.z - _minEnviroment.z);

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

Enviroment::~Enviroment()
{

};


void	Enviroment::Inicializar(void)
{

};

void	Enviroment::Update(void)
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