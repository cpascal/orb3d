
#include "stdafx.h"
#include "MainFrm.h"
#include "ObjectView.h"
#include "Resource.h"
#include "GameEditor.h"
#include "AppContext.h"

class CObjectViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CObjectView;

	DECLARE_SERIAL(CObjectViewMenuButton)

public:
	CObjectViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CObjectViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

CObjectView::CObjectView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CObjectView::~CObjectView()
{
}

BEGIN_MESSAGE_MAP(CObjectView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectView ��Ϣ��������

int CObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ���������ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ����ͼ��:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CObjectViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CObjectViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CObjectViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	//FillObjectView();

	return 0;
}

void CObjectView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CObjectView::FillObjectView()
{
	using namespace engine;

	GameObjectPtr pRoot = AppContext::GetCoreApi()->GetRoot();

	HTREEITEM hRoot = m_wndObjectView.InsertItem(pRoot->GetName().c_str(), 5, 5);
	
	_fill_tree(pRoot, hRoot);

	m_wndObjectView.Expand(hRoot, TVE_EXPAND);
}

void CObjectView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndObjectView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CObjectView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndObjectView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CObjectView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CObjectView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CObjectViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CObjectViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CObjectView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CObjectView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("���ӳ�Ա����..."));
}

void CObjectView::OnClassAddMemberVariable()
{
	// TODO: �ڴ˴�����������������
}

void CObjectView::OnClassDefinition()
{
	// TODO: �ڴ˴�����������������
}

void CObjectView::OnClassProperties()
{
	// TODO: �ڴ˴�����������������
}

void CObjectView::OnNewFolder()
{
	AfxMessageBox(_T("�½��ļ���..."));
}

void CObjectView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndObjectView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CObjectView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndObjectView.SetFocus();
}

void CObjectView::OnChangeVisualStyle()
{
	m_ObjectViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ObjectViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ObjectViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndObjectView.SetImageList(&m_ObjectViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ����*/);
}
void CObjectView::_fill_tree(engine::GameObjectPtr pRoot, HTREEITEM hRoot)
{
	if(pRoot == NULL)
	{
		return;
	}
	
	using namespace engine;
	GameObjectPtr pObj = pRoot->GetFirstChild();

	while(pObj)
	{
		HTREEITEM hObj = m_wndObjectView.InsertItem(pObj->GetName().c_str(), 5, 5, hRoot);

		_fill_tree(pObj, hObj);
		pObj = pObj->GetNextNode();
	}

	/*HTREEITEM hRoot = m_wndObjectView.InsertItem(_T("FakeApp ��"), 5, 5);
	m_wndObjectView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndObjectView.InsertItem(_T("CFakeAboutDlg"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("CFakeAboutDlg()"), 5, 5, hClass);

	m_wndObjectView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndObjectView.InsertItem(_T("CFakeApp"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("CFakeApp()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("InitInstance()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("OnAppAbout()"), 5, 5, hClass);

	hClass = m_wndObjectView.InsertItem(_T("CFakeAppDoc"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("CFakeAppDoc()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("~CFakeAppDoc()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("OnNewDocument()"), 5, 5, hClass);

	hClass = m_wndObjectView.InsertItem(_T("CFakeAppView"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("CFakeAppView()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("~CFakeAppView()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("GetDocument()"), 5, 5, hClass);
	m_wndObjectView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndObjectView.InsertItem(_T("CFakeAppFrame"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("CFakeAppFrame()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("~CFakeAppFrame()"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("m_wndMenuBar"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("m_wndToolBar"), 5, 5, hClass);
	m_wndObjectView.InsertItem(_T("m_wndStatusBar"), 5, 5, hClass);

	hClass = m_wndObjectView.InsertItem(_T("Globals"), 5, 5, hRoot);
	m_wndObjectView.InsertItem(_T("theFakeApp"), 5, 5, hClass);

	
	m_wndObjectView.Expand(hClass, TVE_EXPAND);*/
}