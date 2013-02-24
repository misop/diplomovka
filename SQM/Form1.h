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
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			OpenGL = gcnew COpenGL(this->panel1, 720, 449);
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



			 System::Windows::Forms::Timer^  timer1;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				 this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
				 this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
				 this->panel1 = (gcnew System::Windows::Forms::Panel());
				 this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				 this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->saveToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->sQMToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->straightenToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->computeConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->subdivideConvexHullToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->joinBranchNodePolyhedronsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->menuStrip1->SuspendLayout();
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
				 this->toolStrip1->Location = System::Drawing::Point(0, 24);
				 this->toolStrip1->Name = L"toolStrip1";
				 this->toolStrip1->Size = System::Drawing::Size(744, 25);
				 this->toolStrip1->TabIndex = 0;
				 this->toolStrip1->Text = L"toolStrip1";
				 // 
				 // openFileDialog1
				 // 
				 this->openFileDialog1->FileName = L"openFileDialog1";
				 // 
				 // panel1
				 // 
				 this->panel1->Location = System::Drawing::Point(12, 52);
				 this->panel1->Name = L"panel1";
				 this->panel1->Size = System::Drawing::Size(720, 449);
				 this->panel1->TabIndex = 1;
				 // 
				 // menuStrip1
				 // 
				 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
					 this->sQMToolStripMenuItem});
				 this->menuStrip1->Location = System::Drawing::Point(0, 0);
				 this->menuStrip1->Name = L"menuStrip1";
				 this->menuStrip1->Size = System::Drawing::Size(744, 24);
				 this->menuStrip1->TabIndex = 2;
				 this->menuStrip1->Text = L"menuStrip1";
				 // 
				 // fileToolStripMenuItem
				 // 
				 this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->loadToolStripMenuItem, 
					 this->saveToolStripMenuItem1});
				 this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				 this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
				 this->fileToolStripMenuItem->Text = L"File";
				 // 
				 // loadToolStripMenuItem
				 // 
				 this->loadToolStripMenuItem->Name = L"loadToolStripMenuItem";
				 this->loadToolStripMenuItem->Size = System::Drawing::Size(152, 22);
				 this->loadToolStripMenuItem->Text = L"Open";
				 this->loadToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::loadToolStripMenuItem_Click);
				 // 
				 // saveToolStripMenuItem1
				 // 
				 this->saveToolStripMenuItem1->Name = L"saveToolStripMenuItem1";
				 this->saveToolStripMenuItem1->Size = System::Drawing::Size(152, 22);
				 this->saveToolStripMenuItem1->Text = L"Save";
				 this->saveToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem1_Click);
				 // 
				 // sQMToolStripMenuItem
				 // 
				 this->sQMToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->straightenToolStripMenuItem1, 
					 this->computeConvexHullToolStripMenuItem1, this->subdivideConvexHullToolStripMenuItem1, this->joinBranchNodePolyhedronsToolStripMenuItem});
				 this->sQMToolStripMenuItem->Name = L"sQMToolStripMenuItem";
				 this->sQMToolStripMenuItem->Size = System::Drawing::Size(45, 20);
				 this->sQMToolStripMenuItem->Text = L"SQM";
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
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(744, 513);
				 this->Controls->Add(this->panel1);
				 this->Controls->Add(this->toolStrip1);
				 this->Controls->Add(this->menuStrip1);
				 this->MainMenuStrip = this->menuStrip1;
				 this->Name = L"Form1";
				 this->Text = L"Form1";
				 this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseWheel);
				 this->menuStrip1->ResumeLayout(false);
				 this->menuStrip1->PerformLayout();
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

			 void Form1_MouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ) {
			 }
			 void Form1_MouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
				 // Update the mouse path that is drawn onto the Panel. 
				 float dx = e->X - lastPosition.X;
				 float dy = e->Y - lastPosition.Y;
				 if (e->Button == ::MouseButtons::Right) {
					 OpenGL->cameraFi += dx * 0.3;
					 OpenGL->cameraTheta += dy * 0.3;
				 }
				 lastPosition.X = e->X;
				 lastPosition.Y = e->Y;
				 this->Invalidate();
			 }

			 void Form1_MouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
				 if (Control::ModifierKeys == Keys::Shift)
					 OpenGL->glCamera->setDist(OpenGL->glCamera->dist - e->Delta/20);
				 else
					 OpenGL->glCamera->setDist(OpenGL->glCamera->dist - e->Delta);

				 this->Invalidate();
			 }

			 void Form1_MouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e )
			 {
			 }

			 void Form1_KeyPress(Object^ sender, KeyPressEventArgs^ e)
			 {
			 }

#pragma endregion
	private: System::Void loadToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 string fileName = "";
					 MarshalString(this->openFileDialog1->FileName, fileName);
					 //MessageBox::Show(this->openFileDialog1->FileName);
					 OpenGL->loadSkeletonFromFile(fileName);
				 }
			 }
	private: System::Void saveToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 string fileName = "";
					 MarshalString(this->saveFileDialog1->FileName, fileName);
					 OpenGL->saveSkeletonToFile(fileName);
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
	};
}

