
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "HOpenGL.h"
#endif

#include "HOpenGLDoc.h"
#include "HOpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592

IMPLEMENT_DYNCREATE(CHOpenGLView, CView)

BEGIN_MESSAGE_MAP(CHOpenGLView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHOpenGLView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


CHOpenGLView::CHOpenGLView() noexcept
{
	m_move = FALSE;
	m_camera_x = 0;
	m_camera_y = 0;
	m_viewtype = 2;
	m_spaceship_x = 0;
	m_spaceship_y = 0;
	m_spaceship_z = 0;
	m_speed = 0.1;
	m_angle_lr = 90;
	m_angle_tb = 90;
	m_check_l = FALSE;
	m_check_r = FALSE;
	m_check_t = FALSE;
	m_check_b = FALSE;
}

CHOpenGLView::~CHOpenGLView()
{
}

BOOL CHOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CHOpenGLView::OnDraw(CDC* /*pDC*/)
{
	CHOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetTimer(1, 33, NULL); //바로 draw하는 대신 33ms마다 타이머 콜
}



void CHOpenGLView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CHOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CHOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CHOpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHOpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}



#ifdef _DEBUG
void CHOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CHOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHOpenGLDoc* CHOpenGLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHOpenGLDoc)));
	return (CHOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CHOpenGLView::SetDevicePixelFormat(HDC hdc)
{
	int pixelformat;
	PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_GENERIC_FORMAT |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0, 0, 0, 0, 0, 0,
	8,
	0,
	8,
	0, 0, 0, 0,
	16,
	0,
	0,
	PFD_MAIN_PLANE,
	0,
	0, 0, 0
	};

	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
		MessageBox(LPCTSTR("SetPixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	return TRUE;
}


int CHOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hDC = GetDC()->m_hDC;
	
	if (!SetDevicePixelFormat(m_hDC))
	{
		return -1;
	}

	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);
	
	InitGL();
	return 0;
}


void CHOpenGLView::OnDestroy()
{
	CView::OnDestroy();
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hglRC);
}



void CHOpenGLView::InitGL()

{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}


void CHOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	ReSizeGLScene(cx, cy);
}

void CHOpenGLView::ReSizeGLScene(GLsizei width, GLsizei height)

{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CHOpenGLView::DrawGLScene()

{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	

	if (m_viewtype == 1) {//탑뷰
		gluLookAt(m_camera_x, m_camera_y, 70, m_camera_x, m_camera_y, 0.f, 0.f, 1.f, 0.f);

		GLdouble angleX, angleY;
		if (m_move) { //마우스로 시점 조정
			angleX = 0.5 * (m_current.x - m_anchor.x);
			angleY = 0.5 * (m_current.y - m_anchor.y);
			glRotatef(angleX, 0, 1, 0);
			glRotatef(angleY, 1, 0, 0);
		}
		
		//우주선의 위치를 조정하고 그림, 배경 행성은 위치 고정
		glPushMatrix();
		glTranslatef(m_spaceship_x, m_spaceship_y, m_spaceship_z);
		glRotatef(m_angle_lr - 90, 0, 0, 1);
		glRotatef(-(m_angle_tb - 90), 1, 0, 0);
		m_B711205.DrawSpaceship();
		glPopMatrix();

		m_B711205.DrawSphere();
		
	}
	else {//3인칭 뷰
		gluLookAt(0, - 5, 1, 0.f, 0, 0.f, 0.f, 1.f, 0.f);

		GLdouble angleZ, angleY;
		if (m_move) {
			angleZ = 0.5 * (m_current.x - m_anchor.x);
			angleY = 0.5 * (m_current.y - m_anchor.y);
			glRotatef(angleZ, 0, 0, 1);
			glRotatef(angleY, 1, 0, 0);
		}

		//우주선 위치 고정, 우주선이 움직이고 회전한 만큼 배경의 위치를 역으로 변환
		m_B711205.DrawSpaceship();

		glPushMatrix();
		glTranslatef(-m_spaceship_x, -m_spaceship_y, -m_spaceship_z);
		glRotatef(-(m_angle_lr - 90), 0, 0, 1);
		glRotatef((m_angle_tb - 90), 1, 0, 0);
		m_B711205.DrawSphere();
		glPopMatrix();
		
	}

	m_B711205.SetLight();
	
	SwapBuffers(m_hDC);
}

//마우스 콜백 함수를 이용해 시점 변환
void CHOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_move) {
		m_current = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void CHOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_move = TRUE;
	m_anchor = point;
	CView::OnLButtonDown(nFlags, point);
}


void CHOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_move = FALSE;
	CView::OnLButtonUp(nFlags, point);
}


void CHOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
		//탑뷰에서 카메라 이동
	case VK_LEFT:
		m_camera_x -= 1;
		break;
	case VK_RIGHT:
		m_camera_x += 1;
		break;
	case VK_UP:
		m_camera_y += 1;
		break;
	case VK_DOWN:
		m_camera_y -= 1;
		break;
		//카메라 모드 설정
	case '1':
		m_viewtype = 1;
		break;
	case '2':
		m_viewtype = 2;
		break;
		//우주선 속도 조정
	case VK_ADD:
		m_speed += 0.1;
		break;
	case VK_SUBTRACT:
		if (m_speed > 0.1) {
			m_speed -= 0.1;
		}
		break;
		//우주선 회전 각도 조정
	case 'A':
		m_check_l = TRUE;
		break;
	case 'D':
		m_check_r = TRUE;
		break;
	case 'W':
		m_check_t = TRUE;
		break;
	case 'S':
		m_check_b = TRUE;
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CHOpenGLView::OnTimer(UINT_PTR nIDEvent)
{
	//wasd키 입력에 따른 우주선 각도 변환
	//키 입력 횟수와 실제 프레임에 적용되는 횟수를 동기화시키기 위해 OnTimer에서 조정해줌
	if (m_check_l == TRUE) {
		m_angle_lr += 1;
		m_check_l = FALSE;
	}
	if (m_check_r == TRUE) {
		m_angle_lr -= 1;
		m_check_r = FALSE;
	}
	if (m_check_t == TRUE) {
		m_angle_tb -= 1;
		m_check_t = FALSE;
	}
	if (m_check_b == TRUE) {
		m_angle_tb += 1;
		m_check_b = FALSE;
	}

	//우주선이 움직인 각도만큼 구면좌표계에서 직교좌표계로 변환, 반지름은 프레임당 이동한 거리(속도)
	m_spaceship_x += m_speed * sin(m_angle_tb * PI / 180) * cos(m_angle_lr * PI / 180);
	m_spaceship_y += m_speed * sin(m_angle_tb * PI / 180) * sin(m_angle_lr * PI / 180);
	m_spaceship_z += m_speed * cos(m_angle_tb * PI / 180);

	DrawGLScene();
	CView::OnTimer(nIDEvent);
}
