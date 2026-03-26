#include "MainWindow.h"
#include "Launcher.h"

using namespace AplicacionCD2Cells;

System::Void MainWindow::MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e){
	((Launcher ^)_windowLauncher)->Show();
};