#pragma once

#include "Sistema_Agentes.h"
#include "Ambiente.h"

#include "AgentsSystem.h"
#include "Enviroment.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace DinHotSys {

	/// <summary>
	/// Summary for SistemaControl
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SistemaControl : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::TabPage^  tabPage6;
	private: System::Windows::Forms::Button^  bStartSim;
	private: System::Windows::Forms::TabControl^  tCMoreOptions;
	private: System::Windows::Forms::TabPage^  tabPage7;
	private: System::Windows::Forms::TabPage^  tabTIMETASKS;
	private: System::Windows::Forms::Form ^_ventanaPrincipal;
	private: System::Windows::Forms::MenuStrip^  MenuPrincipal;
	private: System::Windows::Forms::ToolStripMenuItem^  archivoToolStripMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::ComponentModel::BackgroundWorker^  bwControl;
	private: System::ComponentModel::BackgroundWorker^  bwDrawMask;
	private: System::Windows::Forms::TabControl^  tabControlGraph;
	private: System::Windows::Forms::TabPage^  tabPage0;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
	private: System::Windows::Forms::Panel^  panelTab0Izq;
	private: System::Windows::Forms::Panel^  panelTab0Der;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::ComponentModel::BackgroundWorker^  bwDraw;
	public: 
		#ifdef TIME_TASKS
			float TFrame, TRenderTotal,TRenderTrampa, TRenderMV, TCalcMaskV;
		#endif
//	public:	Sistema_Agentes		*_theMatrixAgents_Dynamic;
//	public:	Sistema_Agentes		*_theMatrixAgents_Static;
	public:	Ambiente			*_theMatrixAmbiente;

	public:	Sistema_Agentes		*_theMatrixAgents_Dynamic;
	public:	AgentsSystem		*_theMatrixAgents_Static;
	public:	Enviroment			*_theMatrixEnvironment;

	public:	int					_numDFCs;

	public: bool _Borrar,_CrearMover;
	private: System::Windows::Forms::TextBox^  tBTimeRenderT;
	public: 

	public: 
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  tBTimeCalc;


	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  tBTimeRenderSim;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::GroupBox^  groupModTrampas;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  bDelete;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  bCrear;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  numericZTrampa;
	private: System::Windows::Forms::NumericUpDown^  numericYTrampa;
	private: System::Windows::Forms::NumericUpDown^  numericXTrampa;
	private: System::Windows::Forms::NumericUpDown^  numericIndiceTrampa;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Button^  bCrearObjetos;

	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::NumericUpDown^  numericRectAlto;

	private: System::Windows::Forms::NumericUpDown^  numericRectY;
	private: System::Windows::Forms::NumericUpDown^  numericRectAncho;



	private: System::Windows::Forms::NumericUpDown^  numericRectX;



	private: System::Windows::Forms::Button^  bModCampo;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::NumericUpDown^  numericYCampo;

private: System::Windows::Forms::NumericUpDown^  numericXCampo;

	private: System::Windows::Forms::Button^  bModRect;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::NumericUpDown^  numericNumObjetos;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::NumericUpDown^  numericRadioObjetos;
private: System::Windows::Forms::Button^  bStopSim;






private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::NumericUpDown^  numericDelta;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::NumericUpDown^  numericDeadTime;
private: System::Windows::Forms::TabControl^  tabControl3;

private: System::Windows::Forms::TabPage^  tabPage8;
private: System::Windows::Forms::TabPage^  tabPage4;
private: System::Windows::Forms::CheckBox^  cB_TryFreeStagnant;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::CheckBox^  cB_RandPerturbation;
private: System::Windows::Forms::NumericUpDown^  nUD_NumDFCs;
private: System::Windows::Forms::CheckBox^  cB_DEBAttraction;
private: System::Windows::Forms::Label^  label24;

private: System::Windows::Forms::NumericUpDown^  nUD_StepsForChange;
private: System::Windows::Forms::CheckBox^  cB_EVLInteraction;
private: System::Windows::Forms::NumericUpDown^  nUD_RadInteraction;
private: System::Windows::Forms::CheckBox^  cB_DFCInteraction;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::NumericUpDown^  nUD_DFC_Radius;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::NumericUpDown^  nUD_Ratio_DFC_DEB;
private: System::Windows::Forms::RadioButton^  rB_EVL_Global;
private: System::Windows::Forms::RadioButton^  rB_EVL_Selected;
private: System::Windows::Forms::Panel^  panel1;
private: System::Windows::Forms::RadioButton^  rB_tipoEVL_Linea;

private: System::Windows::Forms::RadioButton^  rB_tipoEVL_Puntual;
private: System::Windows::Forms::RadioButton^  rB_tipoEVL_Distribuido;


private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::NumericUpDown^  nUD_DataEVL_StepChangeDist;

private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::NumericUpDown^  nUD_DataEVL_StepRedLine;

private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::NumericUpDown^  nUD_DataEVL_PosY;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::NumericUpDown^  nUD_DataEVL_AnchoLinea;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::NumericUpDown^  nUD_EVL_Factor;
private: System::Windows::Forms::CheckBox^  cB_AdhesionEVL;
private: System::Windows::Forms::NumericUpDown^  nUD_StepsMovDB_margin;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::NumericUpDown^  nUD_StepsEVL_Mov;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Panel^  panel2;
private: System::Windows::Forms::RadioButton^  rB_tipoEVL_DistMarginal;

private: System::Windows::Forms::RadioButton^  rB_tipoEVL_marginal;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::NumericUpDown^  nUP_ProliferacionSteps;





























	private: System::Windows::Forms::Label^  label13;








	private: delegate void DelegateThreadTask();
	private: void funcionActualizacion(void);
	public:
		SistemaControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		SistemaControl(System::Windows::Forms::Form ^vPrincipal)
		{
			_ventanaPrincipal = vPrincipal;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SistemaControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->MenuPrincipal = (gcnew System::Windows::Forms::MenuStrip());
			this->archivoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tabControlGraph = (gcnew System::Windows::Forms::TabControl());
			this->tabPage0 = (gcnew System::Windows::Forms::TabPage());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panelTab0Izq = (gcnew System::Windows::Forms::Panel());
			this->panelTab0Der = (gcnew System::Windows::Forms::Panel());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupModTrampas = (gcnew System::Windows::Forms::GroupBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->bDelete = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->bCrear = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->numericZTrampa = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericYTrampa = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericXTrampa = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericIndiceTrampa = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->numericRadioObjetos = (gcnew System::Windows::Forms::NumericUpDown());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->numericNumObjetos = (gcnew System::Windows::Forms::NumericUpDown());
			this->bModCampo = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->numericYCampo = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericXCampo = (gcnew System::Windows::Forms::NumericUpDown());
			this->bModRect = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->bCrearObjetos = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->numericRectAlto = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericRectY = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericRectAncho = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericRectX = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl3 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->nUD_StepsEVL_Mov = (gcnew System::Windows::Forms::NumericUpDown());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->nUD_StepsMovDB_margin = (gcnew System::Windows::Forms::NumericUpDown());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->cB_AdhesionEVL = (gcnew System::Windows::Forms::CheckBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->rB_tipoEVL_DistMarginal = (gcnew System::Windows::Forms::RadioButton());
			this->rB_tipoEVL_marginal = (gcnew System::Windows::Forms::RadioButton());
			this->rB_tipoEVL_Distribuido = (gcnew System::Windows::Forms::RadioButton());
			this->nUD_DataEVL_StepChangeDist = (gcnew System::Windows::Forms::NumericUpDown());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->nUD_DataEVL_AnchoLinea = (gcnew System::Windows::Forms::NumericUpDown());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->nUD_DataEVL_StepRedLine = (gcnew System::Windows::Forms::NumericUpDown());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->nUD_DataEVL_PosY = (gcnew System::Windows::Forms::NumericUpDown());
			this->rB_tipoEVL_Linea = (gcnew System::Windows::Forms::RadioButton());
			this->rB_tipoEVL_Puntual = (gcnew System::Windows::Forms::RadioButton());
			this->nUD_EVL_Factor = (gcnew System::Windows::Forms::NumericUpDown());
			this->rB_EVL_Selected = (gcnew System::Windows::Forms::RadioButton());
			this->rB_EVL_Global = (gcnew System::Windows::Forms::RadioButton());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->nUD_Ratio_DFC_DEB = (gcnew System::Windows::Forms::NumericUpDown());
			this->cB_EVLInteraction = (gcnew System::Windows::Forms::CheckBox());
			this->cB_DEBAttraction = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage8 = (gcnew System::Windows::Forms::TabPage());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->nUP_ProliferacionSteps = (gcnew System::Windows::Forms::NumericUpDown());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->cB_DFCInteraction = (gcnew System::Windows::Forms::CheckBox());
			this->nUD_RadInteraction = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUD_DFC_Radius = (gcnew System::Windows::Forms::NumericUpDown());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->nUD_StepsForChange = (gcnew System::Windows::Forms::NumericUpDown());
			this->cB_TryFreeStagnant = (gcnew System::Windows::Forms::CheckBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->cB_RandPerturbation = (gcnew System::Windows::Forms::CheckBox());
			this->nUD_NumDFCs = (gcnew System::Windows::Forms::NumericUpDown());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->numericDeadTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->numericDelta = (gcnew System::Windows::Forms::NumericUpDown());
			this->bStopSim = (gcnew System::Windows::Forms::Button());
			this->bStartSim = (gcnew System::Windows::Forms::Button());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->tCMoreOptions = (gcnew System::Windows::Forms::TabControl());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->tabTIMETASKS = (gcnew System::Windows::Forms::TabPage());
			this->tBTimeCalc = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->tBTimeRenderSim = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->tBTimeRenderT = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->bwControl = (gcnew System::ComponentModel::BackgroundWorker());
			this->bwDraw = (gcnew System::ComponentModel::BackgroundWorker());
			this->bwDrawMask = (gcnew System::ComponentModel::BackgroundWorker());
			this->MenuPrincipal->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->tabControlGraph->SuspendLayout();
			this->tabPage0->SuspendLayout();
			this->tableLayoutPanel2->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupModTrampas->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericZTrampa))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericYTrampa))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericXTrampa))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericIndiceTrampa))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRadioObjetos))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericNumObjetos))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericYCampo))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericXCampo))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectAlto))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectAncho))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectX))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->tabControl3->SuspendLayout();
			this->tabPage4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsEVL_Mov))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsMovDB_margin))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_StepChangeDist))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_AnchoLinea))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_StepRedLine))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_PosY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_EVL_Factor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_Ratio_DFC_DEB))->BeginInit();
			this->tabPage8->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUP_ProliferacionSteps))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_RadInteraction))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFC_Radius))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsForChange))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_NumDFCs))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDeadTime))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDelta))->BeginInit();
			this->tCMoreOptions->SuspendLayout();
			this->tabTIMETASKS->SuspendLayout();
			this->SuspendLayout();
			// 
			// MenuPrincipal
			// 
			this->MenuPrincipal->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->archivoToolStripMenuItem});
			this->MenuPrincipal->Location = System::Drawing::Point(0, 0);
			this->MenuPrincipal->Name = L"MenuPrincipal";
			this->MenuPrincipal->Size = System::Drawing::Size(1251, 24);
			this->MenuPrincipal->TabIndex = 1;
			this->MenuPrincipal->Text = L"Menú";
			// 
			// archivoToolStripMenuItem
			// 
			this->archivoToolStripMenuItem->Name = L"archivoToolStripMenuItem";
			this->archivoToolStripMenuItem->Size = System::Drawing::Size(60, 20);
			this->archivoToolStripMenuItem->Text = L"Archivo";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				10)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				80)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				10)));
			this->tableLayoutPanel1->Controls->Add(this->tabControlGraph, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->tabControl1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->tCMoreOptions, 1, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 24);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 4.965243F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 93.74379F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 1.22905F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1251, 801);
			this->tableLayoutPanel1->TabIndex = 2;
			// 
			// tabControlGraph
			// 
			this->tabControlGraph->Controls->Add(this->tabPage0);
			this->tabControlGraph->Controls->Add(this->tabPage1);
			this->tabControlGraph->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlGraph->Location = System::Drawing::Point(128, 42);
			this->tabControlGraph->Name = L"tabControlGraph";
			this->tabControlGraph->SelectedIndex = 0;
			this->tabControlGraph->Size = System::Drawing::Size(994, 745);
			this->tabControlGraph->TabIndex = 4;
			// 
			// tabPage0
			// 
			this->tabPage0->Controls->Add(this->tableLayoutPanel2);
			this->tabPage0->Location = System::Drawing::Point(4, 22);
			this->tabPage0->Name = L"tabPage0";
			this->tabPage0->Size = System::Drawing::Size(986, 719);
			this->tabPage0->TabIndex = 4;
			this->tabPage0->Text = L"Intial State";
			this->tabPage0->UseVisualStyleBackColor = true;
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->ColumnCount = 3;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				45.71429F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				8.637874F)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				45.64784F)));
			this->tableLayoutPanel2->Controls->Add(this->panelTab0Izq, 0, 0);
			this->tableLayoutPanel2->Controls->Add(this->panelTab0Der, 2, 0);
			this->tableLayoutPanel2->Controls->Add(this->tabControl2, 0, 1);
			this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel2->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 2;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 80.49065F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 19.50935F)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(986, 719);
			this->tableLayoutPanel2->TabIndex = 0;
			// 
			// panelTab0Izq
			// 
			this->panelTab0Izq->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(115)), static_cast<System::Int32>(static_cast<System::Byte>(139)), 
				static_cast<System::Int32>(static_cast<System::Byte>(221)));
			this->panelTab0Izq->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelTab0Izq->Cursor = System::Windows::Forms::Cursors::Hand;
			this->panelTab0Izq->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelTab0Izq->Location = System::Drawing::Point(3, 3);
			this->panelTab0Izq->Name = L"panelTab0Izq";
			this->panelTab0Izq->Size = System::Drawing::Size(444, 572);
			this->panelTab0Izq->TabIndex = 5;
			this->panelTab0Izq->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &SistemaControl::panelTab0Izq_Paint);
			this->panelTab0Izq->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &SistemaControl::panelTab0Izq_MouseDown);
			this->panelTab0Izq->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &SistemaControl::panelTab0Izq_MouseMove);
			this->panelTab0Izq->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &SistemaControl::panelTab0Izq_MouseUp);
			// 
			// panelTab0Der
			// 
			this->panelTab0Der->AutoScroll = true;
			this->panelTab0Der->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->panelTab0Der->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelTab0Der->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelTab0Der->Location = System::Drawing::Point(538, 3);
			this->panelTab0Der->Name = L"panelTab0Der";
			this->panelTab0Der->Size = System::Drawing::Size(445, 572);
			this->panelTab0Der->TabIndex = 6;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPage2);
			this->tabControl2->Controls->Add(this->tabPage3);
			this->tabControl2->Location = System::Drawing::Point(2, 580);
			this->tabControl2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(446, 132);
			this->tabControl2->TabIndex = 7;
			this->tabControl2->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &SistemaControl::tabControl2_Selected);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupModTrampas);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage2->Size = System::Drawing::Size(438, 106);
			this->tabPage2->TabIndex = 0;
			this->tabPage2->Text = L"Mod Objetos";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupModTrampas
			// 
			this->groupModTrampas->Controls->Add(this->label7);
			this->groupModTrampas->Controls->Add(this->bDelete);
			this->groupModTrampas->Controls->Add(this->label3);
			this->groupModTrampas->Controls->Add(this->label6);
			this->groupModTrampas->Controls->Add(this->label2);
			this->groupModTrampas->Controls->Add(this->label5);
			this->groupModTrampas->Controls->Add(this->bCrear);
			this->groupModTrampas->Controls->Add(this->label1);
			this->groupModTrampas->Controls->Add(this->label4);
			this->groupModTrampas->Controls->Add(this->numericZTrampa);
			this->groupModTrampas->Controls->Add(this->numericYTrampa);
			this->groupModTrampas->Controls->Add(this->numericXTrampa);
			this->groupModTrampas->Controls->Add(this->numericIndiceTrampa);
			this->groupModTrampas->Location = System::Drawing::Point(0, 6);
			this->groupModTrampas->Name = L"groupModTrampas";
			this->groupModTrampas->Size = System::Drawing::Size(500, 100);
			this->groupModTrampas->TabIndex = 11;
			this->groupModTrampas->TabStop = false;
			this->groupModTrampas->Text = L"Modificacion Objetos";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(343, 48);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(75, 13);
			this->label7->TabIndex = 7;
			this->label7->Text = L"Coordenada Z";
			// 
			// bDelete
			// 
			this->bDelete->Location = System::Drawing::Point(26, 71);
			this->bDelete->Name = L"bDelete";
			this->bDelete->Size = System::Drawing::Size(75, 23);
			this->bDelete->TabIndex = 0;
			this->bDelete->Text = L"Destruir";
			this->bDelete->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(83, 57);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(37, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Actual";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(193, 48);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(75, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Coordenada Y";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Eliminar Objeto";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(343, 10);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(75, 13);
			this->label5->TabIndex = 5;
			this->label5->Text = L"Coordenada X";
			// 
			// bCrear
			// 
			this->bCrear->Location = System::Drawing::Point(26, 32);
			this->bCrear->Name = L"bCrear";
			this->bCrear->Size = System::Drawing::Size(75, 23);
			this->bCrear->TabIndex = 1;
			this->bCrear->Text = L"Crear";
			this->bCrear->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(78, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Agregar Objeto";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(193, 10);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(70, 13);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Indice Objeto";
			// 
			// numericZTrampa
			// 
			this->numericZTrampa->DecimalPlaces = 4;
			this->numericZTrampa->Location = System::Drawing::Point(320, 64);
			this->numericZTrampa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericZTrampa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericZTrampa->Name = L"numericZTrampa";
			this->numericZTrampa->Size = System::Drawing::Size(120, 20);
			this->numericZTrampa->TabIndex = 3;
			this->numericZTrampa->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericZTrampa_ValueChanged);
			// 
			// numericYTrampa
			// 
			this->numericYTrampa->DecimalPlaces = 4;
			this->numericYTrampa->Location = System::Drawing::Point(168, 64);
			this->numericYTrampa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericYTrampa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericYTrampa->Name = L"numericYTrampa";
			this->numericYTrampa->Size = System::Drawing::Size(120, 20);
			this->numericYTrampa->TabIndex = 2;
			this->numericYTrampa->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericYTrampa_ValueChanged);
			// 
			// numericXTrampa
			// 
			this->numericXTrampa->DecimalPlaces = 4;
			this->numericXTrampa->Location = System::Drawing::Point(320, 27);
			this->numericXTrampa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericXTrampa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericXTrampa->Name = L"numericXTrampa";
			this->numericXTrampa->Size = System::Drawing::Size(120, 20);
			this->numericXTrampa->TabIndex = 1;
			this->numericXTrampa->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericXTrampa_ValueChanged);
			// 
			// numericIndiceTrampa
			// 
			this->numericIndiceTrampa->Location = System::Drawing::Point(168, 27);
			this->numericIndiceTrampa->Name = L"numericIndiceTrampa";
			this->numericIndiceTrampa->Size = System::Drawing::Size(120, 20);
			this->numericIndiceTrampa->TabIndex = 0;
			this->numericIndiceTrampa->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericIndiceTrampa_ValueChanged);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->label18);
			this->tabPage3->Controls->Add(this->numericRadioObjetos);
			this->tabPage3->Controls->Add(this->label17);
			this->tabPage3->Controls->Add(this->numericNumObjetos);
			this->tabPage3->Controls->Add(this->bModCampo);
			this->tabPage3->Controls->Add(this->label11);
			this->tabPage3->Controls->Add(this->label15);
			this->tabPage3->Controls->Add(this->numericYCampo);
			this->tabPage3->Controls->Add(this->numericXCampo);
			this->tabPage3->Controls->Add(this->bModRect);
			this->tabPage3->Controls->Add(this->label8);
			this->tabPage3->Controls->Add(this->label9);
			this->tabPage3->Controls->Add(this->label10);
			this->tabPage3->Controls->Add(this->bCrearObjetos);
			this->tabPage3->Controls->Add(this->label14);
			this->tabPage3->Controls->Add(this->numericRectAlto);
			this->tabPage3->Controls->Add(this->numericRectY);
			this->tabPage3->Controls->Add(this->numericRectAncho);
			this->tabPage3->Controls->Add(this->numericRectX);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage3->Size = System::Drawing::Size(503, 106);
			this->tabPage3->TabIndex = 1;
			this->tabPage3->Text = L"Elementos Contexto";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(184, 68);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(74, 13);
			this->label18->TabIndex = 27;
			this->label18->Text = L"Radio Objetos";
			// 
			// numericRadioObjetos
			// 
			this->numericRadioObjetos->DecimalPlaces = 4;
			this->numericRadioObjetos->Location = System::Drawing::Point(186, 85);
			this->numericRadioObjetos->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericRadioObjetos->Name = L"numericRadioObjetos";
			this->numericRadioObjetos->Size = System::Drawing::Size(75, 20);
			this->numericRadioObjetos->TabIndex = 26;
			this->numericRadioObjetos->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->numericRadioObjetos->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericRadioObjetos_ValueChanged);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(184, 30);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(68, 13);
			this->label17->TabIndex = 25;
			this->label17->Text = L"Num Objetos";
			// 
			// numericNumObjetos
			// 
			this->numericNumObjetos->DecimalPlaces = 4;
			this->numericNumObjetos->Location = System::Drawing::Point(186, 47);
			this->numericNumObjetos->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericNumObjetos->Name = L"numericNumObjetos";
			this->numericNumObjetos->Size = System::Drawing::Size(75, 20);
			this->numericNumObjetos->TabIndex = 24;
			this->numericNumObjetos->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->numericNumObjetos->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericNumObjetos_ValueChanged);
			// 
			// bModCampo
			// 
			this->bModCampo->Location = System::Drawing::Point(370, 6);
			this->bModCampo->Name = L"bModCampo";
			this->bModCampo->Size = System::Drawing::Size(120, 23);
			this->bModCampo->TabIndex = 23;
			this->bModCampo->Text = L"Mod Fin Campo";
			this->bModCampo->UseVisualStyleBackColor = true;
			this->bModCampo->Click += gcnew System::EventHandler(this, &SistemaControl::bModCampo_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(368, 68);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(35, 13);
			this->label11->TabIndex = 22;
			this->label11->Text = L"Pos Y";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(368, 30);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(35, 13);
			this->label15->TabIndex = 21;
			this->label15->Text = L"Pos X";
			// 
			// numericYCampo
			// 
			this->numericYCampo->DecimalPlaces = 4;
			this->numericYCampo->Location = System::Drawing::Point(370, 84);
			this->numericYCampo->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericYCampo->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericYCampo->Name = L"numericYCampo";
			this->numericYCampo->Size = System::Drawing::Size(120, 20);
			this->numericYCampo->TabIndex = 20;
			this->numericYCampo->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericYCampo_ValueChanged);
			// 
			// numericXCampo
			// 
			this->numericXCampo->DecimalPlaces = 4;
			this->numericXCampo->Location = System::Drawing::Point(370, 47);
			this->numericXCampo->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericXCampo->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericXCampo->Name = L"numericXCampo";
			this->numericXCampo->Size = System::Drawing::Size(120, 20);
			this->numericXCampo->TabIndex = 19;
			this->numericXCampo->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericXCampo_ValueChanged);
			// 
			// bModRect
			// 
			this->bModRect->Location = System::Drawing::Point(96, 6);
			this->bModRect->Name = L"bModRect";
			this->bModRect->Size = System::Drawing::Size(96, 23);
			this->bModRect->TabIndex = 18;
			this->bModRect->Text = L"Mod Rectangulo";
			this->bModRect->UseVisualStyleBackColor = true;
			this->bModRect->Click += gcnew System::EventHandler(this, &SistemaControl::bModRect_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(95, 68);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(25, 13);
			this->label8->TabIndex = 17;
			this->label8->Text = L"Alto";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(3, 67);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(63, 13);
			this->label9->TabIndex = 16;
			this->label9->Text = L"Pos Inicio Y";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(95, 30);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(38, 13);
			this->label10->TabIndex = 15;
			this->label10->Text = L"Ancho";
			// 
			// bCrearObjetos
			// 
			this->bCrearObjetos->Location = System::Drawing::Point(5, 6);
			this->bCrearObjetos->Name = L"bCrearObjetos";
			this->bCrearObjetos->Size = System::Drawing::Size(80, 23);
			this->bCrearObjetos->TabIndex = 9;
			this->bCrearObjetos->Text = L"Crear Objetos";
			this->bCrearObjetos->UseVisualStyleBackColor = true;
			this->bCrearObjetos->Click += gcnew System::EventHandler(this, &SistemaControl::bCrearObjetos_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(4, 30);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(63, 13);
			this->label14->TabIndex = 14;
			this->label14->Text = L"Pos Inicio X";
			// 
			// numericRectAlto
			// 
			this->numericRectAlto->DecimalPlaces = 4;
			this->numericRectAlto->Location = System::Drawing::Point(98, 84);
			this->numericRectAlto->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {680, 0, 0, 0});
			this->numericRectAlto->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericRectAlto->Name = L"numericRectAlto";
			this->numericRectAlto->Size = System::Drawing::Size(75, 20);
			this->numericRectAlto->TabIndex = 13;
			this->numericRectAlto->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericRectAlto->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericRectAlto_ValueChanged);
			// 
			// numericRectY
			// 
			this->numericRectY->DecimalPlaces = 4;
			this->numericRectY->Location = System::Drawing::Point(5, 84);
			this->numericRectY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericRectY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericRectY->Name = L"numericRectY";
			this->numericRectY->Size = System::Drawing::Size(80, 20);
			this->numericRectY->TabIndex = 11;
			this->numericRectY->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericRectY_ValueChanged);
			// 
			// numericRectAncho
			// 
			this->numericRectAncho->DecimalPlaces = 4;
			this->numericRectAncho->Location = System::Drawing::Point(98, 47);
			this->numericRectAncho->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {680, 0, 0, 0});
			this->numericRectAncho->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericRectAncho->Name = L"numericRectAncho";
			this->numericRectAncho->Size = System::Drawing::Size(75, 20);
			this->numericRectAncho->TabIndex = 10;
			this->numericRectAncho->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericRectAncho->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericRectAncho_ValueChanged);
			// 
			// numericRectX
			// 
			this->numericRectX->Location = System::Drawing::Point(5, 47);
			this->numericRectX->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, 0});
			this->numericRectX->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {340, 0, 0, System::Int32::MinValue});
			this->numericRectX->Name = L"numericRectX";
			this->numericRectX->Size = System::Drawing::Size(80, 20);
			this->numericRectX->TabIndex = 8;
			this->numericRectX->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericRectX_ValueChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3, 3, 3, 3);
			this->tabPage1->Size = System::Drawing::Size(937, 719);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"En desarrollo";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Controls->Add(this->tabPage6);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(3, 42);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(119, 745);
			this->tabControl1->TabIndex = 10;
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->tabControl3);
			this->tabPage5->Controls->Add(this->label22);
			this->tabPage5->Controls->Add(this->numericDeadTime);
			this->tabPage5->Controls->Add(this->label19);
			this->tabPage5->Controls->Add(this->numericDelta);
			this->tabPage5->Controls->Add(this->bStopSim);
			this->tabPage5->Controls->Add(this->bStartSim);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3, 3, 3, 3);
			this->tabPage5->Size = System::Drawing::Size(111, 719);
			this->tabPage5->TabIndex = 0;
			this->tabPage5->Text = L"Mod";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// tabControl3
			// 
			this->tabControl3->Controls->Add(this->tabPage4);
			this->tabControl3->Controls->Add(this->tabPage8);
			this->tabControl3->Location = System::Drawing::Point(2, 180);
			this->tabControl3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabControl3->Name = L"tabControl3";
			this->tabControl3->SelectedIndex = 0;
			this->tabControl3->Size = System::Drawing::Size(131, 518);
			this->tabControl3->TabIndex = 18;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->nUD_StepsEVL_Mov);
			this->tabPage4->Controls->Add(this->label33);
			this->tabPage4->Controls->Add(this->nUD_StepsMovDB_margin);
			this->tabPage4->Controls->Add(this->label32);
			this->tabPage4->Controls->Add(this->cB_AdhesionEVL);
			this->tabPage4->Controls->Add(this->label31);
			this->tabPage4->Controls->Add(this->panel1);
			this->tabPage4->Controls->Add(this->nUD_EVL_Factor);
			this->tabPage4->Controls->Add(this->rB_EVL_Selected);
			this->tabPage4->Controls->Add(this->rB_EVL_Global);
			this->tabPage4->Controls->Add(this->label26);
			this->tabPage4->Controls->Add(this->nUD_Ratio_DFC_DEB);
			this->tabPage4->Controls->Add(this->cB_EVLInteraction);
			this->tabPage4->Controls->Add(this->cB_DEBAttraction);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage4->Size = System::Drawing::Size(123, 492);
			this->tabPage4->TabIndex = 0;
			this->tabPage4->Text = L"Env";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// nUD_StepsEVL_Mov
			// 
			this->nUD_StepsEVL_Mov->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_StepsEVL_Mov->Location = System::Drawing::Point(26, 183);
			this->nUD_StepsEVL_Mov->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_StepsEVL_Mov->Name = L"nUD_StepsEVL_Mov";
			this->nUD_StepsEVL_Mov->Size = System::Drawing::Size(40, 20);
			this->nUD_StepsEVL_Mov->TabIndex = 57;
			this->nUD_StepsEVL_Mov->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_StepsEVL_Mov->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_StepsEVL_Mov_ValueChanged);
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(20, 166);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(110, 13);
			this->label33->TabIndex = 56;
			this->label33->Text = L"StepsMovEVL_Group";
			// 
			// nUD_StepsMovDB_margin
			// 
			this->nUD_StepsMovDB_margin->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_StepsMovDB_margin->Location = System::Drawing::Point(22, 50);
			this->nUD_StepsMovDB_margin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_StepsMovDB_margin->Name = L"nUD_StepsMovDB_margin";
			this->nUD_StepsMovDB_margin->Size = System::Drawing::Size(40, 20);
			this->nUD_StepsMovDB_margin->TabIndex = 55;
			this->nUD_StepsMovDB_margin->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_StepsMovDB_margin->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_StepsMovDB_margin_ValueChanged);
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(17, 32);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(108, 13);
			this->label32->TabIndex = 54;
			this->label32->Text = L"StepsMovDB_Margin";
			// 
			// cB_AdhesionEVL
			// 
			this->cB_AdhesionEVL->AutoSize = true;
			this->cB_AdhesionEVL->Location = System::Drawing::Point(22, 146);
			this->cB_AdhesionEVL->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_AdhesionEVL->Name = L"cB_AdhesionEVL";
			this->cB_AdhesionEVL->Size = System::Drawing::Size(127, 17);
			this->cB_AdhesionEVL->TabIndex = 52;
			this->cB_AdhesionEVL->Text = L"EVL Strong Adhesion";
			this->cB_AdhesionEVL->UseVisualStyleBackColor = true;
			this->cB_AdhesionEVL->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_AdhesionEVL_CheckedChanged);
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(92, 255);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(54, 13);
			this->label31->TabIndex = 51;
			this->label31->Text = L"factorEVL";
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Controls->Add(this->nUD_DataEVL_StepChangeDist);
			this->panel1->Controls->Add(this->label29);
			this->panel1->Controls->Add(this->label30);
			this->panel1->Controls->Add(this->nUD_DataEVL_AnchoLinea);
			this->panel1->Controls->Add(this->label28);
			this->panel1->Controls->Add(this->nUD_DataEVL_StepRedLine);
			this->panel1->Controls->Add(this->label27);
			this->panel1->Controls->Add(this->nUD_DataEVL_PosY);
			this->panel1->Controls->Add(this->rB_tipoEVL_Linea);
			this->panel1->Controls->Add(this->rB_tipoEVL_Puntual);
			this->panel1->Location = System::Drawing::Point(4, 279);
			this->panel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(116, 210);
			this->panel1->TabIndex = 42;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->rB_tipoEVL_DistMarginal);
			this->panel2->Controls->Add(this->rB_tipoEVL_marginal);
			this->panel2->Controls->Add(this->rB_tipoEVL_Distribuido);
			this->panel2->Location = System::Drawing::Point(2, 156);
			this->panel2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(113, 54);
			this->panel2->TabIndex = 50;
			// 
			// rB_tipoEVL_DistMarginal
			// 
			this->rB_tipoEVL_DistMarginal->AutoSize = true;
			this->rB_tipoEVL_DistMarginal->Checked = true;
			this->rB_tipoEVL_DistMarginal->Location = System::Drawing::Point(8, 35);
			this->rB_tipoEVL_DistMarginal->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_tipoEVL_DistMarginal->Name = L"rB_tipoEVL_DistMarginal";
			this->rB_tipoEVL_DistMarginal->Size = System::Drawing::Size(47, 17);
			this->rB_tipoEVL_DistMarginal->TabIndex = 4;
			this->rB_tipoEVL_DistMarginal->TabStop = true;
			this->rB_tipoEVL_DistMarginal->Text = L"Both";
			this->rB_tipoEVL_DistMarginal->UseVisualStyleBackColor = true;
			this->rB_tipoEVL_DistMarginal->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_tipoEVL_DistMarginal_CheckedChanged);
			// 
			// rB_tipoEVL_marginal
			// 
			this->rB_tipoEVL_marginal->AutoSize = true;
			this->rB_tipoEVL_marginal->Location = System::Drawing::Point(8, 17);
			this->rB_tipoEVL_marginal->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_tipoEVL_marginal->Name = L"rB_tipoEVL_marginal";
			this->rB_tipoEVL_marginal->Size = System::Drawing::Size(81, 17);
			this->rB_tipoEVL_marginal->TabIndex = 3;
			this->rB_tipoEVL_marginal->Text = L"Only Margin";
			this->rB_tipoEVL_marginal->UseVisualStyleBackColor = true;
			this->rB_tipoEVL_marginal->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_tipoEVL_marginal_CheckedChanged);
			// 
			// rB_tipoEVL_Distribuido
			// 
			this->rB_tipoEVL_Distribuido->AutoSize = true;
			this->rB_tipoEVL_Distribuido->Location = System::Drawing::Point(8, 2);
			this->rB_tipoEVL_Distribuido->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_tipoEVL_Distribuido->Name = L"rB_tipoEVL_Distribuido";
			this->rB_tipoEVL_Distribuido->Size = System::Drawing::Size(100, 17);
			this->rB_tipoEVL_Distribuido->TabIndex = 2;
			this->rB_tipoEVL_Distribuido->Text = L"Only DIstributed";
			this->rB_tipoEVL_Distribuido->UseVisualStyleBackColor = true;
			this->rB_tipoEVL_Distribuido->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_tipoEVL_Distribuido_CheckedChanged);
			// 
			// nUD_DataEVL_StepChangeDist
			// 
			this->nUD_DataEVL_StepChangeDist->Location = System::Drawing::Point(50, 135);
			this->nUD_DataEVL_StepChangeDist->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_DataEVL_StepChangeDist->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_DataEVL_StepChangeDist->Name = L"nUD_DataEVL_StepChangeDist";
			this->nUD_DataEVL_StepChangeDist->Size = System::Drawing::Size(55, 20);
			this->nUD_DataEVL_StepChangeDist->TabIndex = 46;
			this->nUD_DataEVL_StepChangeDist->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->nUD_DataEVL_StepChangeDist->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_DataEVL_StepChangeDist_ValueChanged);
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(18, 118);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(116, 13);
			this->label29->TabIndex = 47;
			this->label29->Text = L"Step Change Adhesion";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(76, 76);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(58, 13);
			this->label30->TabIndex = 49;
			this->label30->Text = L"Width Line";
			// 
			// nUD_DataEVL_AnchoLinea
			// 
			this->nUD_DataEVL_AnchoLinea->Location = System::Drawing::Point(15, 72);
			this->nUD_DataEVL_AnchoLinea->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_DataEVL_AnchoLinea->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_DataEVL_AnchoLinea->Name = L"nUD_DataEVL_AnchoLinea";
			this->nUD_DataEVL_AnchoLinea->Size = System::Drawing::Size(55, 20);
			this->nUD_DataEVL_AnchoLinea->TabIndex = 48;
			this->nUD_DataEVL_AnchoLinea->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {600, 0, 0, 0});
			this->nUD_DataEVL_AnchoLinea->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_DataEVL_AnchoLinea_ValueChanged);
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(76, 100);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(52, 13);
			this->label28->TabIndex = 45;
			this->label28->Text = L"Step Red";
			// 
			// nUD_DataEVL_StepRedLine
			// 
			this->nUD_DataEVL_StepRedLine->Location = System::Drawing::Point(14, 96);
			this->nUD_DataEVL_StepRedLine->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_DataEVL_StepRedLine->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_DataEVL_StepRedLine->Name = L"nUD_DataEVL_StepRedLine";
			this->nUD_DataEVL_StepRedLine->Size = System::Drawing::Size(55, 20);
			this->nUD_DataEVL_StepRedLine->TabIndex = 44;
			this->nUD_DataEVL_StepRedLine->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->nUD_DataEVL_StepRedLine->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_DataEVL_StepRedLine_ValueChanged);
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(75, 33);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(35, 13);
			this->label27->TabIndex = 43;
			this->label27->Text = L"Pos Y";
			// 
			// nUD_DataEVL_PosY
			// 
			this->nUD_DataEVL_PosY->Location = System::Drawing::Point(14, 29);
			this->nUD_DataEVL_PosY->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_DataEVL_PosY->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_DataEVL_PosY->Name = L"nUD_DataEVL_PosY";
			this->nUD_DataEVL_PosY->Size = System::Drawing::Size(55, 20);
			this->nUD_DataEVL_PosY->TabIndex = 43;
			this->nUD_DataEVL_PosY->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {500, 0, 0, 0});
			this->nUD_DataEVL_PosY->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_DataEVL_PosY_ValueChanged);
			// 
			// rB_tipoEVL_Linea
			// 
			this->rB_tipoEVL_Linea->AutoSize = true;
			this->rB_tipoEVL_Linea->Location = System::Drawing::Point(9, 53);
			this->rB_tipoEVL_Linea->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_tipoEVL_Linea->Name = L"rB_tipoEVL_Linea";
			this->rB_tipoEVL_Linea->Size = System::Drawing::Size(64, 17);
			this->rB_tipoEVL_Linea->TabIndex = 1;
			this->rB_tipoEVL_Linea->Text = L"EndLine";
			this->rB_tipoEVL_Linea->UseVisualStyleBackColor = true;
			this->rB_tipoEVL_Linea->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_tipoEVL_Linea_CheckedChanged);
			// 
			// rB_tipoEVL_Puntual
			// 
			this->rB_tipoEVL_Puntual->AutoSize = true;
			this->rB_tipoEVL_Puntual->Checked = true;
			this->rB_tipoEVL_Puntual->Location = System::Drawing::Point(8, 10);
			this->rB_tipoEVL_Puntual->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_tipoEVL_Puntual->Name = L"rB_tipoEVL_Puntual";
			this->rB_tipoEVL_Puntual->Size = System::Drawing::Size(79, 17);
			this->rB_tipoEVL_Puntual->TabIndex = 0;
			this->rB_tipoEVL_Puntual->TabStop = true;
			this->rB_tipoEVL_Puntual->Text = L"Puntual Int.";
			this->rB_tipoEVL_Puntual->UseVisualStyleBackColor = true;
			this->rB_tipoEVL_Puntual->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_tipoEVL_Puntual_CheckedChanged);
			// 
			// nUD_EVL_Factor
			// 
			this->nUD_EVL_Factor->DecimalPlaces = 3;
			this->nUD_EVL_Factor->Location = System::Drawing::Point(31, 251);
			this->nUD_EVL_Factor->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_EVL_Factor->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 262144});
			this->nUD_EVL_Factor->Name = L"nUD_EVL_Factor";
			this->nUD_EVL_Factor->Size = System::Drawing::Size(55, 20);
			this->nUD_EVL_Factor->TabIndex = 50;
			this->nUD_EVL_Factor->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_EVL_Factor->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_EVL_Factor_ValueChanged);
			// 
			// rB_EVL_Selected
			// 
			this->rB_EVL_Selected->AutoSize = true;
			this->rB_EVL_Selected->Location = System::Drawing::Point(22, 228);
			this->rB_EVL_Selected->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_EVL_Selected->Name = L"rB_EVL_Selected";
			this->rB_EVL_Selected->Size = System::Drawing::Size(86, 17);
			this->rB_EVL_Selected->TabIndex = 41;
			this->rB_EVL_Selected->Text = L"% Interaction";
			this->rB_EVL_Selected->UseVisualStyleBackColor = true;
			this->rB_EVL_Selected->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_EVL_Selected_CheckedChanged);
			// 
			// rB_EVL_Global
			// 
			this->rB_EVL_Global->AutoSize = true;
			this->rB_EVL_Global->Checked = true;
			this->rB_EVL_Global->Location = System::Drawing::Point(22, 206);
			this->rB_EVL_Global->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rB_EVL_Global->Name = L"rB_EVL_Global";
			this->rB_EVL_Global->Size = System::Drawing::Size(108, 17);
			this->rB_EVL_Global->TabIndex = 40;
			this->rB_EVL_Global->TabStop = true;
			this->rB_EVL_Global->Text = L"Global Interaction";
			this->rB_EVL_Global->UseVisualStyleBackColor = true;
			this->rB_EVL_Global->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::rB_EVL_Global_CheckedChanged);
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(13, 71);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(149, 13);
			this->label26->TabIndex = 39;
			this->label26->Text = L"Ratio DFCs DEBs Interactions";
			// 
			// nUD_Ratio_DFC_DEB
			// 
			this->nUD_Ratio_DFC_DEB->DecimalPlaces = 2;
			this->nUD_Ratio_DFC_DEB->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->nUD_Ratio_DFC_DEB->Location = System::Drawing::Point(22, 88);
			this->nUD_Ratio_DFC_DEB->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_Ratio_DFC_DEB->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 196608});
			this->nUD_Ratio_DFC_DEB->Name = L"nUD_Ratio_DFC_DEB";
			this->nUD_Ratio_DFC_DEB->Size = System::Drawing::Size(40, 20);
			this->nUD_Ratio_DFC_DEB->TabIndex = 38;
			this->nUD_Ratio_DFC_DEB->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_Ratio_DFC_DEB->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_Ratio_DFC_DEB_ValueChanged);
			// 
			// cB_EVLInteraction
			// 
			this->cB_EVLInteraction->AutoSize = true;
			this->cB_EVLInteraction->Location = System::Drawing::Point(15, 119);
			this->cB_EVLInteraction->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_EVLInteraction->Name = L"cB_EVLInteraction";
			this->cB_EVLInteraction->Size = System::Drawing::Size(98, 17);
			this->cB_EVLInteraction->TabIndex = 1;
			this->cB_EVLInteraction->Text = L"EVL interaction";
			this->cB_EVLInteraction->UseVisualStyleBackColor = true;
			this->cB_EVLInteraction->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_EVLInteraction_CheckedChanged);
			// 
			// cB_DEBAttraction
			// 
			this->cB_DEBAttraction->AutoSize = true;
			this->cB_DEBAttraction->Location = System::Drawing::Point(14, 12);
			this->cB_DEBAttraction->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_DEBAttraction->Name = L"cB_DEBAttraction";
			this->cB_DEBAttraction->Size = System::Drawing::Size(96, 17);
			this->cB_DEBAttraction->TabIndex = 0;
			this->cB_DEBAttraction->Text = L"DEB Attraction";
			this->cB_DEBAttraction->UseVisualStyleBackColor = true;
			this->cB_DEBAttraction->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_DEBAttraction_CheckedChanged);
			// 
			// tabPage8
			// 
			this->tabPage8->Controls->Add(this->label34);
			this->tabPage8->Controls->Add(this->nUP_ProliferacionSteps);
			this->tabPage8->Controls->Add(this->label23);
			this->tabPage8->Controls->Add(this->label25);
			this->tabPage8->Controls->Add(this->cB_DFCInteraction);
			this->tabPage8->Controls->Add(this->nUD_RadInteraction);
			this->tabPage8->Controls->Add(this->nUD_DFC_Radius);
			this->tabPage8->Controls->Add(this->label24);
			this->tabPage8->Controls->Add(this->nUD_StepsForChange);
			this->tabPage8->Controls->Add(this->cB_TryFreeStagnant);
			this->tabPage8->Controls->Add(this->label20);
			this->tabPage8->Controls->Add(this->label21);
			this->tabPage8->Controls->Add(this->cB_RandPerturbation);
			this->tabPage8->Controls->Add(this->nUD_NumDFCs);
			this->tabPage8->Location = System::Drawing::Point(4, 22);
			this->tabPage8->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage8->Name = L"tabPage8";
			this->tabPage8->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tabPage8->Size = System::Drawing::Size(166, 492);
			this->tabPage8->TabIndex = 1;
			this->tabPage8->Text = L"DFC_Agents";
			this->tabPage8->UseVisualStyleBackColor = true;
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(16, 214);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(92, 13);
			this->label34->TabIndex = 39;
			this->label34->Text = L"Steps Proliferation";
			// 
			// nUP_ProliferacionSteps
			// 
			this->nUP_ProliferacionSteps->Location = System::Drawing::Point(71, 231);
			this->nUP_ProliferacionSteps->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUP_ProliferacionSteps->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUP_ProliferacionSteps->Name = L"nUP_ProliferacionSteps";
			this->nUP_ProliferacionSteps->Size = System::Drawing::Size(42, 20);
			this->nUP_ProliferacionSteps->TabIndex = 38;
			this->nUP_ProliferacionSteps->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUP_ProliferacionSteps->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUP_ProliferacionSteps_ValueChanged);
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(69, 179);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(93, 13);
			this->label23->TabIndex = 19;
			this->label23->Text = L"Radius Interaction";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(52, 61);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(60, 13);
			this->label25->TabIndex = 37;
			this->label25->Text = L"Cell Radius";
			// 
			// cB_DFCInteraction
			// 
			this->cB_DFCInteraction->AutoSize = true;
			this->cB_DFCInteraction->Location = System::Drawing::Point(7, 154);
			this->cB_DFCInteraction->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_DFCInteraction->Name = L"cB_DFCInteraction";
			this->cB_DFCInteraction->Size = System::Drawing::Size(99, 17);
			this->cB_DFCInteraction->TabIndex = 2;
			this->cB_DFCInteraction->Text = L"DFC interaction";
			this->cB_DFCInteraction->UseVisualStyleBackColor = true;
			this->cB_DFCInteraction->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_DFCInteraction_CheckedChanged);
			// 
			// nUD_RadInteraction
			// 
			this->nUD_RadInteraction->DecimalPlaces = 3;
			this->nUD_RadInteraction->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->nUD_RadInteraction->Location = System::Drawing::Point(23, 177);
			this->nUD_RadInteraction->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_RadInteraction->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 196608});
			this->nUD_RadInteraction->Name = L"nUD_RadInteraction";
			this->nUD_RadInteraction->Size = System::Drawing::Size(40, 20);
			this->nUD_RadInteraction->TabIndex = 19;
			this->nUD_RadInteraction->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 65536});
			this->nUD_RadInteraction->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_RadInteraction_ValueChanged);
			// 
			// nUD_DFC_Radius
			// 
			this->nUD_DFC_Radius->Location = System::Drawing::Point(16, 61);
			this->nUD_DFC_Radius->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->nUD_DFC_Radius->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_DFC_Radius->Name = L"nUD_DFC_Radius";
			this->nUD_DFC_Radius->Size = System::Drawing::Size(32, 20);
			this->nUD_DFC_Radius->TabIndex = 36;
			this->nUD_DFC_Radius->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_DFC_Radius->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_DFC_Radius_ValueChanged);
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(74, 110);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(34, 13);
			this->label24->TabIndex = 35;
			this->label24->Text = L"Steps";
			// 
			// nUD_StepsForChange
			// 
			this->nUD_StepsForChange->Location = System::Drawing::Point(15, 109);
			this->nUD_StepsForChange->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->nUD_StepsForChange->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_StepsForChange->Name = L"nUD_StepsForChange";
			this->nUD_StepsForChange->Size = System::Drawing::Size(42, 20);
			this->nUD_StepsForChange->TabIndex = 33;
			this->nUD_StepsForChange->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->nUD_StepsForChange->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_StepsForChange_ValueChanged);
			// 
			// cB_TryFreeStagnant
			// 
			this->cB_TryFreeStagnant->AutoSize = true;
			this->cB_TryFreeStagnant->Location = System::Drawing::Point(7, 132);
			this->cB_TryFreeStagnant->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_TryFreeStagnant->Name = L"cB_TryFreeStagnant";
			this->cB_TryFreeStagnant->Size = System::Drawing::Size(111, 17);
			this->cB_TryFreeStagnant->TabIndex = 32;
			this->cB_TryFreeStagnant->Text = L"Try Free Stagnant";
			this->cB_TryFreeStagnant->UseVisualStyleBackColor = true;
			this->cB_TryFreeStagnant->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_TryFreeStagnant_CheckedChanged);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(52, 33);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(86, 13);
			this->label20->TabIndex = 29;
			this->label20->Text = L"# DFC Cells (Init)";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(13, 2);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(91, 13);
			this->label21->TabIndex = 30;
			this->label21->Text = L"Agent Parameters";
			// 
			// cB_RandPerturbation
			// 
			this->cB_RandPerturbation->AutoSize = true;
			this->cB_RandPerturbation->Location = System::Drawing::Point(7, 87);
			this->cB_RandPerturbation->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cB_RandPerturbation->Name = L"cB_RandPerturbation";
			this->cB_RandPerturbation->Size = System::Drawing::Size(112, 17);
			this->cB_RandPerturbation->TabIndex = 31;
			this->cB_RandPerturbation->Text = L"Rand Perturbation";
			this->cB_RandPerturbation->UseVisualStyleBackColor = true;
			this->cB_RandPerturbation->CheckedChanged += gcnew System::EventHandler(this, &SistemaControl::cB_RandPerturbation_CheckedChanged);
			// 
			// nUD_NumDFCs
			// 
			this->nUD_NumDFCs->Location = System::Drawing::Point(15, 33);
			this->nUD_NumDFCs->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_NumDFCs->Name = L"nUD_NumDFCs";
			this->nUD_NumDFCs->Size = System::Drawing::Size(32, 20);
			this->nUD_NumDFCs->TabIndex = 28;
			this->nUD_NumDFCs->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 0, 0});
			this->nUD_NumDFCs->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::nUD_NumDFCs_ValueChanged);
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(6, 138);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(100, 13);
			this->label22->TabIndex = 17;
			this->label22->Text = L"Tiempo Entre Steps";
			// 
			// numericDeadTime
			// 
			this->numericDeadTime->DecimalPlaces = 4;
			this->numericDeadTime->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericDeadTime->Location = System::Drawing::Point(23, 157);
			this->numericDeadTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->numericDeadTime->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 196608});
			this->numericDeadTime->Name = L"numericDeadTime";
			this->numericDeadTime->Size = System::Drawing::Size(75, 20);
			this->numericDeadTime->TabIndex = 16;
			this->numericDeadTime->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericDeadTime->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericDeadTime_ValueChanged);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(6, 99);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(86, 13);
			this->label19->TabIndex = 11;
			this->label19->Text = L"Delta Simulacion";
			// 
			// numericDelta
			// 
			this->numericDelta->DecimalPlaces = 4;
			this->numericDelta->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numericDelta->Location = System::Drawing::Point(23, 116);
			this->numericDelta->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericDelta->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 262144});
			this->numericDelta->Name = L"numericDelta";
			this->numericDelta->Size = System::Drawing::Size(75, 20);
			this->numericDelta->TabIndex = 10;
			this->numericDelta->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->numericDelta->ValueChanged += gcnew System::EventHandler(this, &SistemaControl::numericDelta_ValueChanged);
			// 
			// bStopSim
			// 
			this->bStopSim->Location = System::Drawing::Point(23, 63);
			this->bStopSim->Name = L"bStopSim";
			this->bStopSim->Size = System::Drawing::Size(75, 23);
			this->bStopSim->TabIndex = 9;
			this->bStopSim->Text = L"Stop";
			this->bStopSim->UseVisualStyleBackColor = true;
			this->bStopSim->Click += gcnew System::EventHandler(this, &SistemaControl::bStopSim_Click);
			// 
			// bStartSim
			// 
			this->bStartSim->Location = System::Drawing::Point(23, 22);
			this->bStartSim->Name = L"bStartSim";
			this->bStartSim->Size = System::Drawing::Size(75, 23);
			this->bStartSim->TabIndex = 8;
			this->bStartSim->Text = L"Start";
			this->bStartSim->UseVisualStyleBackColor = true;
			this->bStartSim->Click += gcnew System::EventHandler(this, &SistemaControl::bStartSim_Click);
			// 
			// tabPage6
			// 
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Padding = System::Windows::Forms::Padding(3, 3, 3, 3);
			this->tabPage6->Size = System::Drawing::Size(128, 734);
			this->tabPage6->TabIndex = 1;
			this->tabPage6->Text = L"Opt";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// tCMoreOptions
			// 
			this->tCMoreOptions->Controls->Add(this->tabPage7);
			this->tCMoreOptions->Controls->Add(this->tabTIMETASKS);
			this->tCMoreOptions->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tCMoreOptions->Location = System::Drawing::Point(128, 3);
			this->tCMoreOptions->Name = L"tCMoreOptions";
			this->tCMoreOptions->SelectedIndex = 0;
			this->tCMoreOptions->Size = System::Drawing::Size(994, 33);
			this->tCMoreOptions->TabIndex = 11;
			// 
			// tabPage7
			// 
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Padding = System::Windows::Forms::Padding(3, 3, 3, 3);
			this->tabPage7->Size = System::Drawing::Size(986, 7);
			this->tabPage7->TabIndex = 0;
			this->tabPage7->Text = L"Opciones";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// tabTIMETASKS
			// 
			this->tabTIMETASKS->Controls->Add(this->tBTimeCalc);
			this->tabTIMETASKS->Controls->Add(this->label16);
			this->tabTIMETASKS->Controls->Add(this->tBTimeRenderSim);
			this->tabTIMETASKS->Controls->Add(this->label13);
			this->tabTIMETASKS->Controls->Add(this->tBTimeRenderT);
			this->tabTIMETASKS->Controls->Add(this->label12);
			this->tabTIMETASKS->Location = System::Drawing::Point(4, 22);
			this->tabTIMETASKS->Name = L"tabTIMETASKS";
			this->tabTIMETASKS->Padding = System::Windows::Forms::Padding(3, 3, 3, 3);
			this->tabTIMETASKS->Size = System::Drawing::Size(1127, 8);
			this->tabTIMETASKS->TabIndex = 1;
			this->tabTIMETASKS->Text = L"TIME";
			this->tabTIMETASKS->UseVisualStyleBackColor = true;
			// 
			// tBTimeCalc
			// 
			this->tBTimeCalc->Enabled = false;
			this->tBTimeCalc->Location = System::Drawing::Point(260, 14);
			this->tBTimeCalc->Name = L"tBTimeCalc";
			this->tBTimeCalc->Size = System::Drawing::Size(100, 20);
			this->tBTimeCalc->TabIndex = 9;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(248, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(48, 13);
			this->label16->TabIndex = 8;
			this->label16->Text = L"Calc Sim";
			// 
			// tBTimeRenderSim
			// 
			this->tBTimeRenderSim->Enabled = false;
			this->tBTimeRenderSim->Location = System::Drawing::Point(134, 14);
			this->tBTimeRenderSim->Name = L"tBTimeRenderSim";
			this->tBTimeRenderSim->Size = System::Drawing::Size(100, 20);
			this->tBTimeRenderSim->TabIndex = 3;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(122, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(62, 13);
			this->label13->TabIndex = 2;
			this->label13->Text = L"Render Sim";
			// 
			// tBTimeRenderT
			// 
			this->tBTimeRenderT->Enabled = false;
			this->tBTimeRenderT->Location = System::Drawing::Point(19, 14);
			this->tBTimeRenderT->Name = L"tBTimeRenderT";
			this->tBTimeRenderT->Size = System::Drawing::Size(100, 20);
			this->tBTimeRenderT->TabIndex = 1;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(7, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(69, 13);
			this->label12->TabIndex = 0;
			this->label12->Text = L"Render Total";
			// 
			// bwControl
			// 
			this->bwControl->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &SistemaControl::bwControl_DoWork);
			// 
			// bwDraw
			// 
			this->bwDraw->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &SistemaControl::bwDraw_DoWork);
			// 
			// bwDrawMask
			// 
			this->bwDrawMask->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &SistemaControl::bwDrawMask_DoWork);
			// 
			// SistemaControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1251, 825);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->MenuPrincipal);
			this->MinimumSize = System::Drawing::Size(800, 599);
			this->Name = L"SistemaControl";
			this->ShowInTaskbar = false;
			this->Text = L"Sistema de Control";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SistemaControl::SistemaControl_FormClosed);
			this->Load += gcnew System::EventHandler(this, &SistemaControl::SistemaControl_Load);
			this->MenuPrincipal->ResumeLayout(false);
			this->MenuPrincipal->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tabControlGraph->ResumeLayout(false);
			this->tabPage0->ResumeLayout(false);
			this->tableLayoutPanel2->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->groupModTrampas->ResumeLayout(false);
			this->groupModTrampas->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericZTrampa))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericYTrampa))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericXTrampa))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericIndiceTrampa))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRadioObjetos))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericNumObjetos))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericYCampo))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericXCampo))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectAlto))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectAncho))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericRectX))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->tabControl3->ResumeLayout(false);
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsEVL_Mov))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsMovDB_margin))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_StepChangeDist))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_AnchoLinea))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_StepRedLine))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DataEVL_PosY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_EVL_Factor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_Ratio_DFC_DEB))->EndInit();
			this->tabPage8->ResumeLayout(false);
			this->tabPage8->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUP_ProliferacionSteps))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_RadInteraction))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_DFC_Radius))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_StepsForChange))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_NumDFCs))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDeadTime))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDelta))->EndInit();
			this->tCMoreOptions->ResumeLayout(false);
			this->tabTIMETASKS->ResumeLayout(false);
			this->tabTIMETASKS->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void SistemaControl_Load(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SistemaControl_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
		private: System::Void panelTab0Izq_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void bwControl_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
		private: System::Void panelTab0Izq_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private: System::Void panelTab0Izq_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private: System::Void panelTab0Izq_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private: System::Void bStartSim_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void bStopSim_Click(System::Object^  sender, System::EventArgs^  e);

		private: System::Void numericIndiceTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericXTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericYTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericZTrampa_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void bDelete_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void bCrear_Click(System::Object^  sender, System::EventArgs^  e){_CrearMover = true;};
		private: System::Void bwDraw_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
		private: System::Void bwDrawMask_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
		private: System::Void bCrearObjetos_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void bModRect_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void bModCampo_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void tabControl2_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e);
		private: System::Void numericNumObjetos_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericRadioObjetos_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericRectAncho_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericRectAlto_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericRectX_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericRectY_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericXCampo_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericYCampo_ValueChanged(System::Object^  sender, System::EventArgs^  e);

		private: System::Void numericDelta_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void numericDeadTime_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_NumDFCs_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_RandPerturbation_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_TryFreeStagnant_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_DEBAttraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_StepsForChange_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_EVLInteraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_DFCInteraction_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_RadInteraction_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_DFC_Radius_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_Ratio_DFC_DEB_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_EVL_Global_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_EVL_Selected_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_tipoEVL_Puntual_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_tipoEVL_Linea_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_DataEVL_PosY_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_DataEVL_StepRedLine_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_DataEVL_StepChangeDist_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_DataEVL_AnchoLinea_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_EVL_Factor_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void cB_AdhesionEVL_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_StepsMovDB_margin_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUD_StepsEVL_Mov_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_tipoEVL_Distribuido_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_tipoEVL_marginal_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void rB_tipoEVL_DistMarginal_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void nUP_ProliferacionSteps_ValueChanged(System::Object^  sender, System::EventArgs^  e);
};
}
