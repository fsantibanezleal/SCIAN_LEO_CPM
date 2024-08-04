#pragma once

#include "AgentsSystem.h"
#include "EnvironmentSystem.h"

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
	public:	AgentsSystem		*_theMatrixAgents_Static;
	public:	EnvironmentSystem	*_theMatrixEnvironmentSystem;
	private: System::Windows::Forms::Button^  bStopSim;
	public: 
	private: System::Windows::Forms::Button^  bSartSim;
	private: System::Windows::Forms::NumericUpDown^  nUD_DFCRadius;

	private: System::Windows::Forms::Label^  labelDFCR;
	private: System::Windows::Forms::Button^  bUpdateSettings;
	private: System::Windows::Forms::GroupBox^  gbSettingsDFC_Individual;

	private: System::Windows::Forms::GroupBox^  gbSettingsWS;
	private: System::Windows::Forms::NumericUpDown^  nUD_WS_H;
	private: System::Windows::Forms::Label^  labelWSH;
	private: System::Windows::Forms::NumericUpDown^  nUD_WS_W;
	private: System::Windows::Forms::Label^  labelW;
	private: System::Windows::Forms::GroupBox^  gbSettingsDFC_Group;
	private: System::Windows::Forms::NumericUpDown^  nUD_MarginGap;

	private: System::Windows::Forms::Label^  labelInitialMargindDist;
	private: System::Windows::Forms::NumericUpDown^  nUD_InitialDFCsW;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  nUD_InitialDFCsGap;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  nUD_InitialDFCsH;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  nUD_InitialNumDFCs;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  nUD_DFC_AdhesionDist;
	private: System::Windows::Forms::CheckBox^  cbProliferation;
	private: System::Windows::Forms::CheckBox^  cbStochasticMov;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Label^  nUD_WS_SpeedEVL;
	private: System::Windows::Forms::NumericUpDown^  nUD_WS_MigEVL;
	private: System::Windows::Forms::Label^  label6;

	public:	int					_numDFCs;


	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: agregar c�digo de constructor aqu�
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
		/// Limpiar los recursos que se est�n utilizando.
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
	private: System::ComponentModel::BackgroundWorker^  bwControl;
	private: System::ComponentModel::BackgroundWorker^  bwDraw;


	private:
		/// <summary>
		/// Variable del dise�ador requerida.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// M�todo necesario para admitir el Dise�ador. No se puede modificar
		/// el contenido del m�todo con el editor de c�digo.
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
			this->gbSettingsDFC_Group = (gcnew System::Windows::Forms::GroupBox());
			this->nUD_InitialNumDFCs = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->nUD_InitialDFCsGap = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->nUD_InitialDFCsH = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->nUD_InitialDFCsW = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUD_MarginGap = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->labelInitialMargindDist = (gcnew System::Windows::Forms::Label());
			this->gbSettingsDFC_Individual = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->nUD_DFC_AdhesionDist = (gcnew System::Windows::Forms::NumericUpDown());
			this->cbProliferation = (gcnew System::Windows::Forms::CheckBox());
			this->cbStochasticMov = (gcnew System::Windows::Forms::CheckBox());
			this->labelDFCR = (gcnew System::Windows::Forms::Label());
			this->nUD_DFCRadius = (gcnew System::Windows::Forms::NumericUpDown());
			this->gbSettingsWS = (gcnew System::Windows::Forms::GroupBox());
			this->nUD_WS_H = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelWSH = (gcnew System::Windows::Forms::Label());
			this->nUD_WS_W = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelW = (gcnew System::Windows::Forms::Label());
			this->bUpdateSettings = (gcnew System::Windows::Forms::Button());
			this->bStopSim = (gcnew System::Windows::Forms::Button());
			this->bSartSim = (gcnew System::Windows::Forms::Button());
			this->tpOptions2 = (gcnew System::Windows::Forms::TabPage());
			this->bwControl = (gcnew System::ComponentModel::BackgroundWorker());
			this->bwDraw = (gcnew System::ComponentModel::BackgroundWorker());
			this->nUD_WS_MigEVL = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUD_WS_SpeedEVL = (gcnew System::Windows::Forms::Label());
			this->menuMain->SuspendLayout();
			this->tlpMain->SuspendLayout();
			this->tcGraphics->SuspendLayout();
			this->tpGraphs1->SuspendLayout();
			this->tcOptions->SuspendLayout();
			this->tpOptions1->SuspendLayout();
			this->gbSettingsDFC_Group->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialNumDFCs))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsGap))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsH))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsW))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_MarginGap))->BeginInit();
			this->gbSettingsDFC_Individual->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFC_AdhesionDist))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFCRadius))->BeginInit();
			this->gbSettingsWS->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_H))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_W))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_MigEVL))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			this->SuspendLayout();
			// 
			// menuMain
			// 
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuMain->Location = System::Drawing::Point(0, 0);
			this->menuMain->Name = L"menuMain";
			this->menuMain->Size = System::Drawing::Size(1358, 24);
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
			this->panelSingleMode->Location = System::Drawing::Point(175, 0);
			this->panelSingleMode->Name = L"panelSingleMode";
			this->panelSingleMode->Size = System::Drawing::Size(650, 650);
			this->panelSingleMode->TabIndex = 0;
			// 
			// tpGraphs2
			// 
			this->tpGraphs2->Location = System::Drawing::Point(4, 22);
			this->tpGraphs2->Name = L"tpGraphs2";
			this->tpGraphs2->Padding = System::Windows::Forms::Padding(3);
			this->tpGraphs2->Size = System::Drawing::Size(1298, 928);
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
			this->tpOptions1->Controls->Add(this->gbSettingsDFC_Group);
			this->tpOptions1->Controls->Add(this->gbSettingsDFC_Individual);
			this->tpOptions1->Controls->Add(this->gbSettingsWS);
			this->tpOptions1->Controls->Add(this->bUpdateSettings);
			this->tpOptions1->Controls->Add(this->bStopSim);
			this->tpOptions1->Controls->Add(this->bSartSim);
			this->tpOptions1->Location = System::Drawing::Point(4, 22);
			this->tpOptions1->Name = L"tpOptions1";
			this->tpOptions1->Padding = System::Windows::Forms::Padding(3);
			this->tpOptions1->Size = System::Drawing::Size(314, 928);
			this->tpOptions1->TabIndex = 0;
			this->tpOptions1->Text = L"Options";
			this->tpOptions1->UseVisualStyleBackColor = true;
			// 
			// gbSettingsDFC_Group
			// 
			this->gbSettingsDFC_Group->Controls->Add(this->nUD_InitialNumDFCs);
			this->gbSettingsDFC_Group->Controls->Add(this->label4);
			this->gbSettingsDFC_Group->Controls->Add(this->nUD_InitialDFCsGap);
			this->gbSettingsDFC_Group->Controls->Add(this->label3);
			this->gbSettingsDFC_Group->Controls->Add(this->nUD_InitialDFCsH);
			this->gbSettingsDFC_Group->Controls->Add(this->label2);
			this->gbSettingsDFC_Group->Controls->Add(this->nUD_InitialDFCsW);
			this->gbSettingsDFC_Group->Controls->Add(this->nUD_MarginGap);
			this->gbSettingsDFC_Group->Controls->Add(this->label1);
			this->gbSettingsDFC_Group->Controls->Add(this->labelInitialMargindDist);
			this->gbSettingsDFC_Group->Location = System::Drawing::Point(7, 282);
			this->gbSettingsDFC_Group->Name = L"gbSettingsDFC_Group";
			this->gbSettingsDFC_Group->Size = System::Drawing::Size(301, 179);
			this->gbSettingsDFC_Group->TabIndex = 7;
			this->gbSettingsDFC_Group->TabStop = false;
			this->gbSettingsDFC_Group->Text = L"Settings DFC Group";
			// 
			// nUD_InitialNumDFCs
			// 
			this->nUD_InitialNumDFCs->Location = System::Drawing::Point(163, 146);
			this->nUD_InitialNumDFCs->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_InitialNumDFCs->Name = L"nUD_InitialNumDFCs";
			this->nUD_InitialNumDFCs->Size = System::Drawing::Size(108, 20);
			this->nUD_InitialNumDFCs->TabIndex = 13;
			this->nUD_InitialNumDFCs->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 0, 0});
			this->nUD_InitialNumDFCs->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_InitialNumDFCs_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(18, 146);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(58, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Num DFCs";
			// 
			// nUD_InitialDFCsGap
			// 
			this->nUD_InitialDFCsGap->DecimalPlaces = 1;
			this->nUD_InitialDFCsGap->Location = System::Drawing::Point(163, 111);
			this->nUD_InitialDFCsGap->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_InitialDFCsGap->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_InitialDFCsGap->Name = L"nUD_InitialDFCsGap";
			this->nUD_InitialDFCsGap->Size = System::Drawing::Size(108, 20);
			this->nUD_InitialDFCsGap->TabIndex = 11;
			this->nUD_InitialDFCsGap->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {40, 0, 0, 0});
			this->nUD_InitialDFCsGap->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_InitialDFCsGap_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(18, 111);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(119, 13);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Max DFC-DFC gap (um)";
			// 
			// nUD_InitialDFCsH
			// 
			this->nUD_InitialDFCsH->DecimalPlaces = 1;
			this->nUD_InitialDFCsH->Location = System::Drawing::Point(163, 85);
			this->nUD_InitialDFCsH->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_InitialDFCsH->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_InitialDFCsH->Name = L"nUD_InitialDFCsH";
			this->nUD_InitialDFCsH->Size = System::Drawing::Size(108, 20);
			this->nUD_InitialDFCsH->TabIndex = 9;
			this->nUD_InitialDFCsH->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->nUD_InitialDFCsH->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_InitialDFCsH_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(18, 85);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(88, 13);
			this->label2->TabIndex = 10;
			this->label2->Text = L"Initial Height (um)";
			// 
			// nUD_InitialDFCsW
			// 
			this->nUD_InitialDFCsW->DecimalPlaces = 1;
			this->nUD_InitialDFCsW->Location = System::Drawing::Point(163, 58);
			this->nUD_InitialDFCsW->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_InitialDFCsW->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_InitialDFCsW->Name = L"nUD_InitialDFCsW";
			this->nUD_InitialDFCsW->Size = System::Drawing::Size(108, 20);
			this->nUD_InitialDFCsW->TabIndex = 7;
			this->nUD_InitialDFCsW->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {250, 0, 0, 0});
			this->nUD_InitialDFCsW->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_InitialDFCsW_ValueChanged);
			// 
			// nUD_MarginGap
			// 
			this->nUD_MarginGap->DecimalPlaces = 1;
			this->nUD_MarginGap->Location = System::Drawing::Point(163, 27);
			this->nUD_MarginGap->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {200, 0, 0, 0});
			this->nUD_MarginGap->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_MarginGap->Name = L"nUD_MarginGap";
			this->nUD_MarginGap->Size = System::Drawing::Size(108, 20);
			this->nUD_MarginGap->TabIndex = 7;
			this->nUD_MarginGap->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->nUD_MarginGap->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_MarginGap_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(18, 58);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(85, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Initial Width (um)";
			// 
			// labelInitialMargindDist
			// 
			this->labelInitialMargindDist->AutoSize = true;
			this->labelInitialMargindDist->Location = System::Drawing::Point(18, 27);
			this->labelInitialMargindDist->Name = L"labelInitialMargindDist";
			this->labelInitialMargindDist->Size = System::Drawing::Size(112, 13);
			this->labelInitialMargindDist->TabIndex = 4;
			this->labelInitialMargindDist->Text = L"Initial Margin Gap (um)";
			// 
			// gbSettingsDFC_Individual
			// 
			this->gbSettingsDFC_Individual->Controls->Add(this->label5);
			this->gbSettingsDFC_Individual->Controls->Add(this->nUD_DFC_AdhesionDist);
			this->gbSettingsDFC_Individual->Controls->Add(this->cbProliferation);
			this->gbSettingsDFC_Individual->Controls->Add(this->cbStochasticMov);
			this->gbSettingsDFC_Individual->Controls->Add(this->labelDFCR);
			this->gbSettingsDFC_Individual->Controls->Add(this->nUD_DFCRadius);
			this->gbSettingsDFC_Individual->Location = System::Drawing::Point(7, 467);
			this->gbSettingsDFC_Individual->Name = L"gbSettingsDFC_Individual";
			this->gbSettingsDFC_Individual->Size = System::Drawing::Size(301, 172);
			this->gbSettingsDFC_Individual->TabIndex = 6;
			this->gbSettingsDFC_Individual->TabStop = false;
			this->gbSettingsDFC_Individual->Text = L"Settings DFC Individual";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(18, 70);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(119, 13);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Adhesion Distance (um)";
			// 
			// nUD_DFC_AdhesionDist
			// 
			this->nUD_DFC_AdhesionDist->DecimalPlaces = 1;
			this->nUD_DFC_AdhesionDist->Location = System::Drawing::Point(163, 70);
			this->nUD_DFC_AdhesionDist->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_DFC_AdhesionDist->Name = L"nUD_DFC_AdhesionDist";
			this->nUD_DFC_AdhesionDist->Size = System::Drawing::Size(108, 20);
			this->nUD_DFC_AdhesionDist->TabIndex = 7;
			this->nUD_DFC_AdhesionDist->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->nUD_DFC_AdhesionDist->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_DFC_AdhesionDist_ValueChanged);
			// 
			// cbProliferation
			// 
			this->cbProliferation->AutoSize = true;
			this->cbProliferation->Location = System::Drawing::Point(21, 117);
			this->cbProliferation->Name = L"cbProliferation";
			this->cbProliferation->Size = System::Drawing::Size(81, 17);
			this->cbProliferation->TabIndex = 5;
			this->cbProliferation->Text = L"Proliferation";
			this->cbProliferation->UseVisualStyleBackColor = true;
			// 
			// cbStochasticMov
			// 
			this->cbStochasticMov->AutoSize = true;
			this->cbStochasticMov->Checked = true;
			this->cbStochasticMov->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cbStochasticMov->Location = System::Drawing::Point(163, 117);
			this->cbStochasticMov->Name = L"cbStochasticMov";
			this->cbStochasticMov->Size = System::Drawing::Size(129, 17);
			this->cbStochasticMov->TabIndex = 4;
			this->cbStochasticMov->Text = L"Stochastic Movement";
			this->cbStochasticMov->UseVisualStyleBackColor = true;
			// 
			// labelDFCR
			// 
			this->labelDFCR->AutoSize = true;
			this->labelDFCR->Location = System::Drawing::Point(18, 27);
			this->labelDFCR->Name = L"labelDFCR";
			this->labelDFCR->Size = System::Drawing::Size(87, 13);
			this->labelDFCR->TabIndex = 2;
			this->labelDFCR->Text = L"DFC Radius (um)";
			// 
			// nUD_DFCRadius
			// 
			this->nUD_DFCRadius->DecimalPlaces = 1;
			this->nUD_DFCRadius->Location = System::Drawing::Point(163, 27);
			this->nUD_DFCRadius->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_DFCRadius->Name = L"nUD_DFCRadius";
			this->nUD_DFCRadius->Size = System::Drawing::Size(108, 20);
			this->nUD_DFCRadius->TabIndex = 3;
			this->nUD_DFCRadius->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->nUD_DFCRadius->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_DFCRadius_ValueChanged);
			// 
			// gbSettingsWS
			// 
			this->gbSettingsWS->Controls->Add(this->numericUpDown2);
			this->gbSettingsWS->Controls->Add(this->nUD_WS_SpeedEVL);
			this->gbSettingsWS->Controls->Add(this->nUD_WS_MigEVL);
			this->gbSettingsWS->Controls->Add(this->label6);
			this->gbSettingsWS->Controls->Add(this->nUD_WS_H);
			this->gbSettingsWS->Controls->Add(this->labelWSH);
			this->gbSettingsWS->Controls->Add(this->nUD_WS_W);
			this->gbSettingsWS->Controls->Add(this->labelW);
			this->gbSettingsWS->Location = System::Drawing::Point(7, 113);
			this->gbSettingsWS->Name = L"gbSettingsWS";
			this->gbSettingsWS->Size = System::Drawing::Size(301, 151);
			this->gbSettingsWS->TabIndex = 5;
			this->gbSettingsWS->TabStop = false;
			this->gbSettingsWS->Text = L"Setting Work Space";
			// 
			// nUD_WS_H
			// 
			this->nUD_WS_H->DecimalPlaces = 1;
			this->nUD_WS_H->Location = System::Drawing::Point(163, 54);
			this->nUD_WS_H->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_WS_H->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_WS_H->Name = L"nUD_WS_H";
			this->nUD_WS_H->Size = System::Drawing::Size(108, 20);
			this->nUD_WS_H->TabIndex = 5;
			this->nUD_WS_H->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {400, 0, 0, 0});
			this->nUD_WS_H->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_WS_H_ValueChanged);
			// 
			// labelWSH
			// 
			this->labelWSH->AutoSize = true;
			this->labelWSH->Location = System::Drawing::Point(18, 54);
			this->labelWSH->Name = L"labelWSH";
			this->labelWSH->Size = System::Drawing::Size(61, 13);
			this->labelWSH->TabIndex = 6;
			this->labelWSH->Text = L"Height (um)";
			// 
			// nUD_WS_W
			// 
			this->nUD_WS_W->DecimalPlaces = 1;
			this->nUD_WS_W->Location = System::Drawing::Point(163, 27);
			this->nUD_WS_W->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_WS_W->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_WS_W->Name = L"nUD_WS_W";
			this->nUD_WS_W->Size = System::Drawing::Size(108, 20);
			this->nUD_WS_W->TabIndex = 4;
			this->nUD_WS_W->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {400, 0, 0, 0});
			this->nUD_WS_W->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_WS_W_ValueChanged);
			// 
			// labelW
			// 
			this->labelW->AutoSize = true;
			this->labelW->Location = System::Drawing::Point(18, 27);
			this->labelW->Name = L"labelW";
			this->labelW->Size = System::Drawing::Size(58, 13);
			this->labelW->TabIndex = 4;
			this->labelW->Text = L"Width (um)";
			// 
			// bUpdateSettings
			// 
			this->bUpdateSettings->Location = System::Drawing::Point(28, 71);
			this->bUpdateSettings->Name = L"bUpdateSettings";
			this->bUpdateSettings->Size = System::Drawing::Size(108, 23);
			this->bUpdateSettings->TabIndex = 4;
			this->bUpdateSettings->Text = L"Update Settings";
			this->bUpdateSettings->UseVisualStyleBackColor = true;
			this->bUpdateSettings->Click += gcnew System::EventHandler(this, &MainWindow::bUpdateSettings_Click);
			// 
			// bStopSim
			// 
			this->bStopSim->Location = System::Drawing::Point(170, 32);
			this->bStopSim->Name = L"bStopSim";
			this->bStopSim->Size = System::Drawing::Size(108, 23);
			this->bStopSim->TabIndex = 1;
			this->bStopSim->Text = L"Stop Simulation";
			this->bStopSim->UseVisualStyleBackColor = true;
			this->bStopSim->Click += gcnew System::EventHandler(this, &MainWindow::bStopSim_Click);
			// 
			// bSartSim
			// 
			this->bSartSim->Location = System::Drawing::Point(28, 32);
			this->bSartSim->Name = L"bSartSim";
			this->bSartSim->Size = System::Drawing::Size(108, 23);
			this->bSartSim->TabIndex = 0;
			this->bSartSim->Text = L"Start Simulation";
			this->bSartSim->UseVisualStyleBackColor = true;
			this->bSartSim->Click += gcnew System::EventHandler(this, &MainWindow::bSartSim_Click);
			// 
			// tpOptions2
			// 
			this->tpOptions2->Location = System::Drawing::Point(4, 22);
			this->tpOptions2->Name = L"tpOptions2";
			this->tpOptions2->Padding = System::Windows::Forms::Padding(3);
			this->tpOptions2->Size = System::Drawing::Size(314, 928);
			this->tpOptions2->TabIndex = 1;
			this->tpOptions2->Text = L"More";
			this->tpOptions2->UseVisualStyleBackColor = true;
			// 
			// bwControl
			// 
			this->bwControl->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainWindow::bwControl_DoWork);
			// 
			// bwDraw
			// 
			this->bwDraw->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainWindow::bwDraw_DoWork);
			// 
			// nUD_WS_MigEVL
			// 
			this->nUD_WS_MigEVL->DecimalPlaces = 1;
			this->nUD_WS_MigEVL->Location = System::Drawing::Point(163, 96);
			this->nUD_WS_MigEVL->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_WS_MigEVL->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_WS_MigEVL->Name = L"nUD_WS_MigEVL";
			this->nUD_WS_MigEVL->Size = System::Drawing::Size(108, 20);
			this->nUD_WS_MigEVL->TabIndex = 7;
			this->nUD_WS_MigEVL->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {350, 0, 0, 0});
			this->nUD_WS_MigEVL->ValueChanged += gcnew System::EventHandler(this, &MainWindow::nUD_WS_MigEVL_ValueChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(18, 96);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(131, 13);
			this->label6->TabIndex = 8;
			this->label6->Text = L"Migration EVL Margin (um)";
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->DecimalPlaces = 1;
			this->numericUpDown2->Location = System::Drawing::Point(163, 123);
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(108, 20);
			this->numericUpDown2->TabIndex = 9;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 65536});
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &MainWindow::numericUpDown2_ValueChanged);
			// 
			// nUD_WS_SpeedEVL
			// 
			this->nUD_WS_SpeedEVL->AutoSize = true;
			this->nUD_WS_SpeedEVL->Location = System::Drawing::Point(18, 123);
			this->nUD_WS_SpeedEVL->Name = L"nUD_WS_SpeedEVL";
			this->nUD_WS_SpeedEVL->Size = System::Drawing::Size(140, 13);
			this->nUD_WS_SpeedEVL->TabIndex = 10;
			this->nUD_WS_SpeedEVL->Text = L"Speed EVL Margin (um/min)";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1358, 703);
			this->Controls->Add(this->tlpMain);
			this->Controls->Add(this->menuMain);
			this->MainMenuStrip = this->menuMain;
			this->MaximizeBox = false;
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
			this->Load += gcnew System::EventHandler(this, &MainWindow::MainWindow_Load);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->tlpMain->ResumeLayout(false);
			this->tcGraphics->ResumeLayout(false);
			this->tpGraphs1->ResumeLayout(false);
			this->tcOptions->ResumeLayout(false);
			this->tpOptions1->ResumeLayout(false);
			this->gbSettingsDFC_Group->ResumeLayout(false);
			this->gbSettingsDFC_Group->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialNumDFCs))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsGap))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsH))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_InitialDFCsW))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_MarginGap))->EndInit();
			this->gbSettingsDFC_Individual->ResumeLayout(false);
			this->gbSettingsDFC_Individual->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFC_AdhesionDist))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFCRadius))->EndInit();
			this->gbSettingsWS->ResumeLayout(false);
			this->gbSettingsWS->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_H))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_W))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_WS_MigEVL))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
	private: System::Void MainWindow_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void bwDraw_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
	private: System::Void bwControl_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
	private: System::Void bSartSim_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void bStopSim_Click(System::Object^  sender, System::EventArgs^  e);
	//Main updater
	private: System::Void bUpdateSettings_Click(System::Object^  sender, System::EventArgs^  e);
    // WS
	private: System::Void nUD_WS_W_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_WS_H_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_WS_MigEVL_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e);

    // DFCs Group
	private: System::Void nUD_MarginGap_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_InitialDFCsW_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_InitialDFCsH_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_InitialDFCsGap_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_InitialNumDFCs_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	//DFC Individual
	private: System::Void nUD_DFCRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void nUD_DFC_AdhesionDist_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	};
}
