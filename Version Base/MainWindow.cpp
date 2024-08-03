#include "MainWindow.h"
#include "Launcher.h"

#include "Gl_InitialState.h"

#include <time.h>
#include <windows.h>

Gl_InitialState _windowInitialGl;

using namespace AplicacionCD2Cells;

System::Void MainWindow::MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e){
	((Launcher ^)_windowLauncher)->Show();
}

System::Void MainWindow::MainWindow_Load(System::Object^  sender, System::EventArgs^  e) {
	_numDFCs = 25;
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

	//_theMatrixEnvironmentSystem->Initiate();
	_theMatrixAgents_Static->Initiate(_theMatrixEnvironmentSystem,  _numDFCs, 0, 0);

	bwDraw->RunWorkerAsync();
	while(!bwControl->CancellationPending)
	{
		_theMatrixAgents_Static->UpdateState(_theMatrixEnvironmentSystem);

		//Sleep((float)(numericDeadTime->Value)*1000.0f); // FIXME SET RIGHT DEAD TIME
		Sleep((float)(2)*100.0f); // FIXME SET RIGHT DEAD TIME
	}
}

System::Void MainWindow::bSartSim_Click(System::Object^  sender, System::EventArgs^  e){
	_theMatrixAgents_Static->_bRunning = true;
}
System::Void MainWindow::bStopSim_Click(System::Object^  sender, System::EventArgs^  e){
	_theMatrixAgents_Static->_bRunning = false;
}
