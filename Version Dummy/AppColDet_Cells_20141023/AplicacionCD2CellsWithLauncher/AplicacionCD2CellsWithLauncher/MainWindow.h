#pragma once

//#include "AgentsSystem.h"
//#include "Enviroment.h"

//#include "AgentsSystem.h"
//#include "Enviroment.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace AplicacionCD2Cells {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::Form ^_windowLauncher;
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}
		MainWindow(System::Windows::Forms::Form ^vLauncher)
		{
			_windowLauncher = vLauncher;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuMain;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^  tlpMain;
	private: System::Windows::Forms::TabControl^  tcGraphics;
	private: System::Windows::Forms::TabPage^  tpGraphs1;
	private: System::Windows::Forms::Panel^  panelSingleMode;
	private: System::Windows::Forms::TabPage^  tpGraphs2;
	private: System::Windows::Forms::TabControl^  tcOptions;
	private: System::Windows::Forms::TabPage^  tpOptions1;
	private: System::Windows::Forms::TabPage^  tpOptions2;

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuMain = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tlpMain = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tcGraphics = (gcnew System::Windows::Forms::TabControl());
			this->tpGraphs1 = (gcnew System::Windows::Forms::TabPage());
			this->panelSingleMode = (gcnew System::Windows::Forms::Panel());
			this->tpGraphs2 = (gcnew System::Windows::Forms::TabPage());
			this->tcOptions = (gcnew System::Windows::Forms::TabControl());
			this->tpOptions1 = (gcnew System::Windows::Forms::TabPage());
			this->tpOptions2 = (gcnew System::Windows::Forms::TabPage());
			this->menuMain->SuspendLayout();
			this->tlpMain->SuspendLayout();
			this->tcGraphics->SuspendLayout();
			this->tpGraphs1->SuspendLayout();
			this->tcOptions->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuMain
			// 
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuMain->Location = System::Drawing::Point(0, 0);
			this->menuMain->Name = L"menuMain";
			this->menuMain->Size = System::Drawing::Size(1664, 24);
			this->menuMain->TabIndex = 0;
			this->menuMain->Text = L"Main Menu";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// tlpMain
			// 
			this->tlpMain->ColumnCount = 2;
			this->tlpMain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tlpMain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 80)));
			this->tlpMain->Controls->Add(this->tcGraphics, 1, 0);
			this->tlpMain->Controls->Add(this->tcOptions, 0, 0);
			this->tlpMain->Location = System::Drawing::Point(12, 27);
			this->tlpMain->Name = L"tlpMain";
			this->tlpMain->RowCount = 1;
			this->tlpMain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpMain->Size = System::Drawing::Size(1640, 960);
			this->tlpMain->TabIndex = 1;
			// 
			// tcGraphics
			// 
			this->tcGraphics->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tcGraphics->Controls->Add(this->tpGraphs1);
			this->tcGraphics->Controls->Add(this->tpGraphs2);
			this->tcGraphics->Location = System::Drawing::Point(331, 3);
			this->tcGraphics->Name = L"tcGraphics";
			this->tcGraphics->SelectedIndex = 0;
			this->tcGraphics->Size = System::Drawing::Size(1306, 954);
			this->tcGraphics->TabIndex = 0;
			// 
			// tpGraphs1
			// 
			this->tpGraphs1->Controls->Add(this->panelSingleMode);
			this->tpGraphs1->Location = System::Drawing::Point(4, 22);
			this->tpGraphs1->Name = L"tpGraphs1";
			this->tpGraphs1->Padding = System::Windows::Forms::Padding(3);
			this->tpGraphs1->Size = System::Drawing::Size(1298, 928);
			this->tpGraphs1->TabIndex = 0;
			this->tpGraphs1->Text = L"Single Mode";
			this->tpGraphs1->UseVisualStyleBackColor = true;
			// 
			// panelSingleMode
			// 
			this->panelSingleMode->BackColor = System::Drawing::Color::Silver;
			this->panelSingleMode->Location = System::Drawing::Point(210, 0);
			this->panelSingleMode->Name = L"panelSingleMode";
			this->panelSingleMode->Size = System::Drawing::Size(950, 950);
			this->panelSingleMode->TabIndex = 0;
			// 
			// tpGraphs2
			// 
			this->tpGraphs2->Location = System::Drawing::Point(4, 22);
			this->tpGraphs2->Name = L"tpGraphs2";
			this->tpGraphs2->Padding = System::Windows::Forms::Padding(3);
			this->tpGraphs2->Size = System::Drawing::Size(1462, 902);
			this->tpGraphs2->TabIndex = 1;
			this->tpGraphs2->Text = L"Dual Mode";
			this->tpGraphs2->UseVisualStyleBackColor = true;
			// 
			// tcOptions
			// 
			this->tcOptions->Controls->Add(this->tpOptions1);
			this->tcOptions->Controls->Add(this->tpOptions2);
			this->tcOptions->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tcOptions->Location = System::Drawing::Point(3, 3);
			this->tcOptions->Name = L"tcOptions";
			this->tcOptions->SelectedIndex = 0;
			this->tcOptions->Size = System::Drawing::Size(322, 954);
			this->tcOptions->TabIndex = 1;
			// 
			// tpOptions1
			// 
			this->tpOptions1->Location = System::Drawing::Point(4, 22);
			this->tpOptions1->Name = L"tpOptions1";
			this->tpOptions1->Padding = System::Windows::Forms::Padding(3);
			this->tpOptions1->Size = System::Drawing::Size(314, 928);
			this->tpOptions1->TabIndex = 0;
			this->tpOptions1->Text = L"Options";
			this->tpOptions1->UseVisualStyleBackColor = true;
			// 
			// tpOptions2
			// 
			this->tpOptions2->Location = System::Drawing::Point(4, 22);
			this->tpOptions2->Name = L"tpOptions2";
			this->tpOptions2->Padding = System::Windows::Forms::Padding(3);
			this->tpOptions2->Size = System::Drawing::Size(150, 928);
			this->tpOptions2->TabIndex = 1;
			this->tpOptions2->Text = L"More";
			this->tpOptions2->UseVisualStyleBackColor = true;
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1664, 1011);
			this->Controls->Add(this->tlpMain);
			this->Controls->Add(this->menuMain);
			this->MainMenuStrip = this->menuMain;
			this->MaximizeBox = false;
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->tlpMain->ResumeLayout(false);
			this->tcGraphics->ResumeLayout(false);
			this->tpGraphs1->ResumeLayout(false);
			this->tcOptions->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
	};
}
