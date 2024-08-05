#include "MainWindow.h"
#include "Launcher.h"

#include "Gl_InitialState.h"

#include <time.h>
#include <windows.h>

Gl_InitialState _windowInitialGl;

using namespace AplicacionCD2Cells;

System::Void MainWindow::MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e){
	((Launcher ^)_windowLauncher)->Close();
}

System::Void MainWindow::MainWindow_Load(System::Object^  sender, System::EventArgs^  e) {
	if(((Launcher ^)_windowLauncher)->mutexOGL->WaitOne()) // No realmente necesario.. aun no empiezan a correr las otras hebras
	{
		wglMakeCurrent (NULL, NULL);
		_windowInitialGl.Crear( reinterpret_cast<HWND> (panelSingleMode->Handle.ToPointer()),W_ONE);	

		wglMakeCurrent (NULL, NULL);
	
		((Launcher ^)_windowLauncher)->mutexOGL->ReleaseMutex();
	}

	bwControl->RunWorkerAsync();
}

System::Void MainWindow::bwDraw_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e){
	while(!bwDraw->CancellationPending)
	{
		// Funcion de pintado seleccion de contexto de dibujo actual
		if(((Launcher ^)_windowLauncher)->mutexOGL->WaitOne())
		{
			if(_windowInitialGl.SetActual())
			{
				_windowInitialGl.Resize(panelSingleMode->Width, panelSingleMode->Height);
				_windowInitialGl.Renderizar(_theMatrixEnvironmentSystem,_theMatrixAgents_Static);
			}
			((Launcher ^)_windowLauncher)->mutexOGL->ReleaseMutex();
		}
	}	
}

System::Void MainWindow::bwControl_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e){
	_theMatrixAgents_Static		= new AgentsSystem(); // FIXME ...
	_theMatrixEnvironmentSystem	= new EnvironmentSystem();

	_theMatrixEnvironmentSystem->Initiate();
	_theMatrixAgents_Static->Initiate(_theMatrixEnvironmentSystem);

	bwDraw->RunWorkerAsync();
	while(!bwControl->CancellationPending)
	{
		if(((Launcher ^)_windowLauncher)->mutexOGL->WaitOne())
		{
			_theMatrixEnvironmentSystem->Update();
			_theMatrixAgents_Static->UpdateState(_theMatrixEnvironmentSystem);
			((Launcher ^)_windowLauncher)->mutexOGL->ReleaseMutex();
		}
		//Sleep((float)(numericDeadTime->Value)*1000.0f); // FIXME SET RIGHT DEAD TIME
		Sleep((float)(2)*100.0f); // FIXME SET RIGHT DEAD TIME
	}
}

System::Void MainWindow::bSartSim_Click(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_bShowInitialRect = 0;
	bUpdateSettings->Enabled			   = false;
	_theMatrixAgents_Static->_bRunning     = true;
	_theMatrixEnvironmentSystem->_bRunning = true;
}
System::Void MainWindow::bStopSim_Click(System::Object^  sender, System::EventArgs^  e){
	_theMatrixAgents_Static->_bRunning     = false;
	_theMatrixEnvironmentSystem->_bRunning = false;
	bUpdateSettings->Enabled			   = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// MANAGE OPTIONS FROM GUIDE ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////// Main Updater
System::Void MainWindow::bUpdateSettings_Click(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->Reinitiate();
	_theMatrixAgents_Static->Initiate(_theMatrixEnvironmentSystem);
};
//////////////////////// Work Space
System::Void MainWindow::nUD_WS_W_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_dimWS.x = (float) nUD_WS_W->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_WS_H_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_dimWS.y = (float) nUD_WS_H->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_WS_MigEVL_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_dataEVL_PosRef[NUMSTAGES-1] = (float) nUD_WS_MigEVL->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_SpeedEVL_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_velMarginEVL = (float) nUD_SpeedEVL->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};

//////////////////////// DFCs Group
System::Void MainWindow::nUD_MarginGap_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_gapMargin = (float) nUD_MarginGap->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_InitialDFCsW_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_dimDFCs.x = (float) nUD_InitialDFCsW->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_InitialDFCsH_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixEnvironmentSystem->_dimDFCs.y = (float) nUD_InitialDFCsH->Value;
	_theMatrixEnvironmentSystem->Reinitiate();
};
System::Void MainWindow::nUD_InitialDFCsGap_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	// = (float) nUD_InitialDFCsGap->Value;
};
System::Void MainWindow::nUD_InitialNumDFCs_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixAgents_Static->_numDFCActiveAgents = (int) nUD_InitialNumDFCs->Value;
};

//////////////////////// DFCs Individual
System::Void MainWindow::nUD_DFCRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	_theMatrixAgents_Static->_radius = (float) nUD_DFCRadius->Value;
};
System::Void MainWindow::nUD_DFC_AdhesionDist_ValueChanged(System::Object^  sender, System::EventArgs^  e){
	(float) nUD_DFC_AdhesionDist->Value;
};