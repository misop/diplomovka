#pragma once

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SQMControler.h"
#include <string>
#include "GLCamera.h"
#include "GLEventHandler.h"

using namespace std;
using namespace System::Windows::Forms;

namespace OpenGLForm 
{
	public ref class COpenGL: public System::Windows::Forms::NativeWindow
	{
	private:
		SQMControler *sqmControler;
	public:
#pragma region Custom Variables
		float cameraFi;
		float cameraTheta;
		float cameraDist;
		System::Drawing::Point lastPositionRightMouse;
		System::Drawing::Point lastPositionLeftMouse;
		GLCamera *glCamera;
		GLEventHandler *glEventHandler;
#pragma endregion
		
#pragma region Inits
		COpenGL(System::Windows::Forms::Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight)
		{
			sqmControler = new SQMControler();
			glCamera = new GLCamera();
			glEventHandler = new GLEventHandler(glCamera, sqmControler);

			/*CreateParams^ cp = gcnew CreateParams;
			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentPanel->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());*/
			m_hDC = GetDC((HWND)parentPanel->Handle.ToPointer());

			if(m_hDC)
			{
				MySetPixelFormat(m_hDC);
				resize(iWidth, iHeight);
				InitGL();
			}

			rtri = 0.0f;
			rquad = 0.0f;
		}
		
		COpenGL(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight)
		{
			sqmControler = new SQMControler();
			glCamera = new GLCamera();
			glEventHandler = new GLEventHandler(glCamera, sqmControler);

			CreateParams^ cp = gcnew CreateParams;

			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if(m_hDC)
			{
				MySetPixelFormat(m_hDC);
				resize(iWidth, iHeight);
				InitGL();
			}

			rtri = 0.0f;
			rquad = 0.0f;
		}
#pragma endregion

		System::Void Render(System::Void)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
			
			glPushMatrix();

			/*float x = 0 + sin(M_PI / 180 * cameraTheta) * cos(M_PI / 180 * cameraFi) * cameraDist;
			float y = 0 + cos(M_PI / 180 * cameraTheta) * cameraDist;
			float z = 0 + sin(M_PI / 180 * cameraTheta) * sin(M_PI / 180 * cameraFi) * cameraDist;*/
			/*float x = cx + sin(M_PI / 180 * cameraTheta) * sin(M_PI / 180 * cameraFi) * cameraDist;
			float y = cy + cos(M_PI / 180 * cameraTheta) * cameraDist;
			float z = cz + sin(M_PI / 180 * cameraTheta) * cos(M_PI / 180 * cameraFi) * cameraDist;*/

			glCamera->lookFromCamera();

			sqmControler->draw();

			glPopMatrix();
		}

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC) ;
		}

	private:
		HDC m_hDC;
		HGLRC m_hglrc;
		GLfloat	rtri;				// Angle for the triangle
		GLfloat	rquad;				// Angle for the quad

	protected:
		~COpenGL(System::Void)
		{
			this->DestroyHandle();
			delete sqmControler;
			delete glCamera;
			delete glEventHandler;
		}

		GLint MySetPixelFormat(HDC hdc)
		{
			static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
			{
				sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
				1,											// Version Number
				PFD_DRAW_TO_WINDOW |						// Format Must Support Window
				PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,							// Must Support Double Buffering
				PFD_TYPE_RGBA,								// Request An RGBA Format
				16,										// Select Our Color Depth
				0, 0, 0, 0, 0, 0,							// Color Bits Ignored
				0,											// No Alpha Buffer
				0,											// Shift Bit Ignored
				0,											// No Accumulation Buffer
				0, 0, 0, 0,									// Accumulation Bits Ignored
				16,											// 16Bit Z-Buffer (Depth Buffer)  
				0,											// No Stencil Buffer
				0,											// No Auxiliary Buffer
				PFD_MAIN_PLANE,								// Main Drawing Layer
				0,											// Reserved
				0, 0, 0										// Layer Masks Ignored
			};

			GLint  iPixelFormat; 

			// get the device context's best, available pixel format match 
			if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}

			// make that match the device context's current pixel format 
			if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
		}

		bool InitGL(GLvoid)										// All setup for opengl goes here
		{
			glShadeModel(GL_SMOOTH);							// Enable smooth shading
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
			glClearDepth(1.0f);									// Depth buffer setup
			glEnable(GL_DEPTH_TEST);							// Enables depth testing
			glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
			return TRUE;										// Initialisation went ok
		}

		GLvoid resize(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (height==0)										// Prevent A Divide By Zero By
			{
				height=1;										// Making Height Equal One
			}

			glViewport(0,0,width,height);						// Reset The Current Viewport

			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glLoadIdentity();									// Reset The Projection Matrix

			// Calculate The Aspect Ratio Of The Window
			//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
			gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1.0f, INFINITE);
			glCamera->fovy = 45.0;
			glCamera->aspect = (float)width/(float)height;
			glCamera->width = width;
			glCamera->height = height;

			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glLoadIdentity();									// Reset The Modelview Matrix
		}

#pragma region My Functions

		void setupView() {
			float x = 0, y = 0, z = 0, d = 0;
			sqmControler->getBoundingSphere(x, y, z, d);
			if (d != 0) {
				GLdouble zNear = 1.0;
				GLdouble zFar = zNear + d;
				GLdouble left = x - d;
				GLdouble right = x + d;
				GLdouble bottom = y - d;
				GLdouble top = y + d;
				/*TODO recalculate aspect if needed
				GLdouble aspect = 640.0/480.0;//TODO get aspect from opengl
				if (aspect < 1.0) {//window taller than wide
				bottom /= aspect;
				top /= aspect;
				} else {
				left *= aspect;
				right *= aspect;
				}*/
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(left, right, bottom, top, zNear, zFar);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
			}
		}

#pragma endregion

#pragma region Handling Events
	protected:
		virtual void WndProc(Message% m)override {
			static bool rmousedown = false;
			static bool lmousedown = false;
#pragma region Right Mouse
			if (m.Msg == WM_RBUTTONDOWN) {
				int x = GET_X_LPARAM((int)m.LParam);
				int y = GET_Y_LPARAM((int)m.LParam);
				lastPositionRightMouse.X = x;
				lastPositionRightMouse.Y = y;
				SetCapture((HWND)this->Handle.ToPointer());
				rmousedown = true;
			}
			if (m.Msg == WM_RBUTTONUP) {
				ReleaseCapture();
				rmousedown = false;
			}
			if (rmousedown && m.Msg == WM_MOUSEMOVE) {
				int x = GET_X_LPARAM((int)m.LParam);
				int y = GET_Y_LPARAM((int)m.LParam);
				bool rightButtonDown = (int)m.WParam & MK_RBUTTON;
				if (rightButtonDown) {
					float dx = x - lastPositionRightMouse.X;
					float dy = y - lastPositionRightMouse.Y;
					glCamera->setFi(glCamera->fi - dx*0.3);
					glCamera->setTheta(glCamera->theta - dy*0.3);
				}
				lastPositionRightMouse.X = x;
				lastPositionRightMouse.Y = y;
			}
#pragma endregion
#pragma region Left Mouse
			if (m.Msg == WM_LBUTTONDOWN) {
				int x = GET_X_LPARAM((int)m.LParam);
				int y = GET_Y_LPARAM((int)m.LParam);
				lastPositionLeftMouse.X = x;
				lastPositionLeftMouse.Y = y;
				SetCapture((HWND)this->Handle.ToPointer());
				lmousedown = true;
			}
			if (m.Msg == WM_LBUTTONUP) {
				ReleaseCapture();
				lmousedown = false;
			}
			if (lmousedown && m.Msg == WM_MOUSEMOVE) {
				int x = GET_X_LPARAM((int)m.LParam);
				int y = GET_Y_LPARAM((int)m.LParam);
				bool leftButtonDown = (int)m.WParam & MK_LBUTTON;
				if (leftButtonDown) {
					float dx = x - lastPositionLeftMouse.X;
					float dy = y - lastPositionLeftMouse.Y;
					glCamera->strafeHorizontal(dx*0.3);
					glCamera->strafeVertical(dy*0.3);
				}
				lastPositionLeftMouse.X = x;
				lastPositionLeftMouse.Y = y;
			}
#pragma endregion
			NativeWindow::WndProc(m);
		}
#pragma endregion

	public:
		void loadSkeletonFromFile(string fileName) {
			sqmControler->loadSkeletonFromFile(fileName);
			glCamera->reset();
			//setupView();
		}
		void saveSkeletonToFile(string fileName) {
			sqmControler->saveSkeletonToFile(fileName);
		}
		void straightenSkeleton() {
			sqmControler->straightenSkeleton();
			//setupView();
		}
		void computeConvexHull() {
			sqmControler->computeConvexHull();
			//setupView();
		}
		void subdivideConvexHull() {
			sqmControler->subdivideConvexHull();
		}
		void joinBNPs() {
			sqmControler->joinBNPs();
		}
		void executeSQMAlgorithm() {
			sqmControler->executeSQMAlgorithm();
		}
		void executeSQMAlgorithm(SQMState state) {
			sqmControler->executeSQMAlgorithm(state);
		}
	};

	//Custom methods
}