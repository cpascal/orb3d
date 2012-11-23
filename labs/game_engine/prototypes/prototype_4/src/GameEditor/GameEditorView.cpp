
// GameEditorView.cpp : CGameEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "GameEditor.h"
#endif

#include "GameEditorDoc.h"
#include "GameEditorView.h"


#include "AppContext.h"

#include "GridMesh.h"
#include "Renderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameEditorView

IMPLEMENT_DYNCREATE(CGameEditorView, CView)

BEGIN_MESSAGE_MAP(CGameEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CGameEditorView ����/����

CGameEditorView::CGameEditorView()
{
	// TODO: �ڴ˴���ӹ������

//	m_pGrid = NULL;

}

CGameEditorView::~CGameEditorView()
{
	
}

BOOL CGameEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGameEditorView ����

void CGameEditorView::OnDraw(CDC* /*pDC*/)
{
	CGameEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	Render();
	
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CGameEditorView::OnRButtonUp(UINT nFlags , CPoint point)
{

	RendererPtr pRenderer = AppContext::GetRenderer();

	if(pRenderer)
	{
		pRenderer->OnMouseRButtonUp(nFlags, point);
	}


	ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CGameEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGameEditorView ���

#ifdef _DEBUG
void CGameEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGameEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameEditorDoc* CGameEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameEditorDoc)));
	return (CGameEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameEditorView ��Ϣ�������


void CGameEditorView::OnInitialUpdate()
{
	AppContext::ReleaseContext();
	CRect rc;
	GetClientRect(rc);
	if(false == AppContext::InitContext(m_hWnd, rc.right - rc.left, rc.bottom - rc.top))
	{
		AfxMessageBox(_T("Failed to init engine."), MB_OK | MB_ICONERROR);
		return;
	}

	SetTimer(0, 10, NULL);
	Render();

	CView::OnInitialUpdate();
	// TODO: �ڴ����ר�ô����/����û���
}



void CGameEditorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Render();

	CView::OnTimer(nIDEvent);
}


void CGameEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	engine::Sys_GraphicsPtr pGraphics = AppContext::GetSysGraphics();
	if(pGraphics != NULL)
	{

		if(cx != 0 && cy != 0)
		{
			pGraphics->ResizeFrameBuffer(cx, cy);
			pGraphics->SetRenderTarget(engine::RenderTargetPtr(), engine::DepthStencilBufferPtr());
		}
	}

	AppContext::ResizeRTView(cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}


void CGameEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	AppContext::GetRenderer()->OnMouseMove(nFlags, point);

	CView::OnMouseMove(nFlags, point);
}


void CGameEditorView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: �ڴ����ר�ô����/����û���


}


void CGameEditorView::OnDestroy()
{
	
	AppContext::ReleaseContext();

	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
void CGameEditorView::Render()
{
	RendererPtr pRenderer = AppContext::GetRenderer();

	if(pRenderer)
	{
		pRenderer->Render();
	}
}

void CGameEditorView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ



	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CGameEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RendererPtr pRenderer = AppContext::GetRenderer();

	if(pRenderer)
	{
		pRenderer->OnMouseWheel(nFlags, zDelta, pt);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGameEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	RendererPtr pRenderer = AppContext::GetRenderer();

	if(pRenderer)
	{
		pRenderer->OnMouseRButtonDown(nFlags, point);
	}

	CView::OnRButtonDown(nFlags, point);
}
