#include "Launcher.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace AplicacionCD2Cells;

[STAThreadAttribute]
void main(void){
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	//nombre del proyecto
	//AplicacionCD2Cells::Launcher form;
	//Application::Run(%form);
	Application::Run(gcnew Launcher());
}