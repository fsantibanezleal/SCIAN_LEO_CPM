#include "Launcher.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace AplicacionCD2Cells;

[STAThreadAttribute]
void main(void){
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew Launcher());
}