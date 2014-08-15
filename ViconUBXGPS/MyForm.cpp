#include "MyForm.h"

using namespace System;
using namespace System ::Windows::Forms;

[STAThread]
void main(array<System::String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ViconUBXGPS::MyForm form;
	Application::Run(%form);
}
