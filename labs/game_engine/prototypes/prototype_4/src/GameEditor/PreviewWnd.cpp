// PreviewWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "PreviewWnd.h"
#include "afxdialogex.h"
#include "AppContext.h"

#include "Renderer.h"

// CPreviewWnd �Ի���

IMPLEMENT_DYNAMIC(CPreviewWnd, CDialogEx)

CPreviewWnd::CPreviewWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPreviewWnd::IDD, pParent)
{

}

CPreviewWnd::~CPreviewWnd()
{
}

void CPreviewWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewWnd, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPreviewWnd ��Ϣ�������


BOOL CPreviewWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	using namespace engine;

	CRect rc;

	GetClientRect(rc);


	m_pRenderTarget = AppContext::GetCoreApi()->GetSysGraphics()->CreateRenderWindow(m_hWnd, rc.Width(), rc.Height(), G_FORMAT_R8G8B8A8_UNORM, G_FORMAT_D24_UNORM_S8_UINT, 2, 1, 0, true);

	AppContext::GetCoreApi()->GetSysGraphics()->SetRenderWindow(m_pRenderTarget);

	SetTimer(99, 10, nullptr);

	return TRUE;
}


void CPreviewWnd::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	KillTimer(99);
	AppContext::GetCoreApi()->GetSysGraphics()->SetRenderTarget(engine::RenderTargetPtr());

	m_pRenderTarget->Release();
	m_pRenderTarget.reset();


	CDialogEx::OnClose();
}



void CPreviewWnd::OnTimer(UINT_PTR nIDEvent)
{
	AppContext::GetCoreApi()->GetSysGraphics()->SetRenderWindow(m_pRenderTarget);
	AppContext::GetSysGraphics()->Present();

	CDialogEx::OnTimer(nIDEvent);
}
