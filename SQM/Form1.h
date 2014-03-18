#pragma once

#include "OpenGL.h"
#include <iostream>
#include <string>

#define TIMER_INTERVAL 20

using namespace std;
using namespace System;

void MarshalString ( String ^ s, string& os ) {//from MSDN converting String to std::string
	using namespace Runtime::InteropServices;
	const char* chars = 
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

namespace SQM {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void) : OpenGL(nullptr)
		{
			InitializeComponent();
			//
			//Add the constructor code here
			//
			OpenGL = gcnew COpenGL(this->panel1, 562, 464);
			OpenGL->newFile();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
#pragma region Custom Variables
		OpenGLForm::COpenGL ^ OpenGL;
		Point lastPosition;
#pragma endregion
	private: System::Windows::Forms::ToolStrip^  toolStrip1;


	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;






	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  sQMToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  straightenToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  computeConvexHullToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  subdivideConvexHullToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  joinBranchNodePolyhedronsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: System::Windows::Forms::ToolStripMenuItem^  straightenMeshToolStripMenuItem;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::ToolStripMenuItem^  exportMeshToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newToolStripMenuItem;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown5;
	private: System::Windows::Forms::Label^  label7;


	private: System::Windows::Forms::ToolStripMenuItem^  showToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  wireframeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  quaternionSmoothingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  quaternionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  valencyWeightedLaplacianToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  oneRingAreaWeightedLaplacianToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  noSmoothingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  avaragingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editSkeletonToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  normalsToolStripMenuItem;
	private: System::Windows::Forms::Label^  label9;

	private: System::Windows::Forms::NumericUpDown^  numericUpDown6;
	private: System::Windows::Forms::ToolStripMenuItem^  shadersToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  reloadShadersToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown7;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown10;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown9;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown8;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown13;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown12;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  skinningToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cPUSkinningNoTesselationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gPUVertexSkinningnoTesselationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gPUTessSkinningLastOnlyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gPUTessSkinningAllAvarageToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
	private: System::Windows::Forms::ToolStripMenuItem^  animationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  setBindPoseToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addReferencePoseToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  startToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  stopToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAnimationToolStripMenuItem;







			 System::Windows::Forms::Timer^  timer1;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
				 this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				 this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
				 this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
				 this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
				 this->panel1 = (gcnew System::Windows::Forms::Panel());
				 this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				 this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->saveToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->exportMeshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->sQMToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->editSkeletonToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->straightenToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->computeConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->subdivideConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->quaternionSmoothingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->noSmoothingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->avaragingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->quaternionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->valencyWeightedLaplacianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->joinBranchNodePolyhedronsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->straightenMeshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->showToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->wireframeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->normalsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->shadersToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->reloadShadersToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->skinningToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->cPUSkinningNoTesselationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->gPUTessSkinningLastOnlyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->gPUTessSkinningAllAvarageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->animationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->startToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->setBindPoseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->addReferencePoseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->saveAnimationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->panel2 = (gcnew System::Windows::Forms::Panel());
				 this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
				 this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
				 this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
				 this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
				 this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
				 this->numericUpDown10 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->numericUpDown9 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->numericUpDown8 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label11 = (gcnew System::Windows::Forms::Label());
				 this->label10 = (gcnew System::Windows::Forms::Label());
				 this->label8 = (gcnew System::Windows::Forms::Label());
				 this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
				 this->label14 = (gcnew System::Windows::Forms::Label());
				 this->label13 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown13 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->numericUpDown12 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->numericUpDown11 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label12 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				 this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->label3 = (gcnew System::Windows::Forms::Label());
				 this->label7 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label6 = (gcnew System::Windows::Forms::Label());
				 this->label4 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label5 = (gcnew System::Windows::Forms::Label());
				 this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
				 this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
				 this->numericUpDown7 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown6 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label9 = (gcnew System::Windows::Forms::Label());
				 this->toolStrip1->SuspendLayout();
				 this->menuStrip1->SuspendLayout();
				 this->panel2->SuspendLayout();
				 this->tabControl1->SuspendLayout();
				 this->tabPage1->SuspendLayout();
				 this->tabControl2->SuspendLayout();
				 this->tabPage3->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown10))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown9))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown8))->BeginInit();
				 this->tabPage4->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown13))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown12))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown11))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
				 this->tabPage2->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown7))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown6))->BeginInit();
				 this->SuspendLayout();
				 // 
				 // timer1
				 // 
				 this->timer1->Enabled = true;
				 this->timer1->Interval = 20;
				 this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
				 // 
				 // toolStrip1
				 // 
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->toolStripButton1, 
					 this->toolStripButton3, this->toolStripButton2});
				 this->toolStrip1->Location = System::Drawing::Point(0, 24);
				 this->toolStrip1->Name = L"toolStrip1";
				 this->toolStrip1->Size = System::Drawing::Size(744, 25);
				 this->toolStrip1->TabIndex = 0;
				 this->toolStrip1->Text = L"toolStrip1";
				 // 
				 // toolStripButton1
				 // 
				 this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
				 this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripButton1->Name = L"toolStripButton1";
				 this->toolStripButton1->Size = System::Drawing::Size(23, 22);
				 this->toolStripButton1->Text = L"toolStripButton1";
				 this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
				 // 
				 // toolStripButton3
				 // 
				 this->toolStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton3.Image")));
				 this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripButton3->Name = L"toolStripButton3";
				 this->toolStripButton3->Size = System::Drawing::Size(23, 22);
				 this->toolStripButton3->Text = L"toolStripButton3";
				 this->toolStripButton3->Click += gcnew System::EventHandler(this, &Form1::toolStripButton3_Click);
				 // 
				 // toolStripButton2
				 // 
				 this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
				 this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripButton2->Name = L"toolStripButton2";
				 this->toolStripButton2->Size = System::Drawing::Size(23, 22);
				 this->toolStripButton2->Text = L"toolStripButton2";
				 this->toolStripButton2->Click += gcnew System::EventHandler(this, &Form1::toolStripButton2_Click);
				 // 
				 // openFileDialog1
				 // 
				 this->openFileDialog1->FileName = L"openFileDialog1";
				 // 
				 // panel1
				 // 
				 this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
					 | System::Windows::Forms::AnchorStyles::Left) 
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->panel1->Location = System::Drawing::Point(182, 49);
				 this->panel1->Name = L"panel1";
				 this->panel1->Size = System::Drawing::Size(562, 464);
				 this->panel1->TabIndex = 1;
				 this->panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Panel1_MouseDown);
				 this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Panel1_MouseMove);
				 this->panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Panel1_MouseUp);
				 this->panel1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &Form1::panel1_PreviewKeyDown);
				 this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
				 // 
				 // menuStrip1
				 // 
				 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->fileToolStripMenuItem, 
					 this->sQMToolStripMenuItem, this->showToolStripMenuItem, this->shadersToolStripMenuItem, this->settingsToolStripMenuItem, this->animationToolStripMenuItem});
				 this->menuStrip1->Location = System::Drawing::Point(0, 0);
				 this->menuStrip1->Name = L"menuStrip1";
				 this->menuStrip1->Size = System::Drawing::Size(744, 24);
				 this->menuStrip1->TabIndex = 2;
				 this->menuStrip1->Text = L"menuStrip1";
				 // 
				 // fileToolStripMenuItem
				 // 
				 this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->newToolStripMenuItem, 
					 this->loadToolStripMenuItem, this->saveToolStripMenuItem1, this->exportMeshToolStripMenuItem, this->toolStripSeparator1, this->exitToolStripMenuItem});
				 this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				 this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
				 this->fileToolStripMenuItem->Text = L"&File";
				 // 
				 // newToolStripMenuItem
				 // 
				 this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
				 this->newToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
				 this->newToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->newToolStripMenuItem->Text = L"&New";
				 this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::newToolStripMenuItem_Click);
				 // 
				 // loadToolStripMenuItem
				 // 
				 this->loadToolStripMenuItem->Name = L"loadToolStripMenuItem";
				 this->loadToolStripMenuItem->ShortcutKeyDisplayString = L"";
				 this->loadToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
				 this->loadToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->loadToolStripMenuItem->Text = L"&Open";
				 this->loadToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::loadToolStripMenuItem_Click);
				 // 
				 // saveToolStripMenuItem1
				 // 
				 this->saveToolStripMenuItem1->Name = L"saveToolStripMenuItem1";
				 this->saveToolStripMenuItem1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
				 this->saveToolStripMenuItem1->Size = System::Drawing::Size(179, 22);
				 this->saveToolStripMenuItem1->Text = L"&Save";
				 this->saveToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem1_Click);
				 // 
				 // exportMeshToolStripMenuItem
				 // 
				 this->exportMeshToolStripMenuItem->Name = L"exportMeshToolStripMenuItem";
				 this->exportMeshToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
				 this->exportMeshToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->exportMeshToolStripMenuItem->Text = L"&Export mesh";
				 this->exportMeshToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exportMeshToolStripMenuItem_Click);
				 // 
				 // toolStripSeparator1
				 // 
				 this->toolStripSeparator1->Name = L"toolStripSeparator1";
				 this->toolStripSeparator1->Size = System::Drawing::Size(176, 6);
				 // 
				 // exitToolStripMenuItem
				 // 
				 this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
				 this->exitToolStripMenuItem->ShortcutKeyDisplayString = L"Alt + F4";
				 this->exitToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->exitToolStripMenuItem->Text = L"E&xit";
				 this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
				 // 
				 // sQMToolStripMenuItem
				 // 
				 this->sQMToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->editSkeletonToolStripMenuItem, 
					 this->straightenToolStripMenuItem1, this->computeConvexHullToolStripMenuItem1, this->subdivideConvexHullToolStripMenuItem1, this->joinBranchNodePolyhedronsToolStripMenuItem, 
					 this->straightenMeshToolStripMenuItem});
				 this->sQMToolStripMenuItem->Name = L"sQMToolStripMenuItem";
				 this->sQMToolStripMenuItem->Size = System::Drawing::Size(45, 20);
				 this->sQMToolStripMenuItem->Text = L"&SQM";
				 // 
				 // editSkeletonToolStripMenuItem
				 // 
				 this->editSkeletonToolStripMenuItem->Checked = true;
				 this->editSkeletonToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->editSkeletonToolStripMenuItem->Name = L"editSkeletonToolStripMenuItem";
				 this->editSkeletonToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->editSkeletonToolStripMenuItem->Text = L"Edit Skeleton";
				 this->editSkeletonToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::editSkeletonToolStripMenuItem_Click);
				 // 
				 // straightenToolStripMenuItem1
				 // 
				 this->straightenToolStripMenuItem1->Name = L"straightenToolStripMenuItem1";
				 this->straightenToolStripMenuItem1->Size = System::Drawing::Size(236, 22);
				 this->straightenToolStripMenuItem1->Text = L"Straighten";
				 this->straightenToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::straightenToolStripMenuItem1_Click);
				 // 
				 // computeConvexHullToolStripMenuItem1
				 // 
				 this->computeConvexHullToolStripMenuItem1->Name = L"computeConvexHullToolStripMenuItem1";
				 this->computeConvexHullToolStripMenuItem1->Size = System::Drawing::Size(236, 22);
				 this->computeConvexHullToolStripMenuItem1->Text = L"Compute Convex Hull";
				 this->computeConvexHullToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::computeConvexHullToolStripMenuItem1_Click);
				 // 
				 // subdivideConvexHullToolStripMenuItem1
				 // 
				 this->subdivideConvexHullToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->quaternionSmoothingToolStripMenuItem, 
					 this->toolStripSeparator2, this->noSmoothingToolStripMenuItem, this->avaragingToolStripMenuItem, this->quaternionToolStripMenuItem, 
					 this->valencyWeightedLaplacianToolStripMenuItem, this->oneRingAreaWeightedLaplacianToolStripMenuItem});
				 this->subdivideConvexHullToolStripMenuItem1->Name = L"subdivideConvexHullToolStripMenuItem1";
				 this->subdivideConvexHullToolStripMenuItem1->Size = System::Drawing::Size(236, 22);
				 this->subdivideConvexHullToolStripMenuItem1->Text = L"Subdivide Convex Hull";
				 this->subdivideConvexHullToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::subdivideConvexHullToolStripMenuItem1_Click);
				 // 
				 // quaternionSmoothingToolStripMenuItem
				 // 
				 this->quaternionSmoothingToolStripMenuItem->Name = L"quaternionSmoothingToolStripMenuItem";
				 this->quaternionSmoothingToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->quaternionSmoothingToolStripMenuItem->Text = L"Smoothing";
				 // 
				 // toolStripSeparator2
				 // 
				 this->toolStripSeparator2->Name = L"toolStripSeparator2";
				 this->toolStripSeparator2->Size = System::Drawing::Size(254, 6);
				 // 
				 // noSmoothingToolStripMenuItem
				 // 
				 this->noSmoothingToolStripMenuItem->Name = L"noSmoothingToolStripMenuItem";
				 this->noSmoothingToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->noSmoothingToolStripMenuItem->Text = L"No Smoothing";
				 this->noSmoothingToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::noSmoothingToolStripMenuItem_Click);
				 // 
				 // avaragingToolStripMenuItem
				 // 
				 this->avaragingToolStripMenuItem->Name = L"avaragingToolStripMenuItem";
				 this->avaragingToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->avaragingToolStripMenuItem->Text = L"Avaraging";
				 this->avaragingToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::avaragingToolStripMenuItem_Click);
				 // 
				 // quaternionToolStripMenuItem
				 // 
				 this->quaternionToolStripMenuItem->Checked = true;
				 this->quaternionToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->quaternionToolStripMenuItem->Name = L"quaternionToolStripMenuItem";
				 this->quaternionToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->quaternionToolStripMenuItem->Text = L"Quaternion";
				 this->quaternionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::quaternionToolStripMenuItem_Click);
				 // 
				 // valencyWeightedLaplacianToolStripMenuItem
				 // 
				 this->valencyWeightedLaplacianToolStripMenuItem->Name = L"valencyWeightedLaplacianToolStripMenuItem";
				 this->valencyWeightedLaplacianToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->valencyWeightedLaplacianToolStripMenuItem->Text = L"Valency Weighted Laplacian";
				 this->valencyWeightedLaplacianToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::valencyWeightedLaplacianToolStripMenuItem_Click);
				 // 
				 // oneRingAreaWeightedLaplacianToolStripMenuItem
				 // 
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem->Name = L"oneRingAreaWeightedLaplacianToolStripMenuItem";
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem->Size = System::Drawing::Size(257, 22);
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem->Text = L"One Ring Area Weighted Laplacian";
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::oneRingAreaWeightedLaplacianToolStripMenuItem_Click);
				 // 
				 // joinBranchNodePolyhedronsToolStripMenuItem
				 // 
				 this->joinBranchNodePolyhedronsToolStripMenuItem->Name = L"joinBranchNodePolyhedronsToolStripMenuItem";
				 this->joinBranchNodePolyhedronsToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->joinBranchNodePolyhedronsToolStripMenuItem->Text = L"Join Branch Node Polyhedrons";
				 this->joinBranchNodePolyhedronsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::joinBranchNodePolyhedronsToolStripMenuItem_Click);
				 // 
				 // straightenMeshToolStripMenuItem
				 // 
				 this->straightenMeshToolStripMenuItem->Name = L"straightenMeshToolStripMenuItem";
				 this->straightenMeshToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->straightenMeshToolStripMenuItem->Text = L"Final Vertex Placement";
				 this->straightenMeshToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::straightenMeshToolStripMenuItem_Click);
				 // 
				 // showToolStripMenuItem
				 // 
				 this->showToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->wireframeToolStripMenuItem, 
					 this->normalsToolStripMenuItem});
				 this->showToolStripMenuItem->Name = L"showToolStripMenuItem";
				 this->showToolStripMenuItem->Size = System::Drawing::Size(48, 20);
				 this->showToolStripMenuItem->Text = L"Show";
				 // 
				 // wireframeToolStripMenuItem
				 // 
				 this->wireframeToolStripMenuItem->Checked = true;
				 this->wireframeToolStripMenuItem->CheckOnClick = true;
				 this->wireframeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->wireframeToolStripMenuItem->Name = L"wireframeToolStripMenuItem";
				 this->wireframeToolStripMenuItem->Size = System::Drawing::Size(129, 22);
				 this->wireframeToolStripMenuItem->Text = L"Wireframe";
				 this->wireframeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::wireframeToolStripMenuItem_Click);
				 // 
				 // normalsToolStripMenuItem
				 // 
				 this->normalsToolStripMenuItem->CheckOnClick = true;
				 this->normalsToolStripMenuItem->Name = L"normalsToolStripMenuItem";
				 this->normalsToolStripMenuItem->Size = System::Drawing::Size(129, 22);
				 this->normalsToolStripMenuItem->Text = L"Normals";
				 this->normalsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::normalsToolStripMenuItem_Click);
				 // 
				 // shadersToolStripMenuItem
				 // 
				 this->shadersToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->reloadShadersToolStripMenuItem});
				 this->shadersToolStripMenuItem->Name = L"shadersToolStripMenuItem";
				 this->shadersToolStripMenuItem->Size = System::Drawing::Size(60, 20);
				 this->shadersToolStripMenuItem->Text = L"Shaders";
				 // 
				 // reloadShadersToolStripMenuItem
				 // 
				 this->reloadShadersToolStripMenuItem->Name = L"reloadShadersToolStripMenuItem";
				 this->reloadShadersToolStripMenuItem->Size = System::Drawing::Size(154, 22);
				 this->reloadShadersToolStripMenuItem->Text = L"Reload Shaders";
				 this->reloadShadersToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::reloadShadersToolStripMenuItem_Click);
				 // 
				 // settingsToolStripMenuItem
				 // 
				 this->settingsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->skinningToolStripMenuItem});
				 this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
				 this->settingsToolStripMenuItem->Size = System::Drawing::Size(61, 20);
				 this->settingsToolStripMenuItem->Text = L"S&ettings";
				 // 
				 // skinningToolStripMenuItem
				 // 
				 this->skinningToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->cPUSkinningNoTesselationToolStripMenuItem, 
					 this->gPUVertexSkinningnoTesselationToolStripMenuItem, this->gPUTessSkinningLastOnlyToolStripMenuItem, this->gPUTessSkinningAllAvarageToolStripMenuItem});
				 this->skinningToolStripMenuItem->Name = L"skinningToolStripMenuItem";
				 this->skinningToolStripMenuItem->Size = System::Drawing::Size(120, 22);
				 this->skinningToolStripMenuItem->Text = L"Skinning";
				 // 
				 // cPUSkinningNoTesselationToolStripMenuItem
				 // 
				 this->cPUSkinningNoTesselationToolStripMenuItem->Checked = true;
				 this->cPUSkinningNoTesselationToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->cPUSkinningNoTesselationToolStripMenuItem->Name = L"cPUSkinningNoTesselationToolStripMenuItem";
				 this->cPUSkinningNoTesselationToolStripMenuItem->Size = System::Drawing::Size(230, 22);
				 this->cPUSkinningNoTesselationToolStripMenuItem->Text = L"CPU skinning";
				 this->cPUSkinningNoTesselationToolStripMenuItem->ToolTipText = L"No Tesselation";
				 this->cPUSkinningNoTesselationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::cPUSkinningNoTesselationToolStripMenuItem_Click);
				 // 
				 // gPUVertexSkinningnoTesselationToolStripMenuItem
				 // 
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem->Name = L"gPUVertexSkinningnoTesselationToolStripMenuItem";
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem->Size = System::Drawing::Size(230, 22);
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem->Text = L"GPU Vertex skinning";
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem->ToolTipText = L"No Tesselation";
				 this->gPUVertexSkinningnoTesselationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gPUVertexSkinningnoTesselationToolStripMenuItem_Click);
				 // 
				 // gPUTessSkinningLastOnlyToolStripMenuItem
				 // 
				 this->gPUTessSkinningLastOnlyToolStripMenuItem->Name = L"gPUTessSkinningLastOnlyToolStripMenuItem";
				 this->gPUTessSkinningLastOnlyToolStripMenuItem->Size = System::Drawing::Size(230, 22);
				 this->gPUTessSkinningLastOnlyToolStripMenuItem->Text = L"GPU Tess skinning last only";
				 this->gPUTessSkinningLastOnlyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gPUTessSkinningLastOnlyToolStripMenuItem_Click);
				 // 
				 // gPUTessSkinningAllAvarageToolStripMenuItem
				 // 
				 this->gPUTessSkinningAllAvarageToolStripMenuItem->Name = L"gPUTessSkinningAllAvarageToolStripMenuItem";
				 this->gPUTessSkinningAllAvarageToolStripMenuItem->Size = System::Drawing::Size(230, 22);
				 this->gPUTessSkinningAllAvarageToolStripMenuItem->Text = L"GPU Tess skinning all avarage";
				 this->gPUTessSkinningAllAvarageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gPUTessSkinningAllAvarageToolStripMenuItem_Click);
				 // 
				 // animationToolStripMenuItem
				 // 
				 this->animationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->startToolStripMenuItem, 
					 this->stopToolStripMenuItem, this->setBindPoseToolStripMenuItem, this->addReferencePoseToolStripMenuItem, this->saveAnimationToolStripMenuItem});
				 this->animationToolStripMenuItem->Name = L"animationToolStripMenuItem";
				 this->animationToolStripMenuItem->Size = System::Drawing::Size(75, 20);
				 this->animationToolStripMenuItem->Text = L"Animation";
				 // 
				 // startToolStripMenuItem
				 // 
				 this->startToolStripMenuItem->Name = L"startToolStripMenuItem";
				 this->startToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->startToolStripMenuItem->Text = L"Start";
				 this->startToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::startToolStripMenuItem_Click);
				 // 
				 // stopToolStripMenuItem
				 // 
				 this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
				 this->stopToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->stopToolStripMenuItem->Text = L"Stop";
				 this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::stopToolStripMenuItem_Click);
				 // 
				 // setBindPoseToolStripMenuItem
				 // 
				 this->setBindPoseToolStripMenuItem->Name = L"setBindPoseToolStripMenuItem";
				 this->setBindPoseToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->setBindPoseToolStripMenuItem->Text = L"Set Bind Pose";
				 this->setBindPoseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::setBindPoseToolStripMenuItem_Click);
				 // 
				 // addReferencePoseToolStripMenuItem
				 // 
				 this->addReferencePoseToolStripMenuItem->Name = L"addReferencePoseToolStripMenuItem";
				 this->addReferencePoseToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->addReferencePoseToolStripMenuItem->Text = L"Add Reference Pose";
				 this->addReferencePoseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addReferencePoseToolStripMenuItem_Click);
				 // 
				 // saveAnimationToolStripMenuItem
				 // 
				 this->saveAnimationToolStripMenuItem->Name = L"saveAnimationToolStripMenuItem";
				 this->saveAnimationToolStripMenuItem->Size = System::Drawing::Size(179, 22);
				 this->saveAnimationToolStripMenuItem->Text = L"Save Animation";
				 this->saveAnimationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveAnimationToolStripMenuItem_Click);
				 // 
				 // panel2
				 // 
				 this->panel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
					 | System::Windows::Forms::AnchorStyles::Left));
				 this->panel2->Controls->Add(this->tabControl1);
				 this->panel2->Location = System::Drawing::Point(0, 49);
				 this->panel2->Name = L"panel2";
				 this->panel2->Size = System::Drawing::Size(176, 464);
				 this->panel2->TabIndex = 3;
				 // 
				 // tabControl1
				 // 
				 this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
					 | System::Windows::Forms::AnchorStyles::Left));
				 this->tabControl1->Controls->Add(this->tabPage1);
				 this->tabControl1->Controls->Add(this->tabPage2);
				 this->tabControl1->Location = System::Drawing::Point(4, 3);
				 this->tabControl1->Name = L"tabControl1";
				 this->tabControl1->SelectedIndex = 0;
				 this->tabControl1->Size = System::Drawing::Size(172, 449);
				 this->tabControl1->TabIndex = 16;
				 // 
				 // tabPage1
				 // 
				 this->tabPage1->BackColor = System::Drawing::SystemColors::Control;
				 this->tabPage1->Controls->Add(this->checkBox2);
				 this->tabPage1->Controls->Add(this->tabControl2);
				 this->tabPage1->Controls->Add(this->numericUpDown5);
				 this->tabPage1->Controls->Add(this->textBox1);
				 this->tabPage1->Controls->Add(this->numericUpDown1);
				 this->tabPage1->Controls->Add(this->label2);
				 this->tabPage1->Controls->Add(this->label3);
				 this->tabPage1->Controls->Add(this->label7);
				 this->tabPage1->Controls->Add(this->numericUpDown2);
				 this->tabPage1->Controls->Add(this->label6);
				 this->tabPage1->Controls->Add(this->label4);
				 this->tabPage1->Controls->Add(this->numericUpDown4);
				 this->tabPage1->Controls->Add(this->numericUpDown3);
				 this->tabPage1->Controls->Add(this->label5);
				 this->tabPage1->Location = System::Drawing::Point(4, 22);
				 this->tabPage1->Name = L"tabPage1";
				 this->tabPage1->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage1->Size = System::Drawing::Size(164, 423);
				 this->tabPage1->TabIndex = 0;
				 this->tabPage1->Text = L"Node";
				 // 
				 // checkBox2
				 // 
				 this->checkBox2->AutoSize = true;
				 this->checkBox2->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
				 this->checkBox2->Enabled = false;
				 this->checkBox2->Location = System::Drawing::Point(9, 162);
				 this->checkBox2->Name = L"checkBox2";
				 this->checkBox2->Size = System::Drawing::Size(64, 17);
				 this->checkBox2->TabIndex = 14;
				 this->checkBox2->Text = L"Capsule";
				 this->checkBox2->UseVisualStyleBackColor = true;
				 this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox2_CheckedChanged);
				 // 
				 // tabControl2
				 // 
				 this->tabControl2->Controls->Add(this->tabPage3);
				 this->tabControl2->Controls->Add(this->tabPage4);
				 this->tabControl2->Location = System::Drawing::Point(9, 185);
				 this->tabControl2->Name = L"tabControl2";
				 this->tabControl2->SelectedIndex = 0;
				 this->tabControl2->Size = System::Drawing::Size(149, 117);
				 this->tabControl2->TabIndex = 13;
				 // 
				 // tabPage3
				 // 
				 this->tabPage3->BackColor = System::Drawing::SystemColors::Control;
				 this->tabPage3->Controls->Add(this->numericUpDown10);
				 this->tabPage3->Controls->Add(this->numericUpDown9);
				 this->tabPage3->Controls->Add(this->numericUpDown8);
				 this->tabPage3->Controls->Add(this->label11);
				 this->tabPage3->Controls->Add(this->label10);
				 this->tabPage3->Controls->Add(this->label8);
				 this->tabPage3->Location = System::Drawing::Point(4, 22);
				 this->tabPage3->Name = L"tabPage3";
				 this->tabPage3->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage3->Size = System::Drawing::Size(141, 91);
				 this->tabPage3->TabIndex = 0;
				 this->tabPage3->Text = L"Scale";
				 // 
				 // numericUpDown10
				 // 
				 this->numericUpDown10->DecimalPlaces = 2;
				 this->numericUpDown10->Enabled = false;
				 this->numericUpDown10->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->numericUpDown10->Location = System::Drawing::Point(31, 63);
				 this->numericUpDown10->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, System::Int32::MinValue});
				 this->numericUpDown10->Name = L"numericUpDown10";
				 this->numericUpDown10->ReadOnly = true;
				 this->numericUpDown10->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown10->TabIndex = 5;
				 this->numericUpDown10->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown10->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown10_ValueChanged);
				 // 
				 // numericUpDown9
				 // 
				 this->numericUpDown9->DecimalPlaces = 2;
				 this->numericUpDown9->Enabled = false;
				 this->numericUpDown9->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->numericUpDown9->Location = System::Drawing::Point(31, 36);
				 this->numericUpDown9->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, System::Int32::MinValue});
				 this->numericUpDown9->Name = L"numericUpDown9";
				 this->numericUpDown9->ReadOnly = true;
				 this->numericUpDown9->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown9->TabIndex = 4;
				 this->numericUpDown9->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown9->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown9_ValueChanged);
				 // 
				 // numericUpDown8
				 // 
				 this->numericUpDown8->DecimalPlaces = 2;
				 this->numericUpDown8->Enabled = false;
				 this->numericUpDown8->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->numericUpDown8->Location = System::Drawing::Point(31, 9);
				 this->numericUpDown8->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, System::Int32::MinValue});
				 this->numericUpDown8->Name = L"numericUpDown8";
				 this->numericUpDown8->ReadOnly = true;
				 this->numericUpDown8->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown8->TabIndex = 3;
				 this->numericUpDown8->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown8->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown8_ValueChanged);
				 // 
				 // label11
				 // 
				 this->label11->AutoSize = true;
				 this->label11->Location = System::Drawing::Point(6, 65);
				 this->label11->Name = L"label11";
				 this->label11->Size = System::Drawing::Size(17, 13);
				 this->label11->TabIndex = 2;
				 this->label11->Text = L"Z:";
				 // 
				 // label10
				 // 
				 this->label10->AutoSize = true;
				 this->label10->Location = System::Drawing::Point(6, 38);
				 this->label10->Name = L"label10";
				 this->label10->Size = System::Drawing::Size(17, 13);
				 this->label10->TabIndex = 1;
				 this->label10->Text = L"Y:";
				 // 
				 // label8
				 // 
				 this->label8->AutoSize = true;
				 this->label8->Location = System::Drawing::Point(6, 11);
				 this->label8->Name = L"label8";
				 this->label8->Size = System::Drawing::Size(17, 13);
				 this->label8->TabIndex = 0;
				 this->label8->Text = L"X:";
				 // 
				 // tabPage4
				 // 
				 this->tabPage4->BackColor = System::Drawing::SystemColors::Control;
				 this->tabPage4->Controls->Add(this->label14);
				 this->tabPage4->Controls->Add(this->label13);
				 this->tabPage4->Controls->Add(this->numericUpDown13);
				 this->tabPage4->Controls->Add(this->numericUpDown12);
				 this->tabPage4->Controls->Add(this->numericUpDown11);
				 this->tabPage4->Controls->Add(this->label12);
				 this->tabPage4->Location = System::Drawing::Point(4, 22);
				 this->tabPage4->Name = L"tabPage4";
				 this->tabPage4->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage4->Size = System::Drawing::Size(141, 91);
				 this->tabPage4->TabIndex = 1;
				 this->tabPage4->Text = L"Rotate";
				 // 
				 // label14
				 // 
				 this->label14->AutoSize = true;
				 this->label14->Location = System::Drawing::Point(6, 65);
				 this->label14->Name = L"label14";
				 this->label14->Size = System::Drawing::Size(17, 13);
				 this->label14->TabIndex = 5;
				 this->label14->Text = L"Z:";
				 // 
				 // label13
				 // 
				 this->label13->AutoSize = true;
				 this->label13->Location = System::Drawing::Point(6, 38);
				 this->label13->Name = L"label13";
				 this->label13->Size = System::Drawing::Size(17, 13);
				 this->label13->TabIndex = 4;
				 this->label13->Text = L"Y:";
				 // 
				 // numericUpDown13
				 // 
				 this->numericUpDown13->Enabled = false;
				 this->numericUpDown13->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
				 this->numericUpDown13->Location = System::Drawing::Point(31, 63);
				 this->numericUpDown13->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
				 this->numericUpDown13->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
				 this->numericUpDown13->Name = L"numericUpDown13";
				 this->numericUpDown13->ReadOnly = true;
				 this->numericUpDown13->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown13->TabIndex = 3;
				 this->numericUpDown13->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown13_ValueChanged);
				 // 
				 // numericUpDown12
				 // 
				 this->numericUpDown12->Enabled = false;
				 this->numericUpDown12->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
				 this->numericUpDown12->Location = System::Drawing::Point(31, 36);
				 this->numericUpDown12->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
				 this->numericUpDown12->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
				 this->numericUpDown12->Name = L"numericUpDown12";
				 this->numericUpDown12->ReadOnly = true;
				 this->numericUpDown12->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown12->TabIndex = 2;
				 this->numericUpDown12->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown12_ValueChanged);
				 // 
				 // numericUpDown11
				 // 
				 this->numericUpDown11->Enabled = false;
				 this->numericUpDown11->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
				 this->numericUpDown11->Location = System::Drawing::Point(31, 9);
				 this->numericUpDown11->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
				 this->numericUpDown11->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
				 this->numericUpDown11->Name = L"numericUpDown11";
				 this->numericUpDown11->ReadOnly = true;
				 this->numericUpDown11->Size = System::Drawing::Size(104, 20);
				 this->numericUpDown11->TabIndex = 1;
				 this->numericUpDown11->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown11_ValueChanged);
				 // 
				 // label12
				 // 
				 this->label12->AutoSize = true;
				 this->label12->Location = System::Drawing::Point(6, 11);
				 this->label12->Name = L"label12";
				 this->label12->Size = System::Drawing::Size(17, 13);
				 this->label12->TabIndex = 0;
				 this->label12->Text = L"X:";
				 // 
				 // numericUpDown5
				 // 
				 this->numericUpDown5->Enabled = false;
				 this->numericUpDown5->Location = System::Drawing::Point(70, 136);
				 this->numericUpDown5->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown5->Name = L"numericUpDown5";
				 this->numericUpDown5->ReadOnly = true;
				 this->numericUpDown5->Size = System::Drawing::Size(91, 20);
				 this->numericUpDown5->TabIndex = 12;
				 this->numericUpDown5->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown5->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown5_ValueChanged);
				 // 
				 // textBox1
				 // 
				 this->textBox1->Enabled = false;
				 this->textBox1->Location = System::Drawing::Point(29, 6);
				 this->textBox1->Name = L"textBox1";
				 this->textBox1->ReadOnly = true;
				 this->textBox1->Size = System::Drawing::Size(132, 20);
				 this->textBox1->TabIndex = 1;
				 this->textBox1->Text = L"no node selected";
				 // 
				 // numericUpDown1
				 // 
				 this->numericUpDown1->DecimalPlaces = 2;
				 this->numericUpDown1->Enabled = false;
				 this->numericUpDown1->Location = System::Drawing::Point(29, 32);
				 this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown1->Name = L"numericUpDown1";
				 this->numericUpDown1->ReadOnly = true;
				 this->numericUpDown1->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown1->TabIndex = 2;
				 this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
				 // 
				 // label2
				 // 
				 this->label2->AutoSize = true;
				 this->label2->Location = System::Drawing::Point(3, 9);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(21, 13);
				 this->label2->TabIndex = 3;
				 this->label2->Text = L"ID:";
				 // 
				 // label3
				 // 
				 this->label3->AutoSize = true;
				 this->label3->Location = System::Drawing::Point(6, 35);
				 this->label3->Name = L"label3";
				 this->label3->Size = System::Drawing::Size(17, 13);
				 this->label3->TabIndex = 4;
				 this->label3->Text = L"X:";
				 // 
				 // label7
				 // 
				 this->label7->AutoSize = true;
				 this->label7->Location = System::Drawing::Point(6, 139);
				 this->label7->Name = L"label7";
				 this->label7->Size = System::Drawing::Size(58, 13);
				 this->label7->TabIndex = 11;
				 this->label7->Text = L"Tess level:";
				 // 
				 // numericUpDown2
				 // 
				 this->numericUpDown2->DecimalPlaces = 2;
				 this->numericUpDown2->Enabled = false;
				 this->numericUpDown2->Location = System::Drawing::Point(29, 58);
				 this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown2->Name = L"numericUpDown2";
				 this->numericUpDown2->ReadOnly = true;
				 this->numericUpDown2->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown2->TabIndex = 5;
				 this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown2_ValueChanged);
				 // 
				 // label6
				 // 
				 this->label6->AutoSize = true;
				 this->label6->Location = System::Drawing::Point(6, 113);
				 this->label6->Name = L"label6";
				 this->label6->Size = System::Drawing::Size(43, 13);
				 this->label6->TabIndex = 10;
				 this->label6->Text = L"Radius:";
				 // 
				 // label4
				 // 
				 this->label4->AutoSize = true;
				 this->label4->Location = System::Drawing::Point(6, 61);
				 this->label4->Name = L"label4";
				 this->label4->Size = System::Drawing::Size(17, 13);
				 this->label4->TabIndex = 6;
				 this->label4->Text = L"Y:";
				 // 
				 // numericUpDown4
				 // 
				 this->numericUpDown4->DecimalPlaces = 2;
				 this->numericUpDown4->Enabled = false;
				 this->numericUpDown4->Location = System::Drawing::Point(55, 110);
				 this->numericUpDown4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown4->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown4->Name = L"numericUpDown4";
				 this->numericUpDown4->ReadOnly = true;
				 this->numericUpDown4->Size = System::Drawing::Size(106, 20);
				 this->numericUpDown4->TabIndex = 9;
				 this->numericUpDown4->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown4->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown4_ValueChanged);
				 // 
				 // numericUpDown3
				 // 
				 this->numericUpDown3->DecimalPlaces = 2;
				 this->numericUpDown3->Enabled = false;
				 this->numericUpDown3->Location = System::Drawing::Point(29, 84);
				 this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown3->Name = L"numericUpDown3";
				 this->numericUpDown3->ReadOnly = true;
				 this->numericUpDown3->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown3->TabIndex = 7;
				 this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown3_ValueChanged);
				 // 
				 // label5
				 // 
				 this->label5->AutoSize = true;
				 this->label5->Location = System::Drawing::Point(6, 87);
				 this->label5->Name = L"label5";
				 this->label5->Size = System::Drawing::Size(17, 13);
				 this->label5->TabIndex = 8;
				 this->label5->Text = L"Z:";
				 // 
				 // tabPage2
				 // 
				 this->tabPage2->BackColor = System::Drawing::SystemColors::Control;
				 this->tabPage2->Controls->Add(this->checkBox1);
				 this->tabPage2->Controls->Add(this->numericUpDown7);
				 this->tabPage2->Controls->Add(this->label1);
				 this->tabPage2->Controls->Add(this->numericUpDown6);
				 this->tabPage2->Controls->Add(this->label9);
				 this->tabPage2->Location = System::Drawing::Point(4, 22);
				 this->tabPage2->Name = L"tabPage2";
				 this->tabPage2->Padding = System::Windows::Forms::Padding(3);
				 this->tabPage2->Size = System::Drawing::Size(164, 423);
				 this->tabPage2->TabIndex = 1;
				 this->tabPage2->Text = L"Global";
				 // 
				 // checkBox1
				 // 
				 this->checkBox1->AutoSize = true;
				 this->checkBox1->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
				 this->checkBox1->Checked = true;
				 this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->checkBox1->Location = System::Drawing::Point(6, 58);
				 this->checkBox1->Name = L"checkBox1";
				 this->checkBox1->Size = System::Drawing::Size(133, 17);
				 this->checkBox1->TabIndex = 18;
				 this->checkBox1->Text = L"Use Capsules Attribute";
				 this->checkBox1->UseVisualStyleBackColor = true;
				 this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
				 // 
				 // numericUpDown7
				 // 
				 this->numericUpDown7->DecimalPlaces = 2;
				 this->numericUpDown7->Enabled = false;
				 this->numericUpDown7->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
				 this->numericUpDown7->Location = System::Drawing::Point(68, 32);
				 this->numericUpDown7->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown7->Name = L"numericUpDown7";
				 this->numericUpDown7->ReadOnly = true;
				 this->numericUpDown7->Size = System::Drawing::Size(87, 20);
				 this->numericUpDown7->TabIndex = 17;
				 this->numericUpDown7->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {85, 0, 0, 131072});
				 this->numericUpDown7->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown7_ValueChanged);
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->Location = System::Drawing::Point(3, 34);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(57, 13);
				 this->label1->TabIndex = 16;
				 this->label1->Text = L"Threshold:";
				 // 
				 // numericUpDown6
				 // 
				 this->numericUpDown6->Enabled = false;
				 this->numericUpDown6->Location = System::Drawing::Point(68, 6);
				 this->numericUpDown6->Name = L"numericUpDown6";
				 this->numericUpDown6->ReadOnly = true;
				 this->numericUpDown6->Size = System::Drawing::Size(87, 20);
				 this->numericUpDown6->TabIndex = 13;
				 this->numericUpDown6->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown6_ValueChanged);
				 // 
				 // label9
				 // 
				 this->label9->AutoSize = true;
				 this->label9->Location = System::Drawing::Point(0, 8);
				 this->label9->Name = L"label9";
				 this->label9->Size = System::Drawing::Size(62, 13);
				 this->label9->TabIndex = 15;
				 this->label9->Text = L"Tess Level:";
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(744, 513);
				 this->Controls->Add(this->panel2);
				 this->Controls->Add(this->panel1);
				 this->Controls->Add(this->toolStrip1);
				 this->Controls->Add(this->menuStrip1);
				 this->MainMenuStrip = this->menuStrip1;
				 this->Name = L"Form1";
				 this->Text = L"Base Manfild Mesh from Skeleton";
				 this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Panel1_MouseWheel);
				 this->toolStrip1->ResumeLayout(false);
				 this->toolStrip1->PerformLayout();
				 this->menuStrip1->ResumeLayout(false);
				 this->menuStrip1->PerformLayout();
				 this->panel2->ResumeLayout(false);
				 this->tabControl1->ResumeLayout(false);
				 this->tabPage1->ResumeLayout(false);
				 this->tabPage1->PerformLayout();
				 this->tabControl2->ResumeLayout(false);
				 this->tabPage3->ResumeLayout(false);
				 this->tabPage3->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown10))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown9))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown8))->EndInit();
				 this->tabPage4->ResumeLayout(false);
				 this->tabPage4->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown13))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown12))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown11))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
				 this->tabPage2->ResumeLayout(false);
				 this->tabPage2->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown7))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown6))->EndInit();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 UNREFERENCED_PARAMETER(sender);
				 UNREFERENCED_PARAMETER(e);
				 OpenGL->Render();
				 OpenGL->SwapOpenGLBuffers();
			 }

#pragma region Mouse Handling
			 void InvalidateNodeGUI() {
				 this->numericUpDown1->ReadOnly = true;
				 this->numericUpDown2->ReadOnly = true;
				 this->numericUpDown3->ReadOnly = true;
				 this->numericUpDown4->ReadOnly = true;
				 this->numericUpDown5->ReadOnly = true;
				 this->numericUpDown8->ReadOnly = true;
				 this->numericUpDown9->ReadOnly = true;
				 this->numericUpDown10->ReadOnly = true;
				 this->numericUpDown11->ReadOnly = true;
				 this->numericUpDown12->ReadOnly = true;
				 this->numericUpDown13->ReadOnly = true;

				 this->numericUpDown1->Enabled = false;
				 this->numericUpDown2->Enabled = false;
				 this->numericUpDown3->Enabled = false;
				 this->numericUpDown4->Enabled = false;
				 this->numericUpDown5->Enabled = false;
				 this->numericUpDown8->Enabled = false;
				 this->numericUpDown9->Enabled = false;
				 this->numericUpDown10->Enabled = false;
				 this->numericUpDown11->Enabled = false;
				 this->numericUpDown12->Enabled = false;
				 this->numericUpDown13->Enabled = false;
				 this->checkBox2->Enabled = false;

				 this->panel1->Focus();
			 }
			 void UpdateNodeGUI() {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 System::String^ stringID = gcnew System::String(node->getIdStr().c_str());
					 this->textBox1->Text = stringID;
					 this->numericUpDown1->Value = System::Decimal(node->getX());
					 this->numericUpDown2->Value = System::Decimal(node->getY());;
					 this->numericUpDown3->Value = System::Decimal(node->getZ());;
					 this->numericUpDown4->Value = System::Decimal(node->getNodeRadius());
					 this->numericUpDown5->Value = System::Decimal(node->getTessLevel());
					 this->numericUpDown8->Value = System::Decimal(node->getScaleX());
					 this->numericUpDown9->Value = System::Decimal(node->getScaleY());
					 this->numericUpDown10->Value = System::Decimal(node->getScaleZ());
					 this->numericUpDown11->Value = System::Decimal(node->getRotateX());
					 this->numericUpDown12->Value = System::Decimal(node->getRotateY());
					 this->numericUpDown13->Value = System::Decimal(node->getRotateZ());
					 this->checkBox2->Checked = node->getIsCapsule();
				 }
			 }
			 void ReEnableNodeGUI() {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 this->numericUpDown1->ReadOnly = false;
					 this->numericUpDown2->ReadOnly = false;
					 this->numericUpDown3->ReadOnly = false;
					 this->numericUpDown4->ReadOnly = false;
					 this->numericUpDown5->ReadOnly = false;
					 this->numericUpDown8->ReadOnly = false;
					 this->numericUpDown9->ReadOnly = false;
					 this->numericUpDown10->ReadOnly = false;
					 this->numericUpDown11->ReadOnly = false;
					 this->numericUpDown12->ReadOnly = false;
					 this->numericUpDown13->ReadOnly = false;

					 this->numericUpDown1->Enabled = true;
					 this->numericUpDown2->Enabled = true;
					 this->numericUpDown3->Enabled = true;
					 this->numericUpDown4->Enabled = true;
					 this->numericUpDown5->Enabled = true;
					 this->numericUpDown8->Enabled = true;
					 this->numericUpDown9->Enabled = true;
					 this->numericUpDown10->Enabled = true;
					 this->numericUpDown11->Enabled = true;
					 this->numericUpDown12->Enabled = true;
					 this->numericUpDown13->Enabled = true;
					 this->checkBox2->Enabled = true;
				 } else {
					 this->textBox1->Text = L"no node selected";
					 this->numericUpDown1->Value = System::Decimal(0);
					 this->numericUpDown2->Value = System::Decimal(0);
					 this->numericUpDown3->Value = System::Decimal(0);
					 this->numericUpDown4->Value = System::Decimal(1);
					 this->numericUpDown5->Value = System::Decimal(1);
					 this->numericUpDown8->Value = System::Decimal(1);
					 this->numericUpDown9->Value = System::Decimal(1);
					 this->numericUpDown10->Value = System::Decimal(1);
					 this->numericUpDown11->Value = System::Decimal(0);
					 this->numericUpDown12->Value = System::Decimal(0);
					 this->numericUpDown13->Value = System::Decimal(0);
					 this->checkBox2->Checked = false;
				 }
			 }

			 void Panel1_MouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ) {
				 //lastPosition.X = e->X;
				 //lastPosition.Y = e->Y;
				 InvalidateNodeGUI();
				 int mouseFlags = 0;
				 if (e->Button == ::MouseButtons::Right)
					 mouseFlags |= RIGHT_MOUSE_DOWN;
				 if (e->Button == ::MouseButtons::Left)
					 mouseFlags |= LEFT_MOUSE_DOWN;				 
				 if (e->Button == ::MouseButtons::Middle)
					 mouseFlags |= MIDDLE_MOUSE_DOWN;
				 OpenGL->glEventHandler->mouseDown(e->X, e->Y, mouseFlags);
				 UpdateNodeGUI();
			 }
			 void Panel1_MouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
				 /*// Update the mouse path that is drawn onto the Panel. 
				 float dx = e->X - lastPosition.X;
				 float dy = e->Y - lastPosition.Y;
				 //rotate scene
				 if (e->Button == ::MouseButtons::Right) {
				 GLCamera *glCamera = OpenGL->glCamera;
				 glCamera->setFi(glCamera->fi - dx*0.3);
				 glCamera->setTheta(glCamera->theta - dy*0.3);
				 //OpenGL->cameraFi += dx * 0.3;
				 //OpenGL->cameraTheta += dy * 0.3;
				 }
				 //move scene
				 if (e->Button == ::MouseButtons::Left) {
				 GLCamera *glCamera = OpenGL->glCamera;
				 glCamera->strafeHorizontal(dx*0.3);
				 glCamera->strafeVertical(dy*0.3);
				 }
				 lastPosition.X = e->X;
				 lastPosition.Y = e->Y;
				 this->Invalidate();*/
				 OpenGL->glEventHandler->mouseMoved(e->X, e->Y);
				 UpdateNodeGUI();
			 }

			 void Panel1_MouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
				 if (Control::ModifierKeys == Keys::Shift)
					 OpenGL->glCamera->setDist(OpenGL->glCamera->dist - e->Delta/20);
				 else
					 OpenGL->glCamera->setDist(OpenGL->glCamera->dist - e->Delta);

				 this->Invalidate();
			 }

			 void Panel1_MouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
				 OpenGL->glEventHandler->mouseUp(e->X, e->Y);
				 ReEnableNodeGUI();
			 }

	private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) {
				 OpenGL->glEventHandler->KeyDown(e->KeyValue);
			 }

			 void Form1_KeyPress(Object^ sender, KeyPressEventArgs^ e)
			 {
			 }

#pragma endregion
			 void refreshCheckBoxes() {
				 noSmoothingToolStripMenuItem->Checked = false;
				 avaragingToolStripMenuItem->Checked = true;
				 quaternionToolStripMenuItem->Checked = false;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = false;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = false;

				 editSkeletonToolStripMenuItem->Checked = true;
				 straightenToolStripMenuItem1->Checked = false;
				 computeConvexHullToolStripMenuItem1->Checked = false;
				 subdivideConvexHullToolStripMenuItem1->Checked = false;
				 joinBranchNodePolyhedronsToolStripMenuItem->Checked = false;
				 straightenMeshToolStripMenuItem->Checked = false;
			 }
			 void turnOffGlobal() {
				 this->numericUpDown6->Enabled = false;
				 this->numericUpDown6->ReadOnly = true;

				 this->numericUpDown7->Enabled = false;
				 this->numericUpDown7->ReadOnly = true;
			 }
			 void turnOnGlobal() {
				 this->numericUpDown6->Enabled = true;
				 this->numericUpDown6->ReadOnly = false;

				 this->numericUpDown7->Enabled = true;
				 this->numericUpDown7->ReadOnly = false;
			 }
			 void checkSQMMenu(int idx) {
				 editSkeletonToolStripMenuItem->Checked = (idx == 0) ? true : false;
				 straightenToolStripMenuItem1->Checked = (idx == 1) ? true : false;
				 computeConvexHullToolStripMenuItem1->Checked = (idx == 2) ? true : false;
				 subdivideConvexHullToolStripMenuItem1->Checked = (idx == 3) ? true : false;
				 joinBranchNodePolyhedronsToolStripMenuItem->Checked = (idx == 4) ? true : false;
				 straightenMeshToolStripMenuItem->Checked = (idx == 5) ? true : false;
				 checkBox1->Enabled = (idx == 0) ? true : false;
				 if (idx != 0) InvalidateNodeGUI();
			 }
			 void checkSmoothingMenu(int idx) {
				 noSmoothingToolStripMenuItem->Checked = (idx == 0) ? true : false;
				 avaragingToolStripMenuItem->Checked = (idx == 1) ? true : false;
				 quaternionToolStripMenuItem->Checked = (idx == 2) ? true : false;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = (idx == 3) ? true : false;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = (idx == 4) ? true : false;
			 }
			 void checkSkinningMenu(int idx) {
				 cPUSkinningNoTesselationToolStripMenuItem->Checked = (idx == 0) ? true : false;
				 gPUVertexSkinningnoTesselationToolStripMenuItem->Checked = (idx == 1) ? true : false;
				 gPUTessSkinningLastOnlyToolStripMenuItem->Checked = (idx == 2) ? true : false;
				 gPUTessSkinningAllAvarageToolStripMenuItem->Checked = (idx == 3) ? true : false;
			 }
			 void Recalculate() {
				 SQMState state = OpenGL->getSQMController()->getState();
				 OpenGL->restartSQMAlgorithm();
				 OpenGL->executeSQMAlgorithm(state);
			 }
	private: System::Void newToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->newFile();
				 refreshCheckBoxes();
				 turnOffGlobal();
			 }
	private: System::Void loadToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 string fileName = "";
					 MarshalString(this->openFileDialog1->FileName, fileName);
					 //MessageBox::Show(this->openFileDialog1->FileName);
					 OpenGL->loadSkeletonFromFile(fileName);
					 refreshCheckBoxes();
					 turnOffGlobal();
				 }
			 }
	private: System::Void saveToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 string fileName = "";
					 MarshalString(this->saveFileDialog1->FileName, fileName);
					 OpenGL->saveSkeletonToFile(fileName);
				 }
			 }
	private: System::Void exportMeshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 string fileName = "";
					 MarshalString(this->saveFileDialog1->FileName, fileName);
					 OpenGL->exportMeshToFile(fileName);
				 }
			 }
	private: System::Void editSkeletonToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->restartSQMAlgorithm();
				 checkSQMMenu(0);

				 turnOffGlobal();
			 }
	private: System::Void straightenToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMStraighten);
				 checkSQMMenu(1);
				 SQMState state = OpenGL->getSQMController()->getState();
				 if (state == SQMJoinBNPs) {
					 checkSQMMenu(4);
				 }
				 if (state == SQMFinalPlacement) {
					 checkSQMMenu(5);
				 }

				 turnOffGlobal();
			 }
	private: System::Void computeConvexHullToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMComputeConvexHull);
				 checkSQMMenu(2);

				 turnOffGlobal();
			 }
	private: System::Void subdivideConvexHullToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMSubdivideConvexHull);
				 checkSQMMenu(3);

				 turnOffGlobal();
			 }
	private: System::Void joinBranchNodePolyhedronsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMJoinBNPs);
				 checkSQMMenu(4);

				 turnOnGlobal();
			 }
	private: System::Void straightenMeshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMFinalPlacement);
				 checkSQMMenu(5);

				 turnOnGlobal();
			 }
	private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->glEventHandler->state = NodeEditDescendantsState;
			 }
	private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->glEventHandler->state = NodeEditState;
			 }
	private: System::Void panel1_Resize(System::Object^  sender, System::EventArgs^  e) {
				 Control^ control = dynamic_cast<Control^>(sender);
				 if (OpenGL != nullptr)
					 OpenGL->resize((GLsizei)control->Size.Width, (GLsizei)control->Size.Height);
			 }
	private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown1->Value);
					 OpenGL->getSQMController()->setSelectedX(value);
				 }
			 }
	private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown2->Value);
					 OpenGL->getSQMController()->setSelectedY(value);
				 }
			 }
	private: System::Void numericUpDown3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown3->Value);
					 OpenGL->getSQMController()->setSelectedZ(value);
				 }
			 }
	private: System::Void numericUpDown4_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown4->Value);
					 OpenGL->getSQMController()->setSelectedRadius(value);
				 }
			 }
	private: System::Void numericUpDown5_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown5->Value);
					 OpenGL->getSQMController()->setSelectedTessLevel(value);
				 }
			 }
	private: System::Void numericUpDown6_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 float value = System::Decimal::ToSingle(this->numericUpDown6->Value);
				 OpenGL->setGlobalTesselation(value);
				 this->panel1->Focus();
			 }
	private: System::Void restartToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->restartSQMAlgorithm();
			 }
	private: System::Void wireframeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->alterWireframe();
			 }
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 Application::Exit();
			 }
	private: System::Void noSmoothingToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkSmoothingMenu(0);

				 OpenGL->setSmoothingOff();
			 }
	private: System::Void avaragingToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkSmoothingMenu(1);

				 OpenGL->setAvaragingSmoothing();
			 }
	private: System::Void quaternionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkSmoothingMenu(2);

				 OpenGL->setQuaternionSmoothing();
			 }
	private: System::Void valencyWeightedLaplacianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkSmoothingMenu(3);

				 OpenGL->setValencyWeightedLaplacian();
			 }
	private: System::Void oneRingAreaWeightedLaplacianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkSmoothingMenu(4);

				 OpenGL->setOneRingWeightedLaplacian();
			 }
	private: System::Void normalsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->alterDrawNormals();
			 }
	private: System::Void reloadShadersToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->ReloadShaders();
			 }
	private: System::Void numericUpDown7_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 float value = System::Decimal::ToSingle(this->numericUpDown7->Value);
				 OpenGL->setGlobalThreshold(value);
				 this->panel1->Focus();
			 }
	private: System::Void numericUpDown8_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown8->Value);
					 OpenGL->getSQMController()->setSelectedScaleX(value);
				 }
			 }
	private: System::Void numericUpDown9_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown9->Value);
					 OpenGL->getSQMController()->setSelectedScaleY(value);
				 }
			 }
	private: System::Void numericUpDown10_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown10->Value);
					 OpenGL->getSQMController()->setSelectedScaleZ(value);
				 }
			 }
	private: System::Void numericUpDown11_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown11->Value);
					 OpenGL->getSQMController()->setSelectedRotateX(value);
				 }
			 }
	private: System::Void numericUpDown12_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown12->Value);
					 OpenGL->getSQMController()->setSelectedRotateY(value);
				 }
			 }
	private: System::Void numericUpDown13_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 float value = System::Decimal::ToSingle(this->numericUpDown13->Value);
					 OpenGL->getSQMController()->setSelectedRotateZ(value);
				 }
			 }
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->setGlobalUseCapsules(checkBox1->Checked);
			 }
	private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 node->setIsCapsule(checkBox2->Checked);
				 }
			 }
	private: System::Void cPUSkinningNoTesselationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 bool shouldRecalculate = OpenGL->setSkinningType(CPU);
				 checkSkinningMenu(0);
				 if (shouldRecalculate) Recalculate();
			 }
	private: System::Void gPUVertexSkinningnoTesselationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 bool shouldRecalculate = OpenGL->setSkinningType(GPUVertex);
				 checkSkinningMenu(1);
				 if (shouldRecalculate) Recalculate();
			 }
	private: System::Void gPUTessSkinningLastOnlyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 bool shouldRecalculate = OpenGL->setSkinningType(GPUTessLast);
				 checkSkinningMenu(2);
				 if (shouldRecalculate) Recalculate();
			 }
	private: System::Void gPUTessSkinningAllAvarageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 bool shouldRecalculate = OpenGL->setSkinningType(GPUTessAvarage);
				 checkSkinningMenu(3);
				 if (shouldRecalculate) Recalculate();
			 }
	private: System::Void toolStripButton3_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->glEventHandler->state = NodeRotateState;
			 }
	private: System::Void setBindPoseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->setBindPose();
			 }
	private: System::Void addReferencePoseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->setReferencePose();
			 }
	private: System::Void saveAnimationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 string fileName = "";
					 MarshalString(this->saveFileDialog1->FileName, fileName);
					 OpenGL->saveAnimation(fileName);
				 }
			 }
	private: System::Void startToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->StartAnimation();
			 }
	private: System::Void stopToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->StopAnimation();
			 }
};
}

