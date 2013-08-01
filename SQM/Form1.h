#pragma once

#include "OpenGL.h"
#include <iostream>
#include <string>

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
	private: System::Windows::Forms::Label^  label1;
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
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  restartToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  wireframeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  quaternionSmoothingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  quaternionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  valencyWeightedLaplacianToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  oneRingAreaWeightedLaplacianToolStripMenuItem;





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
				 this->straightenToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->computeConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->subdivideConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->joinBranchNodePolyhedronsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->straightenMeshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->restartToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->showToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->wireframeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->panel2 = (gcnew System::Windows::Forms::Panel());
				 this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label7 = (gcnew System::Windows::Forms::Label());
				 this->label6 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label5 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label4 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->label3 = (gcnew System::Windows::Forms::Label());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
				 this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->quaternionSmoothingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->quaternionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->valencyWeightedLaplacianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->oneRingAreaWeightedLaplacianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStrip1->SuspendLayout();
				 this->menuStrip1->SuspendLayout();
				 this->panel2->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
				 this->SuspendLayout();
				 // 
				 // timer1
				 // 
				 this->timer1->Enabled = true;
				 this->timer1->Interval = 10;
				 this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
				 // 
				 // toolStrip1
				 // 
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripButton1, 
					 this->toolStripButton2});
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
				 this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
				 // 
				 // menuStrip1
				 // 
				 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->fileToolStripMenuItem, 
					 this->sQMToolStripMenuItem, this->editToolStripMenuItem, this->showToolStripMenuItem});
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
				 this->sQMToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->straightenToolStripMenuItem1, 
					 this->computeConvexHullToolStripMenuItem1, this->subdivideConvexHullToolStripMenuItem1, this->joinBranchNodePolyhedronsToolStripMenuItem, 
					 this->straightenMeshToolStripMenuItem});
				 this->sQMToolStripMenuItem->Name = L"sQMToolStripMenuItem";
				 this->sQMToolStripMenuItem->Size = System::Drawing::Size(45, 20);
				 this->sQMToolStripMenuItem->Text = L"&SQM";
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
				 this->subdivideConvexHullToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->quaternionSmoothingToolStripMenuItem, 
					 this->toolStripSeparator2, this->quaternionToolStripMenuItem, this->valencyWeightedLaplacianToolStripMenuItem, this->oneRingAreaWeightedLaplacianToolStripMenuItem});
				 this->subdivideConvexHullToolStripMenuItem1->Name = L"subdivideConvexHullToolStripMenuItem1";
				 this->subdivideConvexHullToolStripMenuItem1->Size = System::Drawing::Size(236, 22);
				 this->subdivideConvexHullToolStripMenuItem1->Text = L"Subdivide Convex Hull";
				 this->subdivideConvexHullToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::subdivideConvexHullToolStripMenuItem1_Click);
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
				 // editToolStripMenuItem
				 // 
				 this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->restartToolStripMenuItem});
				 this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
				 this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
				 this->editToolStripMenuItem->Text = L"&Edit";
				 // 
				 // restartToolStripMenuItem
				 // 
				 this->restartToolStripMenuItem->Name = L"restartToolStripMenuItem";
				 this->restartToolStripMenuItem->Size = System::Drawing::Size(142, 22);
				 this->restartToolStripMenuItem->Text = L"Edit Skeleton";
				 this->restartToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::restartToolStripMenuItem_Click);
				 // 
				 // showToolStripMenuItem
				 // 
				 this->showToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->wireframeToolStripMenuItem});
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
				 this->wireframeToolStripMenuItem->Size = System::Drawing::Size(152, 22);
				 this->wireframeToolStripMenuItem->Text = L"Wireframe";
				 this->wireframeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::wireframeToolStripMenuItem_Click);
				 // 
				 // panel2
				 // 
				 this->panel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
					 | System::Windows::Forms::AnchorStyles::Left));
				 this->panel2->Controls->Add(this->numericUpDown5);
				 this->panel2->Controls->Add(this->label7);
				 this->panel2->Controls->Add(this->label6);
				 this->panel2->Controls->Add(this->numericUpDown4);
				 this->panel2->Controls->Add(this->label5);
				 this->panel2->Controls->Add(this->numericUpDown3);
				 this->panel2->Controls->Add(this->label4);
				 this->panel2->Controls->Add(this->numericUpDown2);
				 this->panel2->Controls->Add(this->label3);
				 this->panel2->Controls->Add(this->label2);
				 this->panel2->Controls->Add(this->numericUpDown1);
				 this->panel2->Controls->Add(this->textBox1);
				 this->panel2->Controls->Add(this->label1);
				 this->panel2->Location = System::Drawing::Point(0, 49);
				 this->panel2->Name = L"panel2";
				 this->panel2->Size = System::Drawing::Size(176, 464);
				 this->panel2->TabIndex = 3;
				 // 
				 // numericUpDown5
				 // 
				 this->numericUpDown5->Enabled = false;
				 this->numericUpDown5->Location = System::Drawing::Point(72, 155);
				 this->numericUpDown5->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
				 this->numericUpDown5->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown5->Name = L"numericUpDown5";
				 this->numericUpDown5->ReadOnly = true;
				 this->numericUpDown5->Size = System::Drawing::Size(91, 20);
				 this->numericUpDown5->TabIndex = 12;
				 this->numericUpDown5->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown5->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown5_ValueChanged);
				 // 
				 // label7
				 // 
				 this->label7->AutoSize = true;
				 this->label7->Location = System::Drawing::Point(8, 158);
				 this->label7->Name = L"label7";
				 this->label7->Size = System::Drawing::Size(58, 13);
				 this->label7->TabIndex = 11;
				 this->label7->Text = L"Tess level:";
				 // 
				 // label6
				 // 
				 this->label6->AutoSize = true;
				 this->label6->Location = System::Drawing::Point(8, 132);
				 this->label6->Name = L"label6";
				 this->label6->Size = System::Drawing::Size(43, 13);
				 this->label6->TabIndex = 10;
				 this->label6->Text = L"Radius:";
				 // 
				 // numericUpDown4
				 // 
				 this->numericUpDown4->DecimalPlaces = 2;
				 this->numericUpDown4->Enabled = false;
				 this->numericUpDown4->Location = System::Drawing::Point(57, 129);
				 this->numericUpDown4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown4->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown4->Name = L"numericUpDown4";
				 this->numericUpDown4->ReadOnly = true;
				 this->numericUpDown4->Size = System::Drawing::Size(106, 20);
				 this->numericUpDown4->TabIndex = 9;
				 this->numericUpDown4->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->numericUpDown4->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown4_ValueChanged);
				 // 
				 // label5
				 // 
				 this->label5->AutoSize = true;
				 this->label5->Location = System::Drawing::Point(8, 106);
				 this->label5->Name = L"label5";
				 this->label5->Size = System::Drawing::Size(17, 13);
				 this->label5->TabIndex = 8;
				 this->label5->Text = L"Z:";
				 // 
				 // numericUpDown3
				 // 
				 this->numericUpDown3->DecimalPlaces = 2;
				 this->numericUpDown3->Enabled = false;
				 this->numericUpDown3->Location = System::Drawing::Point(31, 103);
				 this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown3->Name = L"numericUpDown3";
				 this->numericUpDown3->ReadOnly = true;
				 this->numericUpDown3->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown3->TabIndex = 7;
				 this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown3_ValueChanged);
				 // 
				 // label4
				 // 
				 this->label4->AutoSize = true;
				 this->label4->Location = System::Drawing::Point(8, 80);
				 this->label4->Name = L"label4";
				 this->label4->Size = System::Drawing::Size(17, 13);
				 this->label4->TabIndex = 6;
				 this->label4->Text = L"Y:";
				 // 
				 // numericUpDown2
				 // 
				 this->numericUpDown2->DecimalPlaces = 2;
				 this->numericUpDown2->Enabled = false;
				 this->numericUpDown2->Location = System::Drawing::Point(31, 77);
				 this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown2->Name = L"numericUpDown2";
				 this->numericUpDown2->ReadOnly = true;
				 this->numericUpDown2->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown2->TabIndex = 5;
				 this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown2_ValueChanged);
				 // 
				 // label3
				 // 
				 this->label3->AutoSize = true;
				 this->label3->Location = System::Drawing::Point(8, 54);
				 this->label3->Name = L"label3";
				 this->label3->Size = System::Drawing::Size(17, 13);
				 this->label3->TabIndex = 4;
				 this->label3->Text = L"X:";
				 // 
				 // label2
				 // 
				 this->label2->AutoSize = true;
				 this->label2->Location = System::Drawing::Point(5, 28);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(21, 13);
				 this->label2->TabIndex = 3;
				 this->label2->Text = L"ID:";
				 // 
				 // numericUpDown1
				 // 
				 this->numericUpDown1->DecimalPlaces = 2;
				 this->numericUpDown1->Enabled = false;
				 this->numericUpDown1->Location = System::Drawing::Point(31, 51);
				 this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
				 this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
				 this->numericUpDown1->Name = L"numericUpDown1";
				 this->numericUpDown1->ReadOnly = true;
				 this->numericUpDown1->Size = System::Drawing::Size(132, 20);
				 this->numericUpDown1->TabIndex = 2;
				 this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
				 // 
				 // textBox1
				 // 
				 this->textBox1->Enabled = false;
				 this->textBox1->Location = System::Drawing::Point(31, 25);
				 this->textBox1->Name = L"textBox1";
				 this->textBox1->ReadOnly = true;
				 this->textBox1->Size = System::Drawing::Size(132, 20);
				 this->textBox1->TabIndex = 1;
				 this->textBox1->Text = L"no node selected";
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->Location = System::Drawing::Point(5, 6);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(33, 13);
				 this->label1->TabIndex = 0;
				 this->label1->Text = L"Node";
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
				 this->Text = L"Form1";
				 this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Panel1_MouseWheel);
				 this->toolStrip1->ResumeLayout(false);
				 this->toolStrip1->PerformLayout();
				 this->menuStrip1->ResumeLayout(false);
				 this->menuStrip1->PerformLayout();
				 this->panel2->ResumeLayout(false);
				 this->panel2->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
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

				 this->numericUpDown1->Enabled = false;
				 this->numericUpDown2->Enabled = false;
				 this->numericUpDown3->Enabled = false;
				 this->numericUpDown4->Enabled = false;
				 this->numericUpDown5->Enabled = false;

				 this->panel1->Focus();
			 }
			 void UpdateNodeGUI() {
				 SQMNode *node = OpenGL->getSelected();
				 if (node != NULL) {
					 System::String^ stringID = gcnew System::String(node->getId().c_str());
					 this->textBox1->Text = stringID;
					 this->numericUpDown1->Value = System::Decimal(node->getX());
					 this->numericUpDown2->Value = System::Decimal(node->getY());;
					 this->numericUpDown3->Value = System::Decimal(node->getZ());;
					 this->numericUpDown4->Value = System::Decimal(node->getNodeRadius());
					 this->numericUpDown5->Value = System::Decimal(node->getTessLevel());
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

					 this->numericUpDown1->Enabled = true;
					 this->numericUpDown2->Enabled = true;
					 this->numericUpDown3->Enabled = true;
					 this->numericUpDown4->Enabled = true;
					 this->numericUpDown5->Enabled = true;
				 } else {
					 this->textBox1->Text = L"no node selected";
					 this->numericUpDown1->Value = System::Decimal(0);
					 this->numericUpDown2->Value = System::Decimal(0);;
					 this->numericUpDown3->Value = System::Decimal(0);;
					 this->numericUpDown4->Value = System::Decimal(1);
					 this->numericUpDown5->Value = System::Decimal(1);
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

			 void Form1_KeyPress(Object^ sender, KeyPressEventArgs^ e)
			 {
			 }

#pragma endregion
			 void refreshCheckBoxes() {
				 quaternionToolStripMenuItem->Checked = true;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = false;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = false;
			 }
	private: System::Void newToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->newFile();
				 refreshCheckBoxes();
			 }
	private: System::Void loadToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 string fileName = "";
					 MarshalString(this->openFileDialog1->FileName, fileName);
					 //MessageBox::Show(this->openFileDialog1->FileName);
					 OpenGL->loadSkeletonFromFile(fileName);
					 refreshCheckBoxes();
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
					 OpenGL->exportSkeletonToFile(fileName);
				 }
			 }
	private: System::Void straightenToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMStraighten);
			 }
	private: System::Void computeConvexHullToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMComputeConvexHull);
			 }
	private: System::Void subdivideConvexHullToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMSubdivideConvexHull);
			 }
	private: System::Void joinBranchNodePolyhedronsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMJoinBNPs);
			 }
	private: System::Void straightenMeshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->executeSQMAlgorithm(SQMFinalPlacement);
			 }
	private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->glEventHandler->state = CameraMoveState;
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
	private: System::Void restartToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->restartSQMAlgorithm();
			 }
	private: System::Void wireframeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->alterWireframe();
			 }
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 Application::Exit();
			 }
	private: System::Void quaternionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 quaternionToolStripMenuItem->Checked = true;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = false;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = false;

				 OpenGL->setQuaternionSmoothing();
			 }
	private: System::Void valencyWeightedLaplacianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 quaternionToolStripMenuItem->Checked = false;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = true;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = false;

				 OpenGL->setValencyWeightedLaplacian();
			 }
	private: System::Void oneRingAreaWeightedLaplacianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 quaternionToolStripMenuItem->Checked = false;
				 valencyWeightedLaplacianToolStripMenuItem->Checked = false;
				 oneRingAreaWeightedLaplacianToolStripMenuItem->Checked = true;

				 OpenGL->setOneRingWeightedLaplacian();
			 }
	};
}

