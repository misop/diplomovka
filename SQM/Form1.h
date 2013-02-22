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
			OpenGL = gcnew COpenGL(this->panel1, 732, 473);
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
	private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton2;
	private: System::Windows::Forms::ToolStripMenuItem^  straightenToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  computeConvexHullToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  subdivideConvexHullToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  joinVranchNodePolyhedronsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  executeSQMAlgorithmToolStripMenuItem;
	private: System::Windows::Forms::Panel^  panel1;



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
				 this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->toolStripDropDownButton2 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
				 this->straightenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->computeConvexHullToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->subdivideConvexHullToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->joinVranchNodePolyhedronsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->executeSQMAlgorithmToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
				 this->panel1 = (gcnew System::Windows::Forms::Panel());
				 this->toolStrip1->SuspendLayout();
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
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripDropDownButton1, 
					 this->toolStripDropDownButton2});
				 this->toolStrip1->Location = System::Drawing::Point(0, 0);
				 this->toolStrip1->Name = L"toolStrip1";
				 this->toolStrip1->Size = System::Drawing::Size(744, 25);
				 this->toolStrip1->TabIndex = 0;
				 this->toolStrip1->Text = L"toolStrip1";
				 // 
				 // toolStripDropDownButton1
				 // 
				 this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
				 this->toolStripDropDownButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
					 this->saveToolStripMenuItem});
				 this->toolStripDropDownButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripDropDownButton1.Image")));
				 this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
				 this->toolStripDropDownButton1->Size = System::Drawing::Size(38, 22);
				 this->toolStripDropDownButton1->Text = L"File";
				 // 
				 // openToolStripMenuItem
				 // 
				 this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
				 this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
				 this->openToolStripMenuItem->Text = L"Open";
				 this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
				 // 
				 // saveToolStripMenuItem
				 // 
				 this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
				 this->saveToolStripMenuItem->Size = System::Drawing::Size(103, 22);
				 this->saveToolStripMenuItem->Text = L"Save";
				 this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
				 // 
				 // toolStripDropDownButton2
				 // 
				 this->toolStripDropDownButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
				 this->toolStripDropDownButton2->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->straightenToolStripMenuItem, 
					 this->computeConvexHullToolStripMenuItem, this->subdivideConvexHullToolStripMenuItem, this->joinVranchNodePolyhedronsToolStripMenuItem, 
					 this->executeSQMAlgorithmToolStripMenuItem});
				 this->toolStripDropDownButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripDropDownButton2.Image")));
				 this->toolStripDropDownButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripDropDownButton2->Name = L"toolStripDropDownButton2";
				 this->toolStripDropDownButton2->Size = System::Drawing::Size(46, 22);
				 this->toolStripDropDownButton2->Text = L"SQM";
				 // 
				 // straightenToolStripMenuItem
				 // 
				 this->straightenToolStripMenuItem->Name = L"straightenToolStripMenuItem";
				 this->straightenToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->straightenToolStripMenuItem->Text = L"Straighten";
				 this->straightenToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::straightenToolStripMenuItem_Click);
				 // 
				 // computeConvexHullToolStripMenuItem
				 // 
				 this->computeConvexHullToolStripMenuItem->Name = L"computeConvexHullToolStripMenuItem";
				 this->computeConvexHullToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->computeConvexHullToolStripMenuItem->Text = L"Compute Convex Hull";
				 this->computeConvexHullToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::computeConvexHullToolStripMenuItem_Click);
				 // 
				 // subdivideConvexHullToolStripMenuItem
				 // 
				 this->subdivideConvexHullToolStripMenuItem->Name = L"subdivideConvexHullToolStripMenuItem";
				 this->subdivideConvexHullToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->subdivideConvexHullToolStripMenuItem->Text = L"Subdivide Convex Hull";
				 this->subdivideConvexHullToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::subdivideConvexHullToolStripMenuItem_Click);
				 // 
				 // joinVranchNodePolyhedronsToolStripMenuItem
				 // 
				 this->joinVranchNodePolyhedronsToolStripMenuItem->Name = L"joinVranchNodePolyhedronsToolStripMenuItem";
				 this->joinVranchNodePolyhedronsToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->joinVranchNodePolyhedronsToolStripMenuItem->Text = L"Join Branch Node Polyhedrons";
				 this->joinVranchNodePolyhedronsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::joinVranchNodePolyhedronsToolStripMenuItem_Click);
				 // 
				 // executeSQMAlgorithmToolStripMenuItem
				 // 
				 this->executeSQMAlgorithmToolStripMenuItem->Name = L"executeSQMAlgorithmToolStripMenuItem";
				 this->executeSQMAlgorithmToolStripMenuItem->Size = System::Drawing::Size(236, 22);
				 this->executeSQMAlgorithmToolStripMenuItem->Text = L"Execute SQM Algorithm";
				 this->executeSQMAlgorithmToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::executeSQMAlgorithmToolStripMenuItem_Click);
				 // 
				 // openFileDialog1
				 // 
				 this->openFileDialog1->FileName = L"openFileDialog1";
				 // 
				 // panel1
				 // 
				 this->panel1->Location = System::Drawing::Point(0, 28);
				 this->panel1->Name = L"panel1";
				 this->panel1->Size = System::Drawing::Size(732, 473);
				 this->panel1->TabIndex = 1;
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(744, 513);
				 this->Controls->Add(this->panel1);
				 this->Controls->Add(this->toolStrip1);
				 this->Name = L"Form1";
				 this->Text = L"Form1";
				 this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseWheel);
				 this->toolStrip1->ResumeLayout(false);
				 this->toolStrip1->PerformLayout();
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


	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 string fileName = "";
					 MarshalString(this->openFileDialog1->FileName, fileName);
					 //MessageBox::Show(this->openFileDialog1->FileName);
					 OpenGL->loadSkeletonFromFile(fileName);
				 }
			 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 string fileName = "";
					 MarshalString(this->saveFileDialog1->FileName, fileName);
					 OpenGL->saveSkeletonToFile(fileName);
				 }
			 }
	private: System::Void straightenToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->straightenSkeleton();
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
	private: System::Void computeConvexHullToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenGL->computeConvexHull();
			 }
private: System::Void subdivideConvexHullToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenGL->subdivideConvexHull();
		 }
private: System::Void joinVranchNodePolyhedronsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenGL->joinBNPs();
		 }
private: System::Void executeSQMAlgorithmToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenGL->executeSQMAlgorithm();
		 }
};
}

