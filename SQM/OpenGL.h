#pragma once

#include <windows.h>
#include <windowsx.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL\glew.h>
#include <GL\wglew.h>
#include <string>
#include "SQMControler.h"
#include "GLCamera.h"
#include "GLEventHandler.h"
#include "ShaderUtility.h"
#include "GLProgram.h"
#include "GLShader.h"
#include "GLArrayBuffer.h"
#include "Utility.h"

using namespace std;
using namespace System::Windows::Forms;

#define GL_MAJOR 4
#define GL_MINOR 1

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
		OpenGLPrograms *programs;
		OpenGLShaders *sklTessShaders;
		OpenGLShaders *sklLineShaders;
		OpenGLShaders *bnpShaders;
		OpenGLShaders *triTessShaders;
		OpenGLShaders *quadTessShaders;
		GLArrayBuffer *arrayBuffer;
		bool shouldRender;
#pragma endregion

#pragma region Inits
		COpenGL(System::Windows::Forms::Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight)
		{
			sqmControler = new SQMControler();
			glCamera = new GLCamera();
			glEventHandler = new GLEventHandler(glCamera, sqmControler);
			programs = new OpenGLPrograms();
			sklTessShaders = new OpenGLShaders();
			sklLineShaders = new OpenGLShaders();
			bnpShaders = new OpenGLShaders();
			triTessShaders = new OpenGLShaders();
			quadTessShaders = new OpenGLShaders();
			shouldRender = true;

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
			sqmControler->createIcosahedron();
			sqmControler->drawSkeleton();
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
			if (!shouldRender) return;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer

			//glPushMatrix();

			/*float x = 0 + sin(M_PI / 180 * cameraTheta) * cos(M_PI / 180 * cameraFi) * cameraDist;
			float y = 0 + cos(M_PI / 180 * cameraTheta) * cameraDist;
			float z = 0 + sin(M_PI / 180 * cameraTheta) * sin(M_PI / 180 * cameraFi) * cameraDist;*/
			/*float x = cx + sin(M_PI / 180 * cameraTheta) * sin(M_PI / 180 * cameraFi) * cameraDist;
			float y = cy + cos(M_PI / 180 * cameraTheta) * cameraDist;
			float z = cz + sin(M_PI / 180 * cameraTheta) * cos(M_PI / 180 * cameraFi) * cameraDist;*/

			//glCamera->lookFromCamera();

			//sqmControler->draw();

			//glPopMatrix();

			/*glBindVertexArray(m_vao[0]);          // select first VAO
			glDrawArrays(GL_TRIANGLES, 0, 3);       // draw first object

			glBindVertexArray(m_vao[1]);          // select second VAO
			glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
			glDrawArrays(GL_TRIANGLES, 0, 3);       // draw second object
			*/
			//arrayBuffer->Draw(GL_TRIANGLES);
			//arrayBuffer->Draw(GL_POINTS);

			sqmControler->draw(programs, glCamera);

			glBindVertexArray(0);
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
			delete sklTessShaders;
			delete sklLineShaders;
			delete bnpShaders;
			delete triTessShaders;
			delete quadTessShaders;
			delete programs;
			delete arrayBuffer;
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
			HGLRC tempContext;
			if((tempContext = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if((wglMakeCurrent(m_hDC, tempContext)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}
			//OpenGL version setting
			GLenum err = glewInit();
			if (err != GLEW_OK) {
				MessageBox::Show("GLEW Init Failed");
				return 0;
			}
			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, GL_MAJOR,
				WGL_CONTEXT_MINOR_VERSION_ARB, GL_MINOR,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			if(wglewIsSupported("WGL_ARB_create_context") == 1) {
				m_hglrc = wglCreateContextAttribsARB(m_hDC,0, attribs);
				wglMakeCurrent(NULL,NULL);
				wglDeleteContext(tempContext);
				wglMakeCurrent(m_hDC, m_hglrc);
			} else {       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
				m_hglrc = tempContext;
			}
			int OpenGLVersion[2];
			glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
			glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

			if (!m_hglrc) return 0;

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
			glEnable(GL_PROGRAM_POINT_SIZE);
			InitShaders();
			SetData();
			return true;										// Initialisation went ok
		}
	public:
		void ReloadShaders() {
			shouldRender = false;

			delete sklTessShaders;
			delete sklLineShaders;
			delete bnpShaders;
			delete triTessShaders;
			delete quadTessShaders;
			delete programs;

			programs = new OpenGLPrograms();
			sklTessShaders = new OpenGLShaders();
			sklLineShaders = new OpenGLShaders();
			bnpShaders = new OpenGLShaders();
			triTessShaders = new OpenGLShaders();
			quadTessShaders = new OpenGLShaders();

			InitShaders();

			shouldRender = true;
		}
	protected:
		void ReLoadTessShaders() {
			shouldRender = false;

			delete triTessShaders;
			delete quadTessShaders;
			delete programs->TriMeshTess;
			delete programs->QuadMeshTess;
			
			triTessShaders = new OpenGLShaders();
			quadTessShaders = new OpenGLShaders();

			InitTessShaders();

			shouldRender = true;
		}
		void InitTessShaders() {
			std::string key = "_valency";
			std::string value = ToStringT(sqmControler->getMaxValency());

			std::map<std::string, std::string> replaceMap;
			replaceMap.insert(std::pair<std::string, std::string>(key, value));

			//tri mesh tesselation
			triTessShaders->vert = new GLShader(GL_VERTEX_SHADER);
			triTessShaders->vert->Load("Shaders/TessVertShader.vert", replaceMap);
			triTessShaders->vert->Compile();

			triTessShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
			triTessShaders->ctrl->Load("Shaders/TessTriCtrlShader.glsl", replaceMap);
			triTessShaders->ctrl->Compile();

			triTessShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
			triTessShaders->eval->Load("Shaders/TessTriEvalShader.glsl", replaceMap);
			triTessShaders->eval->Compile();

			triTessShaders->geom = new GLShader(GL_GEOMETRY_SHADER);
			triTessShaders->geom->Load("Shaders/TessGeomShader.geom", replaceMap);
			triTessShaders->geom->Compile();

			triTessShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
			triTessShaders->frag->Load("Shaders/TessFragShader.frag", replaceMap);
			triTessShaders->frag->Compile();

			programs->TriMeshTess = new GLProgram("TriMeshTess");
			programs->TriMeshTess->AttachShader(triTessShaders->vert);
			programs->TriMeshTess->AttachShader(triTessShaders->ctrl);
			programs->TriMeshTess->AttachShader(triTessShaders->eval);
			programs->TriMeshTess->AttachShader(triTessShaders->geom);
			programs->TriMeshTess->AttachShader(triTessShaders->frag);
			programs->TriMeshTess->Link();
			programs->TriMeshTess->SaveProgramLog();
			//quad mesh tesselation
			quadTessShaders->vert = new GLShader(GL_VERTEX_SHADER);
			quadTessShaders->vert->Load("Shaders/TessVertShader.vert", replaceMap);
			quadTessShaders->vert->Compile();

			quadTessShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
			quadTessShaders->ctrl->Load("Shaders/TessQuadCtrlShader.glsl", replaceMap);
			quadTessShaders->ctrl->Compile();

			quadTessShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
			quadTessShaders->eval->Load("Shaders/TessQuadEvalShader.glsl", replaceMap);
			quadTessShaders->eval->Compile();

			quadTessShaders->geom = new GLShader(GL_GEOMETRY_SHADER);
			quadTessShaders->geom->Load("Shaders/TessGeomShader.geom", replaceMap);
			quadTessShaders->geom->Compile();

			quadTessShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
			quadTessShaders->frag->Load("Shaders/TessFragShader.frag", replaceMap);
			quadTessShaders->frag->Compile();

			programs->QuadMeshTess = new GLProgram("QuadMeshTess");
			programs->QuadMeshTess->AttachShader(quadTessShaders->vert);
			programs->QuadMeshTess->AttachShader(quadTessShaders->ctrl);
			programs->QuadMeshTess->AttachShader(quadTessShaders->eval);
			programs->QuadMeshTess->AttachShader(quadTessShaders->geom);
			programs->QuadMeshTess->AttachShader(quadTessShaders->frag);
			programs->QuadMeshTess->Link();
			programs->QuadMeshTess->SaveProgramLog();
		}
		bool InitShaders() {
			//skeleton node drawing
			sklTessShaders->vert = new GLShader(GL_VERTEX_SHADER);
			sklTessShaders->vert->Load("Shaders/SklTessVertShader.vert");
			sklTessShaders->vert->Compile();

			sklTessShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
			sklTessShaders->ctrl->Load("Shaders/SklTessCtrlShader.glsl");
			sklTessShaders->ctrl->Compile();

			sklTessShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
			sklTessShaders->eval->Load("Shaders/SklTessEvalShader.glsl");
			sklTessShaders->eval->Compile();

			sklTessShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
			sklTessShaders->frag->Load("Shaders/SklTessFragShader.frag");
			sklTessShaders->frag->Compile();

			programs->SklNodes = new GLProgram("SklNodes");
			programs->SklNodes->AttachShader(sklTessShaders->vert);
			programs->SklNodes->AttachShader(sklTessShaders->ctrl);
			programs->SklNodes->AttachShader(sklTessShaders->eval);
			programs->SklNodes->AttachShader(sklTessShaders->frag);
			programs->SklNodes->Link();
			programs->SklNodes->SaveProgramLog();
			//skeleton line drawing
			sklLineShaders->vert = new GLShader(GL_VERTEX_SHADER);
			sklLineShaders->vert->Load("Shaders/SklLineVertShader.vert");
			sklLineShaders->vert->Compile();

			sklLineShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
			sklLineShaders->frag->Load("Shaders/SklLineFragShader.frag");
			sklLineShaders->frag->Compile();

			programs->SklLines = new GLProgram("SklLines");
			programs->SklLines->AttachShader(sklLineShaders->vert);
			programs->SklLines->AttachShader(sklLineShaders->frag);
			programs->SklLines->Link();
			programs->SklLines->SaveProgramLog();
			//BNP drawing
			bnpShaders->vert = new GLShader(GL_VERTEX_SHADER);
			bnpShaders->vert->Load("Shaders/BNPVertShader.vert");
			bnpShaders->vert->Compile();

			bnpShaders->geom = new GLShader(GL_GEOMETRY_SHADER);
			bnpShaders->geom->Load("Shaders/BNPGeomShader.geom");
			bnpShaders->geom->Compile();

			bnpShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
			bnpShaders->frag->Load("Shaders/BNPFragShader.frag");
			bnpShaders->frag->Compile();

			programs->BNPs = new GLProgram("BNPs");
			programs->BNPs->AttachShader(bnpShaders->vert);
			programs->BNPs->AttachShader(bnpShaders->geom);
			programs->BNPs->AttachShader(bnpShaders->frag);
			programs->BNPs->Link();
			programs->BNPs->SaveProgramLog();

			InitTessShaders();

			return true;
		}
		void SetData() {
			arrayBuffer = new GLArrayBuffer();
			/*std::vector<float> vert(9);
			std::vector<float> col(9);

			vert[0] =-0.3; vert[1] = 0.5; vert[2] =-1.0;
			vert[3] =-0.8; vert[4] =-0.5; vert[5] =-1.0;
			vert[6] = 0.2; vert[7] =-0.5; vert[8]= -1.0;

			col[0] = 1.0; col[1] = 0.0; col[2] = 0.0;
			col[3] = 0.0; col[4] = 1.0; col[5] = 0.0;
			col[6] = 0.0; col[7] = 0.0; col[8] = 1.0;*/
			std::vector<float> vert;
			std::vector<float> col;

			arrayBuffer->Bind();
			arrayBuffer->BindBufferData(vert, 3, GL_STATIC_DRAW);
			arrayBuffer->BindBufferData(col, 3, GL_STATIC_DRAW);

			/*delete [] m_vao;
			delete [] m_vbo;
			m_vao = new GLuint[2];
			m_vbo = new GLuint[3];
			// First simple object
			float* vert = new float[9];     // vertex array
			float* col  = new float[9];     // color array

			vert[0] =-0.3; vert[1] = 0.5; vert[2] =-1.0;
			vert[3] =-0.8; vert[4] =-0.5; vert[5] =-1.0;
			vert[6] = 0.2; vert[7] =-0.5; vert[8]= -1.0;

			col[0] = 1.0; col[1] = 0.0; col[2] = 0.0;
			col[3] = 0.0; col[4] = 1.0; col[5] = 0.0;
			col[6] = 0.0; col[7] = 0.0; col[8] = 1.0;

			// Second simple object
			float* vert2 = new float[9];    // vertex array

			vert2[0] =-0.2; vert2[1] = 0.5; vert2[2] =-1.0;
			vert2[3] = 0.3; vert2[4] =-0.5; vert2[5] =-1.0;
			vert2[6] = 0.8; vert2[7] = 0.5; vert2[8]= -1.0;

			// Two VAOs allocation
			glGenVertexArrays(2, m_vao);

			// First VAO setup
			glBindVertexArray(m_vao[0]);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glGenBuffers(2, m_vbo);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), col, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Second VAO setup     
			glBindVertexArray(m_vao[1]);

			glGenBuffers(1, &m_vbo[2]);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert2, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);

			delete [] vert;
			delete [] vert2;
			delete [] col;*/
		}

	public:
		SQMControler* getSQMController() {
			return sqmControler;
		}

		GLvoid resize(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (width == 0) {
				width = 1;
			}
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
			glCamera->projection = glm::perspective<float>(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, INFINITE);
			glCamera->fovy = 45.0;
			glCamera->aspect = (float)width/(float)height;
			glCamera->setWidth(width);
			glCamera->setHeight(height);

			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glLoadIdentity();									// Reset The Modelview Matrix
		}

#pragma region My Functions

		void setupView() {//NOT IN USE
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
		//Getters
		SQMNode* getSelected() {
			return sqmControler->getSelected();
		}
		//setters
		void setSmoothingOff() {
			sqmControler->setSmoothingAlgorithm(SQMNoSmoothing);
		}
		void setAvaragingSmoothing() {
			sqmControler->setSmoothingAlgorithm(SQMAvaragingSmoothing);
		}
		void setQuaternionSmoothing() {
			sqmControler->setSmoothingAlgorithm(SQMQuaternionSmoothing);
		}
		void setOneRingWeightedLaplacian() {
			sqmControler->setSmoothingAlgorithm(SQMOneRingLaplacianSmoothing);
		}
		void setValencyWeightedLaplacian() {
			sqmControler->setSmoothingAlgorithm(SQMValencyLaplacianSmoothing);
		}
		void setGlobalTesselation(float globalTesselation) {
			sqmControler->setGlobalTesselation(globalTesselation);
		}
		//functions
		void alterWireframe() {
			sqmControler->setWireframe(!sqmControler->getWireframe());
		}
		void alterDrawNormals() {
			sqmControler->setShouldDrawNormals(!sqmControler->getShouldDrawNormals());
		}
		void newFile() {
			sqmControler->newFile();
			glCamera->reset();
		}
		void loadSkeletonFromFile(string fileName) {
			sqmControler->loadSkeletonFromFile(fileName);
			glCamera->reset();
			//setupView();
		}
		void saveSkeletonToFile(string fileName) {
			sqmControler->saveSkeletonToFile(fileName);
		}
		void exportSkeletonToFile(string fileName) {
			sqmControler->exportSkeletonToFile(fileName);
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
			ReLoadTessShaders();
		}
		void executeSQMAlgorithm() {
			sqmControler->executeSQMAlgorithm();
		}
		void restartSQMAlgorithm() {
			sqmControler->restart();
		}
		void executeSQMAlgorithm(SQMState state) {
			sqmControler->executeSQMAlgorithm(state);
		}
	};

	//Custom methods
}