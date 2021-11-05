
// HOpenGLView.cpp: CHOpenGLView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HOpenGL.h"
#endif

#include "HOpenGLDoc.h"
#include "HOpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHOpenGLView

IMPLEMENT_DYNCREATE(CHOpenGLView, CView)

BEGIN_MESSAGE_MAP(CHOpenGLView, CView)
	// 표준 인쇄 명령입니다.
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
END_MESSAGE_MAP()

// CHOpenGLView 생성/소멸

CHOpenGLView::CHOpenGLView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_move = FALSE;
	m_camera_x = 0;
	m_camera_y = 0;
	m_viewtype = 1;

}

CHOpenGLView::~CHOpenGLView()
{
}

BOOL CHOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CHOpenGLView 그리기

void CHOpenGLView::OnDraw(CDC* /*pDC*/)
{
	CHOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawGLScene();
	Invalidate(FALSE);
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CHOpenGLView 인쇄


void CHOpenGLView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CHOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CHOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CHOpenGLView 진단

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
// CHOpenGLView 메시지 처리기


int CHOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	if (!SetDevicePixelFormat(m_hDC))
	{
		return -1;
	}//

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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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


	if (m_viewtype == 1) {
		gluLookAt(m_camera_x, m_camera_y, 100, m_camera_x, m_camera_y, 0.f, 0.f, 1.f, 0.f);

		GLdouble angleX, angleY;
		if (m_move) {
			angleX = 1 * (m_current.x - m_anchor.x);
			angleY = 1 * (m_current.y - m_anchor.y);
			glRotatef(angleX, 0, 1, 0);
			glRotatef(angleY, 1, 0, 0);
		}
	}
	else {
		gluLookAt(0, -5, 1, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

		GLdouble angleX, angleY;
		if (m_move) {
			angleX = 1 * (m_current.x - m_anchor.x);
			angleY = 1 * (m_current.y - m_anchor.y);
			glRotatef(angleX, 0, 1, 0);
			glRotatef(angleY, 1, 0, 0);
		}
	}
	
	

	m_B70011205.DrawSpaceship();
	m_B70011205.DrawSphere();
	//m_B70011205.DrawXYZ();
	
	m_B70011205.SetLight(); //과제 6.광원 설정하기
	//m_B70011205.SetSpotLight(); //과제 7.Spot light 사용하
	//m_B70011205.DrawCube();
	


	
	SwapBuffers(m_hDC);

}

void CHOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_move) {
		m_current = point;
		//DrawGLScene();
	}
	CView::OnMouseMove(nFlags, point);
}


void CHOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_move = TRUE;
	m_anchor = point;
	CView::OnLButtonDown(nFlags, point);
}


void CHOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_move = FALSE;
	//DrawGLScene();
	CView::OnLButtonUp(nFlags, point);
}


void CHOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar) {
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
	case '1':
		m_viewtype = 1;
		break;
	case '2':
		m_viewtype = 2;
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
