#include "Principal.h"

#include "SistemaControl.h"

#include "GL_EstadoInicial.h"
#include "GL_Simulacion.h"

#include <time.h>
#include <windows.h>

using namespace DinHotSys;

GL_EstadoInicial	ventanaEstadoInicialGl;
GL_Simulacion		ventanaSimulacionGl;

bool	ACTUALIZARVENTANATRAMPA = false;
bool	MOVER   = false;

int		ELEMENTOMODIFICABLE = 0;
System::Void SistemaControl::SistemaControl_Load(System::Object^  sender, System::EventArgs^  e)
{
	_numDFCs = 25;

	#ifdef TIME_TASKS
		TFrame = TRenderTotal = TRenderTrampa = TRenderMV = TCalcMaskV = 0.0f;
	#endif	

	_Borrar = false;
	_CrearMover = false;

	if(((Principal ^)_ventanaPrincipal)->mutexOGL->WaitOne()) // No realmente necesario.. aun no empiezan a correr las otras hebras
	{
		wglMakeCurrent (NULL, NULL);
		ventanaEstadoInicialGl.Crear( reinterpret_cast<HWND> (panelTab0Izq->Handle.ToPointer()),W_ONE);	
		ventanaSimulacionGl.Crear( reinterpret_cast<HWND> (panelTab0Der->Handle.ToPointer()),W_TWO);	

		wglMakeCurrent (NULL, NULL);
	
		((Principal ^)_ventanaPrincipal)->mutexOGL->ReleaseMutex();
	}

	bwControl->RunWorkerAsync();
};

System::Void SistemaControl::SistemaControl_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
{
	((Principal ^)_ventanaPrincipal)->Show();
};

System::Void SistemaControl::panelTab0Izq_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	ACTUALIZARVENTANATRAMPA = true;	 
};

System::Void SistemaControl::funcionActualizacion(void) // Actualizacion de zonas graficas de datos del simulador
{
#ifdef TIME_TASKS
	tBTimeRenderT->Text =		TRenderTotal.ToString();
	tBTimeRenderTrampas->Text = TRenderTrampa.ToString();
	tBTimeRenderMV->Text =		TRenderMV.ToString();
	//tBTimeRenderMR->Text =		((Principal ^)_ventanaPrincipal)->_ventanaMascara->TRenderMR.ToString();
	tBTimeCalcMaskV->Text =		TCalcMaskV.ToString();
	//tBTimeCalcMaskR->Text =		((Principal ^)_ventanaPrincipal)->_ventanaMascara->TCalcMaskReal.ToString();
#endif
};

System::Void SistemaControl::bwDraw_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	#ifdef TIME_TASKS
		LARGE_INTEGER t_ini, t_fin;
	#endif

	while(!bwDraw->CancellationPending)
	{
		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_ini);
		#endif

		// Funcion de pintado seleccion de contexto de dibujo actual
		if(((Principal ^)_ventanaPrincipal)->mutexOGL->WaitOne())
		{
			if(ventanaEstadoInicialGl.SetActual())
			{
				ventanaEstadoInicialGl.Resize(panelTab0Izq->Width,panelTab0Izq->Height);
				ventanaEstadoInicialGl.Renderizar(_theMatrixEnvironment,_theMatrixAgents_Static);
			}
			((Principal ^)_ventanaPrincipal)->mutexOGL->ReleaseMutex();
		}
		ACTUALIZARVENTANATRAMPA = false;

		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_fin);
			TRenderTrampa = performancecounter_diff(&t_fin, &t_ini);
		#endif
	}
};

System::Void SistemaControl::bwDrawMask_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	#ifdef TIME_TASKS
		LARGE_INTEGER t_ini, t_fin;
	#endif

	while(!bwDrawMask->CancellationPending)
	{
		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_ini);
		#endif

		// Funcion de pintado seleccion de contexto de dibujo actual
		if(((Principal ^)_ventanaPrincipal)->mutexOGL->WaitOne())
		{
			if(ventanaSimulacionGl.SetActual())
			{
				ventanaSimulacionGl.Resize(panelTab0Der->Width,panelTab0Der->Height);
				ventanaSimulacionGl.Renderizar(_theMatrixAgents_Dynamic->_fRunning,_theMatrixAmbiente,_theMatrixAgents_Dynamic);
			}
			((Principal ^)_ventanaPrincipal)->mutexOGL->ReleaseMutex();
		}
		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_fin);
			TRenderMV = performancecounter_diff(&t_fin, &t_ini);
		#endif
	}
};

System::Void SistemaControl::bwControl_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	#ifdef TIME_TASKS
		LARGE_INTEGER t_ini, t_fin;
	#endif

	_theMatrixAgents_Static		= new AgentsSystem(); // FIXME ...
	_theMatrixAgents_Dynamic	= new Sistema_Agentes();
	_theMatrixAmbiente			= new Ambiente();
	_theMatrixEnvironment		= new Enviroment();

	//_theMatrixEnvironment->Inicializar();
	_theMatrixAgents_Static->Inicializar(_theMatrixEnvironment,  _numDFCs, 0, 0);
	_theMatrixAgents_Dynamic->Inicializar(_theMatrixAmbiente, _numDFCs, 0, 0);

	bwDraw->RunWorkerAsync();
	bwDrawMask->RunWorkerAsync();
	while(!bwControl->CancellationPending)
	{
		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_ini);
		#endif
		#ifdef TIME_TASKS
			QueryPerformanceCounter(&t_fin);
			TCalcMaskV = performancecounter_diff(&t_fin, &t_ini);
		#endif

		_theMatrixAgents_Dynamic->UpdateEstado(_theMatrixAmbiente);
		_theMatrixAgents_Static->UpdateEstado(_theMatrixEnvironment);

		Sleep((float)(numericDeadTime->Value)*1000.0f); // FIXME SET RIGHT DEAD TIME
		
		#ifdef TIME_TASKS
			if (tabTIMETASKS->InvokeRequired == false)
			{
				funcionActualizacion();
			}
			else
			{
				DelegateThreadTask ^myThreadDelegate = gcnew DelegateThreadTask(this,&SistemaControl::funcionActualizacion);
				this->Invoke(myThreadDelegate);	
			}
		#endif
	}
};
System::Void SistemaControl::panelTab0Izq_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	// ELEMENTOMODIFICABLE = 0 ... modificar esferas
	// ELEMENTOMODIFICABLE = 1 .... modificar rectangulo
	// ELEMENTOMODIFICABLE = 2 ... modificar Campo
	if(ELEMENTOMODIFICABLE == 0)
	{
		if(_Borrar)
		{
			//if(_theMatrixAgents_Static->GetSelected() >= 0)
			{
				//_theMatrixAgents_Static->DeleteEsfera();
			}
			_Borrar = false;
			ACTUALIZARVENTANATRAMPA =  true;
		}
		else if(_CrearMover)
		{// Crear Objeto
			point_3D temp;
			SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
			//_theMatrixAgents_Static->AddEsfera(temp);
			_CrearMover = false; // true: crear, false: mover
			//if(_theMatrixAgents_Static->GetSelected() >= 0)
			{
				//numericIndiceTrampa->Value = _theMatrixAgents_Static->GetSelected();
				//point_3D temp = _theMatrixAgents_Static->GetPosEsferas((unsigned)numericIndiceTrampa->Value);
				//numericXTrampa->Value = (int)temp.x;
				//numericYTrampa->Value = (int)temp.y;
				//numericZTrampa->Value = (int)temp.z;
				ACTUALIZARVENTANATRAMPA =  true;
			}
		}
		else
		{// Seleccionar trampa a MOver
			point_3D temp;
			SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
			//_theMatrixAgents_Static->SelectMove(temp);
			//((Mascara ^)((Principal ^)_ventanaPrincipal)->_ventanaMascara)->_theMatrixAgents_Static->SelectMove((float)e->X,(float)e->Y);
			
			//if(_theMatrixAgents_Static->GetSelected() >= 0)
			{
				//numericIndiceTrampa->Value = _theMatrixAgents_Static->GetSelected();
				//point_3D temp = _theMatrixAgents_Static->GetPosEsferas((unsigned)numericIndiceTrampa->Value);
				//numericXTrampa->Value = (int)temp.x;
				//numericYTrampa->Value = (int)temp.y;
				//numericZTrampa->Value = (int)temp.z;
				ACTUALIZARVENTANATRAMPA =  true;
			}
		}
	}
	if(ELEMENTOMODIFICABLE == 1)
	{
		point_3D temp;
		SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);

		//int temporal = _theMatrixAgents_Static->GetNearestBox(temp);
		//if(temporal == 0)
		{
			//_theMatrixAgents_Static->SetPosBox(temp);
			numericRectX->Value = (int)temp.x;
			numericRectY->Value = (int)temp.y;

			SetFull(temp,1.0f,1.0f,0.0f);
			numericRectAncho->Value = (int)temp.x;
			numericRectAlto->Value = (int)temp.y;

			//_theMatrixAgents_Static->SetDimBox(temp);
		}
		//_theMatrixAgents_Static->SetModBoxState(temporal);
	}
	if(ELEMENTOMODIFICABLE == 2)
	{
		point_3D temp;
		SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
		//_theMatrixAgents_Static->SetPosField(temp);
		numericXCampo->Value = (int)temp.x;
		numericYCampo->Value = (int)temp.y;
	}
};

System::Void SistemaControl::panelTab0Izq_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if(ELEMENTOMODIFICABLE == 0)
	{
		if(_theMatrixAgents_Static && e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			point_3D temp;
			SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
			//_theMatrixAgents_Static->Move(temp);

			//if(_theMatrixAgents_Static->GetSelected() >= 0)
			{
				//point_3D temp = _theMatrixAgents_Static->GetPosEsferas((int)numericIndiceTrampa->Value);
				//numericXTrampa->Value = (int)temp.x;
				//numericYTrampa->Value = (int)temp.y;
				//numericZTrampa->Value = (int)temp.z;
				ACTUALIZARVENTANATRAMPA =  true;
			}
		}
	}
	if(ELEMENTOMODIFICABLE == 1)
	{
		//if(_theMatrixAgents_Static->GetModBoxState() == 0 || _theMatrixAgents_Static->GetModBoxState() == 2)
		{
			point_3D largo,inicioBox;
			Zero(inicioBox);
			Zero(largo);
		//	inicioBox = _theMatrixAgents_Static->GetPosBox();
			SetFull(largo,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
			largo.x = largo.x - inicioBox.x;
			largo.y = inicioBox.y - largo.y;

			if(largo.x < 1.0f) largo.x = 1.0f;
			if(largo.y < 1.0f) largo.y = 1.0f;
		//	_theMatrixAgents_Static->SetDimBox(largo);
		}
		//if(_theMatrixAgents_Static->GetModBoxState() == 1)
		{
			point_3D largo,inicioBox;//,tempFin;
			Zero(inicioBox);
			Zero(largo);
		//	inicioBox	= _theMatrixAgents_Static->GetPosBox();
		//	largo		= _theMatrixAgents_Static->GetDimBox();
		//	largo.y		= -largo.y;
		//	Suma(tempFin,inicioBox,largo);
		//	SetFull(inicioBox,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
		//	largo.x = tempFin.x - inicioBox.x;
		//	largo.y = inicioBox.y - tempFin.y;

		//	if(largo.x < 1.0f)
			{
		//		inicioBox.x = tempFin.x - 1.0f;
		//		largo.x = 1.0f;
			}
		//	if(largo.y < 1.0f)
			{
		//		inicioBox.y = tempFin.y + 1.0f;
		//		largo.y = 1.0f;
			}
		//	_theMatrixAgents_Static->SetPosBox(inicioBox);
			numericRectX->Value = (int)inicioBox.x;
			numericRectY->Value = (int)inicioBox.y;

		//	_theMatrixAgents_Static->SetDimBox(largo);
			numericRectAncho->Value = (int)largo.x;
			numericRectAlto->Value = (int)largo.y;

		}
	}
	if(ELEMENTOMODIFICABLE == 2)
	{
		point_3D temp;
		SetFull(temp,(float)e->X-(float)(panelTab0Izq->Width)/2.0f,(float)(panelTab0Izq->Height)/2.0f-(float)e->Y,0.0f);
		//_theMatrixAgents_Static->SetPosField(temp);
		numericXCampo->Value = (int)temp.x;
		numericYCampo->Value = (int)temp.y;
	}
};

System::Void SistemaControl::panelTab0Izq_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if(ELEMENTOMODIFICABLE == 0)
	{
		//if(_theMatrixAgents_Static->GetSelected() >= 0)
		{
		//	point_3D temp = _theMatrixAgents_Static->GetPosEsferas((int)numericIndiceTrampa->Value);
		//	numericXTrampa->Value = (int)temp.x;
		//	numericYTrampa->Value = (int)temp.y;
		//	numericZTrampa->Value = (int)temp.z;
			ACTUALIZARVENTANATRAMPA =  true;
		}
		//_theMatrixAgents_Static->FreeMove();
	}
	if(ELEMENTOMODIFICABLE == 1)
	{
		//_theMatrixAgents_Static->SetModBoxState(-1);
	}
	if(ELEMENTOMODIFICABLE == 2)
	{
		ELEMENTOMODIFICABLE = -1;
	}
};

System::Void SistemaControl::bStartSim_Click(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetPosPrecursoras((_theMatrixAgents_Static->GetPosBox()).y);
	//_theMatrixAgents_Dynamic->IgualarClase(_theMatrixAgents_Static);
	_theMatrixAgents_Dynamic->_fRunning = true;
	_theMatrixAgents_Static->_fRunning = true;
};

System::Void SistemaControl::bStopSim_Click(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixAgents_Dynamic->_fRunning = false;
	_theMatrixAgents_Static->_fRunning = false;
};
System::Void SistemaControl::numericIndiceTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//numericIndiceTrampa->Maximum = _theMatrixAgents_Static->GetMaxEsferas();
	if(numericIndiceTrampa->Value > numericIndiceTrampa->Maximum)
	{
		numericIndiceTrampa->Value = numericIndiceTrampa->Maximum;
	}
	else if(numericIndiceTrampa->Value < 0)
	{
		numericIndiceTrampa->Value = 0;
	}
	//if(_theMatrixAgents_Static->GetActEsfera((int)numericIndiceTrampa->Value))
	{
		numericXTrampa->Enabled = true;
		numericYTrampa->Enabled = true;
		numericZTrampa->Enabled = true;

	//	point_3D temp = _theMatrixAgents_Static->GetPosEsferas((int)numericIndiceTrampa->Value);
	//	numericXTrampa->Value = (int)temp.x;
	//	numericYTrampa->Value = (int)temp.y;
	//	numericZTrampa->Value = (int)temp.z;
	}
	//else
	{
		numericXTrampa->Enabled = false;
		numericYTrampa->Enabled = false;
		numericZTrampa->Enabled = false;
		numericXTrampa->Value = 0;
		numericYTrampa->Value = 0;
		numericZTrampa->Value = 0;
	}
};
System::Void SistemaControl::numericXTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericXTrampa->Value,(float) numericYTrampa->Value,(float) numericZTrampa->Value);
	//_theMatrixAgents_Static->SetPosEsferas((int)numericIndiceTrampa->Value,temp);
	//_theMatrixAgents_Dynamic->SetPosEsferas((int)numericIndiceTrampa->Value,temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);

	ACTUALIZARVENTANATRAMPA =  true;
};
System::Void SistemaControl::numericYTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericXTrampa->Value,(float) numericYTrampa->Value,(float) numericZTrampa->Value);
	//_theMatrixAgents_Static->SetPosEsferas((int)numericIndiceTrampa->Value,temp);
	//_theMatrixAgents_Dynamic->SetPosEsferas((int)numericIndiceTrampa->Value,temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);

	ACTUALIZARVENTANATRAMPA =  true;
};
System::Void SistemaControl::numericZTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericXTrampa->Value,(float) numericYTrampa->Value,(float) (numericZTrampa->Value));
	//_theMatrixAgents_Static->SetPosEsferas((int)numericIndiceTrampa->Value,temp);
	//_theMatrixAgents_Dynamic->SetPosEsferas((int)numericIndiceTrampa->Value,temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
	ACTUALIZARVENTANATRAMPA =  true;
};

System::Void SistemaControl::bDelete_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//_theMatrixAgents_Static->DeleteEsfera();
	//_theMatrixAgents_Dynamic->DeleteEsfera();
};

System::Void SistemaControl::bCrearObjetos_Click(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->CrearGrupoEsferas((unsigned)numericNumObjetos->Value);
	//_theMatrixAgents_Dynamic->IgualarClase(_theMatrixAgents_Static);
	_theMatrixAgents_Static->Inicializar(_theMatrixEnvironment,_numDFCs,0,0);
	_theMatrixAgents_Dynamic->Inicializar(_theMatrixAmbiente,_numDFCs,0,0);
	ELEMENTOMODIFICABLE = 0;


	_theMatrixEnvironment->_minInitDFC.y = 0.0f;
	_numDFCs = (int) nUD_NumDFCs->Value;
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._perturbacionAzar = cB_RandPerturbation->Checked;
	}
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._liberarEstancado = cB_TryFreeStagnant->Checked;
	}
	_theMatrixEnvironment->_useDEBAttraction = cB_DEBAttraction->Checked;
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._StepsforChange = (int) nUD_StepsForChange->Value;
		_theMatrixAgents_Dynamic->_agentesDFC[i]._persistence = (rand()%(int) nUD_StepsForChange->Value)+1;
	}
	_theMatrixEnvironment->_useEVLInteraction = cB_EVLInteraction->Checked;
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._friendAttraction = cB_DFCInteraction->Checked;
	}
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._ratioFriendAttraction = (float) nUD_RadInteraction->Value;
	}
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._radio = (float) nUD_DFC_Radius->Value;
	}
	_theMatrixEnvironment->_factorDFC_DEB_Action = (float) nUD_Ratio_DFC_DEB->Value;
	if(rB_EVL_Global->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCantidad = 0;
	}
	if(rB_EVL_Selected->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCantidad = 1;
	}
	if(rB_tipoEVL_Puntual->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCampo = 0;
	}
	if(rB_tipoEVL_Linea->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCampo = 1;
	}

	if(rB_tipoEVL_Distribuido->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 0;
	}
	if(rB_tipoEVL_Distribuido->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 1;
	}
	if(rB_tipoEVL_Distribuido->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 2;
	}
	_theMatrixEnvironment->_dataEVL_PosY = (int) nUD_DataEVL_PosY->Value;
	_theMatrixEnvironment->_dataEVL_StepsLinea = (int) nUD_DataEVL_StepRedLine->Value;
	_theMatrixEnvironment->_dataEVL_actualStepsLinea = (int) nUD_DataEVL_StepRedLine->Value;
	_theMatrixEnvironment->_dataEVL_StepsCambioEVLDist = (int) nUD_DataEVL_StepChangeDist->Value;
	_theMatrixEnvironment->_dataEVL_actualStepDistEVL  = 0;
	_theMatrixEnvironment->_dataEVL_AnchoLinea = (int) nUD_DataEVL_AnchoLinea->Value;
	_theMatrixEnvironment->_dataEVL_FactorEVL = (float) nUD_EVL_Factor->Value;
	_theMatrixEnvironment->_useEVLInteractionAdhesion = cB_AdhesionEVL->Checked;
	_theMatrixEnvironment->_stepsMovDEB_margin = (int) nUD_StepsMovDB_margin->Value;
	_theMatrixEnvironment->_actualStepsMovDEB_margin = (int) nUD_StepsMovDB_margin->Value;	
	_theMatrixEnvironment->_stepsMovEVL_margin = (int) nUD_StepsEVL_Mov->Value;
	_theMatrixEnvironment->_actualStepsMovEVL_margin = (int) nUD_StepsEVL_Mov->Value;
	_theMatrixAgents_Dynamic->_posicionPorAttActual = 0;
	_theMatrixEnvironment->_dataEVL_actualStepDistEVL = 0;
	_theMatrixEnvironment->_minInitDFC.y = 0.0f;
	_theMatrixEnvironment->_posMarginDEB.y = 0.0f;

	_theMatrixEnvironment->_stepsProliferacion = (int) nUP_ProliferacionSteps->Value;
	_theMatrixEnvironment->_posProliferation = -1;
	_theMatrixEnvironment->_actualStepsProliferacion = 0;
	
	ventanaSimulacionGl._numVectorParam1 = -1.0f;
	ventanaSimulacionGl._maxVectorParam1 = 0.0f;
	ventanaSimulacionGl._contador = 0;

	ventanaSimulacionGl._numVectorParam2 = -1.0f;
	ventanaSimulacionGl._maxVectorParam2 = 0.0f;

	ventanaSimulacionGl._numVectorParam3 = -1.0f;
	ventanaSimulacionGl._maxVectorParam3 = 0.0f;

	ventanaSimulacionGl._numVectorParam4 = -1.0f;
	ventanaSimulacionGl._maxVectorParam4 = 0.0f;
	ventanaSimulacionGl._numVectorParam5 = -1.0f;
	ventanaSimulacionGl._maxVectorParam5 = 0.0f;
	ventanaSimulacionGl._numVectorParam6 = -1.0f;
	ventanaSimulacionGl._maxVectorParam6 = 0.0f;
	ventanaSimulacionGl._numVectorParam7 = -1.0f;
	ventanaSimulacionGl._maxVectorParam7 = 0.0f;
	ventanaSimulacionGl._numVectorParam8 = -1.0f;
	ventanaSimulacionGl._maxVectorParam8 = 0.0f;
	ventanaSimulacionGl._numVectorParam9 = -1.0f;
	ventanaSimulacionGl._maxVectorParam9 = 0.0f;
};
System::Void SistemaControl::bModRect_Click(System::Object^  sender, System::EventArgs^  e)
{
	ELEMENTOMODIFICABLE = 1;
};
System::Void SistemaControl::bModCampo_Click(System::Object^  sender, System::EventArgs^  e)
{
	ELEMENTOMODIFICABLE = 2;
};
System::Void SistemaControl::tabControl2_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e)
{
	ELEMENTOMODIFICABLE = 0;
};

System::Void SistemaControl::numericNumObjetos_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Static->SetBusy(false);
};
System::Void SistemaControl::numericRadioObjetos_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	//_theMatrixAgents_Static->SetRadEsferas( (float) numericRadioObjetos->Value);
	//_theMatrixAgents_Dynamic->SetRadEsferas( (float) numericRadioObjetos->Value);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);

};
System::Void SistemaControl::numericRectAncho_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericRectAncho->Value, (float) numericRectAlto->Value,0.0f);
	//_theMatrixAgents_Static->SetDimBox(temp);
	//_theMatrixAgents_Dynamic->SetDimBox(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericRectAlto_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericRectAncho->Value, (float) numericRectAlto->Value,0.0f);
	//_theMatrixAgents_Static->SetDimBox(temp);
	//_theMatrixAgents_Dynamic->SetDimBox(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericRectX_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericRectX->Value, (float) numericRectY->Value,0.0f);
	//_theMatrixAgents_Static->SetPosBox(temp);
	//_theMatrixAgents_Dynamic->SetPosBox(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericRectY_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericRectX->Value, (float) numericRectY->Value,0.0f);
	//_theMatrixAgents_Static->SetPosBox(temp);
	//_theMatrixAgents_Dynamic->SetPosBox(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericXCampo_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericXCampo->Value, (float) numericYCampo->Value,0.0f);
	//_theMatrixAgents_Static->SetPosField(temp);
	//_theMatrixAgents_Dynamic->SetPosField(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericYCampo_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	point_3D temp;
	SetFull(temp,(float) numericXCampo->Value, (float) numericYCampo->Value,0.0f);
	//_theMatrixAgents_Static->SetPosField(temp);
	//_theMatrixAgents_Dynamic->SetPosField(temp);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};


System::Void SistemaControl::numericDelta_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	//_theMatrixAgents_Static->SetDelta((float)numericDelta->Value);
	//_theMatrixAgents_Dynamic->SetDelta((float)numericDelta->Value);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};
System::Void SistemaControl::numericDeadTime_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	//_theMatrixAgents_Static->SetBusy(true);
	//_theMatrixAgents_Dynamic->SetBusy(true);

	//_theMatrixAgents_Static->SetBusy(false);
	//_theMatrixAgents_Dynamic->SetBusy(false);
};


System::Void SistemaControl::nUD_NumDFCs_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_numDFCs = (int) nUD_NumDFCs->Value;
};
System::Void SistemaControl::cB_RandPerturbation_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._perturbacionAzar = cB_RandPerturbation->Checked;
		_theMatrixAgents_Static->_agentsWM_DFC[i]._perturbacionAzar = cB_RandPerturbation->Checked;

	}
};

System::Void SistemaControl::cB_TryFreeStagnant_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._liberarEstancado = cB_TryFreeStagnant->Checked;
		_theMatrixAgents_Static->_agentsWM_DFC[i]._liberarEstancado = cB_TryFreeStagnant->Checked;
	}
};

System::Void SistemaControl::cB_DEBAttraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_useDEBAttraction = cB_DEBAttraction->Checked;
};

System::Void SistemaControl::nUD_StepsForChange_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._StepsforChange = (int) nUD_StepsForChange->Value;
		_theMatrixAgents_Dynamic->_agentesDFC[i]._persistence = (rand()%(int) nUD_StepsForChange->Value)+1;

		_theMatrixAgents_Static->_agentsWM_DFC[i]._StepsforChange = (int) nUD_StepsForChange->Value;
		_theMatrixAgents_Static->_agentsWM_DFC[i]._persistence = (rand()%(int) nUD_StepsForChange->Value)+1;
	}
};

System::Void SistemaControl::cB_EVLInteraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_useEVLInteraction = cB_EVLInteraction->Checked;
};

System::Void SistemaControl::cB_DFCInteraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._friendAttraction = cB_DFCInteraction->Checked;
		_theMatrixAgents_Static->_agentsWM_DFC[i]._friendAttraction = cB_DFCInteraction->Checked;
	}
};

System::Void SistemaControl::nUD_RadInteraction_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._ratioFriendAttraction = (float) nUD_RadInteraction->Value;
		_theMatrixAgents_Static->_agentsWM_DFC[i]._ratioFriendAttraction = (float) nUD_RadInteraction->Value;
	}
};
System::Void SistemaControl::nUD_DFC_Radius_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	for(register int i = 0; i < _theMatrixAgents_Dynamic->_numDFCActiveAgents; i++)
	{
		_theMatrixAgents_Dynamic->_agentesDFC[i]._radio = (float) nUD_DFC_Radius->Value;
		_theMatrixAgents_Static->_agentsWM_DFC[i].SetRad( (float) nUD_DFC_Radius->Value,0);
	}
};

System::Void SistemaControl::nUD_Ratio_DFC_DEB_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_factorDFC_DEB_Action = (float) nUD_Ratio_DFC_DEB->Value;
};

System::Void SistemaControl::rB_EVL_Global_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_EVL_Global->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCantidad = 0;
	}
};
System::Void SistemaControl::rB_EVL_Selected_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_EVL_Selected->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCantidad = 1;
	}
};
System::Void SistemaControl::rB_tipoEVL_Puntual_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_tipoEVL_Puntual->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCampo = 0;
	}
};
System::Void SistemaControl::rB_tipoEVL_Linea_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_tipoEVL_Linea->Checked)
	{
		_theMatrixEnvironment->_dataEVL_tipoCampo = 1;
	}
};
System::Void SistemaControl::nUD_DataEVL_PosY_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_dataEVL_PosY = (int) nUD_DataEVL_PosY->Value;
};
System::Void SistemaControl::nUD_DataEVL_StepRedLine_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_dataEVL_StepsLinea = (int) nUD_DataEVL_StepRedLine->Value;
	_theMatrixEnvironment->_dataEVL_actualStepsLinea = (int) nUD_DataEVL_StepRedLine->Value;
};
System::Void SistemaControl::nUD_DataEVL_StepChangeDist_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_dataEVL_StepsCambioEVLDist = (int) nUD_DataEVL_StepChangeDist->Value;
	_theMatrixEnvironment->_dataEVL_actualStepDistEVL  = 0;
};
System::Void SistemaControl::nUD_DataEVL_AnchoLinea_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_dataEVL_AnchoLinea = (int) nUD_DataEVL_AnchoLinea->Value;
};
System::Void SistemaControl::nUD_EVL_Factor_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_dataEVL_FactorEVL = (float) nUD_EVL_Factor->Value;
};

System::Void SistemaControl::cB_AdhesionEVL_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_useEVLInteractionAdhesion = cB_AdhesionEVL->Checked;
};

System::Void SistemaControl::nUD_StepsMovDB_margin_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_stepsMovDEB_margin = (int) nUD_StepsMovDB_margin->Value;
	_theMatrixEnvironment->_actualStepsMovDEB_margin = (int) nUD_StepsMovDB_margin->Value;	
};
System::Void SistemaControl::nUD_StepsEVL_Mov_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_stepsMovEVL_margin = (int) nUD_StepsEVL_Mov->Value;
	_theMatrixEnvironment->_actualStepsMovEVL_margin = (int) nUD_StepsEVL_Mov->Value;
};

System::Void SistemaControl::rB_tipoEVL_Distribuido_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_tipoEVL_Distribuido->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 0;
	}
};
System::Void SistemaControl::rB_tipoEVL_marginal_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_tipoEVL_marginal->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 1;
	}
};

System::Void SistemaControl::rB_tipoEVL_DistMarginal_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if(rB_tipoEVL_DistMarginal->Checked)
	{
		_theMatrixEnvironment->_dataEVL_IncludedCampos = 2;
	}
};

System::Void SistemaControl::nUP_ProliferacionSteps_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	_theMatrixEnvironment->_stepsProliferacion = (int) nUP_ProliferacionSteps->Value;
	_theMatrixEnvironment->_posProliferation = -1;
	_theMatrixEnvironment->_actualStepsProliferacion = 0;
};