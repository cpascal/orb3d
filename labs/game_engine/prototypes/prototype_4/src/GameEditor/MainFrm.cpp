
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "GameEditor.h"

#include "AppSettingsDlg.h"

#include "MainFrm.h"
#include "Renderer.h"


#include "AppContext.h"
#include "MeshImporter_3DSMax.h"
#include "Project.h"

#include "RenderingSettingDlg.h"

#include "PreviewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

	const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_COMPONENT_MENU_BASE, ID_COMPONENT_MENU_BASE + 1000, &CMainFrame::OnComponentMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COMPONENT_MENU_BASE, ID_COMPONENT_MENU_BASE + 1000, &CMainFrame::OnUpdateComponentMenuUI)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_CREATEFROMTEMPLATE_CUBE, &CMainFrame::OnCreatefromtemplateCube)
	ON_COMMAND(ID_GAMEOBJECT_CREATEEMPTY, &CMainFrame::OnGameobjectCreateempty)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_SPHERE, &CMainFrame::OnCreatefromtemplateSphere)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_DIRLIGHT, &CMainFrame::OnCreatefromtemplateDirlight)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_POINTLIGHT, &CMainFrame::OnCreatefromtemplatePointlight)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_SPOTLIGHT, &CMainFrame::OnCreatefromtemplateSpotlight)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_PLANE, &CMainFrame::OnCreatefromtemplatePlane)
	ON_COMMAND(ID_VIEW_SHOWGRID, &CMainFrame::OnViewShowgrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGRID, &CMainFrame::OnUpdateViewShowgrid)
	ON_COMMAND(ID_EDIT_APPSETTINGS, &CMainFrame::OnEditAppsettings)
	//	ON_COMMAND(ID_PROJECT_IMPORT, &CMainFrame::OnProjectImport)
	ON_COMMAND(ID_IMPORT_IMPORTMAXMESH, &CMainFrame::OnImportImportmaxmesh)
	ON_UPDATE_COMMAND_UI(ID_CREATEFROMTEMPLATE_CUBE, &CMainFrame::OnUpdateCreatefromtemplateCube)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_CONE, &CMainFrame::OnCreatefromtemplateCone)
	ON_COMMAND(ID_CREATEFROMTEMPLATE_SKYLIGHT, &CMainFrame::OnCreatefromtemplateSkylight)
	ON_COMMAND(ID_RENDERING_SETTING, &CMainFrame::OnRenderingSetting)
	ON_COMMAND(ID_BUILD_PREVIEW, &CMainFrame::OnBuildPreview)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneWidth(1, 60);

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ����������:
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}
	m_wndTplView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndObjectView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndObjectView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	m_wndTplView.AttachToTabWnd(&m_wndProperties, DM_SHOW, TRUE, &pTabbedBar);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	/*CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);*/

	//	lstBasicCommands.AddTail(ID_GAMEOBJECT_CREATEEMPTY);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	UpdateComClassMap();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndObjectView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_OBJECTVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}


	// ����ģ����ͼ
	CString strTplView = L"ģ�崰��";

	ASSERT(bNameValid);
	if (!m_wndTplView.Create(strTplView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TPLVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����ģ����ͼ������\n");
		return FALSE; // δ�ܴ���
	}


	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	// �������Դ���
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������ԡ�����\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hObjectViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndObjectView.SetIcon(hObjectViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hTemplateViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTplView.SetIcon(hTemplateViewIcon, FALSE);

}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	AppContext::UpdatePropGrid(engine::GameObjectPtr());


	ProjectPtr pProject = AppContext::GetProject();
	if(pProject)
	{
		pProject->Close();
		AppContext::SetProject(ProjectPtr());
		pProject.reset();
	}
	AppContext::UpdateObjectView();

	CFrameWndEx::OnClose();
}
CObjectView* CMainFrame::GetObjectView()
{
	return &m_wndObjectView;
}
COutputWnd* CMainFrame::GetOutput()
{
	return &m_wndOutput;
}
CPropertiesWnd*	CMainFrame::GetPropGrid()
{
	return &m_wndProperties;
}
void CMainFrame::OnCreatefromtemplateCube()
{
	AppContext::GetProject()->CreateObject_FromTemplate_Cube();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnGameobjectCreateempty()
{
	AppContext::GetProject()->CreateObject_Empty();

	AppContext::UpdateObjectView();

	// TODO: �ڴ���������������
}


void CMainFrame::OnCreatefromtemplateSphere()
{
	AppContext::GetProject()->CreateObject_FromTemplate_Sphere();

	AppContext::UpdateObjectView();
	// TODO: �ڴ���������������
}


void CMainFrame::OnCreatefromtemplateDirlight()
{
	AppContext::GetProject()->CreateObject_FromTemplate_DirLight();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnCreatefromtemplatePointlight()
{
	AppContext::GetProject()->CreateObject_FromTemplate_PointLight();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnCreatefromtemplateSpotlight()
{
	AppContext::GetProject()->CreateObject_FromTemplate_SpotLight();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnCreatefromtemplatePlane()
{
	AppContext::GetProject()->CreateObject_FromTemplate_Plane();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnViewShowgrid()
{
	bool bShow = AppContext::GetRenderer()->ShowingGrid();

	AppContext::GetRenderer()->ShowGrid(!bShow);
	// TODO: �ڴ���������������
}


void CMainFrame::OnUpdateViewShowgrid(CCmdUI *pCmdUI)
{
	bool bShow = AppContext::GetRenderer()->ShowingGrid();
	pCmdUI->SetCheck(bShow);
}


void CMainFrame::OnEditAppsettings()
{

	CAppSettingsDlg dlg(this);

	dlg.DoModal();
}


void CMainFrame::OnImportImportmaxmesh()
{
	// TODO: �ڴ���������������

	MeshImporter_3DSMax imp;

	imp.Import(L"./a.max");
}


void CMainFrame::OnUpdateCreatefromtemplateCube(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(Project::Instance()->IsEmpty() == false);
	// TODO: �ڴ������������û����洦��������
}
void CMainFrame::SetFPS(float fps)
{
	CString str;
	str.Format(L"FPS: %.1f", fps);
	m_wndStatusBar.SetPaneText(1, str);

}


void CMainFrame::OnCreatefromtemplateCone()
{
	AppContext::GetProject()->CreateObject_FromTemplate_Cone();

	AppContext::UpdateObjectView();
}


void CMainFrame::OnCreatefromtemplateSkylight()
{
	AppContext::GetProject()->CreateObject_FromTemplate_SkyLight();

	AppContext::UpdateObjectView();
}
bool CMainFrame::UpdateComponentMenu(CMFCPopupMenu* pMenu)
{
	if(pMenu == nullptr)
	{
		return false;
	}

	CString str;
	CMFCToolBarMenuButton* pParent = pMenu->GetParentButton();

	if(pParent == nullptr)
	{
		return false;
	}
	if(pParent->m_strText != L"Component")
	{
		return false;
	}

	using namespace engine;

	boost::unordered_map<std::wstring, std::vector<ExtPackage::ComponentClass*> >::iterator it = m_ComClassMap.begin();

	pMenu->RemoveAllItems();

	int id = ID_COMPONENT_MENU_BASE + 1;
	for(it; it != m_ComClassMap.end(); ++it)
	{
		if(it->second.size() == 0)
		{
			continue;
		}
		CMenu m;
		m.CreateMenu();
		for(size_t i = 0; i < it->second.size(); ++i)
		{
			m.InsertMenuW(i, MF_BYPOSITION, id, it->second[i]->m_name.c_str());

			id++;
		}

		CMFCToolBarMenuButton b(-1, m.GetSafeHmenu(), -1, it->first.c_str());
		m.Detach();

		pMenu->InsertItem(b);
	}

	return true;
}


BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu* pMenuPopup)
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateComponentMenu(pMenuPopup);

	return CFrameWndEx::OnShowPopupMenu(pMenuPopup);
}


void CMainFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	// TODO: �ڴ����ר�ô����/����û���

	CFrameWndEx::OnUpdateFrameMenu(hMenuAlt);
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWndEx::OnCommand(wParam, lParam);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
void CMainFrame::OnComponentMenu(UINT nID)
{
	using namespace engine;
	ExtPackage::ComponentClass* pClass = FindClassByMenuID(nID);

	GameObjectPtr pObj = AppContext::GetSelectedObject();

	if(pObj->GetComponent(pClass->m_name) != GameObjectComponentPtr())
	{
		CString str = pClass->m_name.c_str() + CString(L" can only be added once per object");
		MessageBox(str, L"Component Error", MB_ICONERROR);
		return;
	}

	GameObjectComponentPtr pCom = AppContext::CreateGameObjectComponent(pClass->m_name);

	pObj->AddComponent(pCom);

	AppContext::UpdatePropGrid(pObj);
	return;
}
bool CMainFrame::UpdateComClassMap()
{
	using namespace engine;

	m_ComClassMap.clear();

	GameObjectManagerPtr pManager = AppContext::GetCoreApi()->GetGameObjectManager();

	for(size_t i = 0; i < pManager->GetPackageCount(); ++i)
	{
		ExtPackage* pPack = pManager->GetPackageByIndex(i);
		for(size_t ii = 0; ii < pPack->GetClassCount(); ++ii)
		{
			ExtPackage::ComponentClass* pClass = pPack->GetClassByIndex(ii);

			if(pClass->m_name == L"PropertyManager")
			{
				continue;
			}
			if(pClass->m_catalog == L"DT")
			{
				continue;
			}
			m_ComClassMap[pClass->m_catalog].push_back(pClass);
		}
	}
	return true;
}
engine::ExtPackage::ComponentClass* CMainFrame::FindClassByMenuID(UINT uID)
{
	using namespace engine;

	boost::unordered_map<std::wstring, std::vector<ExtPackage::ComponentClass*> >::iterator it = m_ComClassMap.begin();

	int id = ID_COMPONENT_MENU_BASE + 1;
	for(it; it != m_ComClassMap.end(); ++it)
	{
		if(it->second.size() == 0)
		{
			continue;
		}
		for(size_t i = 0; i < it->second.size(); ++i)
		{
			if(id == uID)
			{
				return it->second[i];
			}

			id++;
		}

	}

	return nullptr;
}
void CMainFrame::OnUpdateComponentMenuUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(AppContext::GetSelectedObject() != engine::GameObjectPtr());
}


void CMainFrame::OnRenderingSetting()
{
	// TODO: �ڴ���������������

	CRenderingSettingDlg dlg(this);

	if(IDOK != dlg.DoModal())
	{

	}
}


BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.lParam = lParam;
	msg.wParam = wParam;

	AppContext::HandleMessage(msg);

	return CFrameWndEx::OnWndMsg(message, wParam, lParam, pResult);
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWndEx::WindowProc(message, wParam, lParam);
}


void CMainFrame::OnBuildPreview()
{
	CPreviewWnd wnd(this);

	wnd.DoModal();
}
