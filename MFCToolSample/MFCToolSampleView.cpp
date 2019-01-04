
// MFCToolSampleView.cpp: CMFCToolSampleView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCToolSample.h"
#endif

#include "MFCToolSampleDoc.h"
#include "MFCToolSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCToolSampleView

IMPLEMENT_DYNCREATE(CMFCToolSampleView, CView)

BEGIN_MESSAGE_MAP(CMFCToolSampleView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCToolSampleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMFCToolSampleView 생성/소멸

CMFCToolSampleView::CMFCToolSampleView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolSampleView::~CMFCToolSampleView()
{
}

BOOL CMFCToolSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCToolSampleView 그리기

void CMFCToolSampleView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolSampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCToolSampleView 인쇄


void CMFCToolSampleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCToolSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCToolSampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCToolSampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCToolSampleView 진단

#ifdef _DEBUG
void CMFCToolSampleView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolSampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCToolSampleDoc* CMFCToolSampleView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolSampleDoc)));
	return (CMFCToolSampleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolSampleView 메시지 처리기

void CMFCToolSampleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CMFCToolSampleApp* App = (CMFCToolSampleApp*)AfxGetApp();
	//App->m_Tool.ResizeDevice(cx, cy);
}
