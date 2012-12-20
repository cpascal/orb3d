
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ObjectView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	CObjectView*			GetObjectView();
	COutputWnd*				GetOutput();
	CPropertiesWnd*			GetPropGrid();

	void					SetFPS(float fps);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CObjectView        m_wndObjectView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CMFCCaptionBar    m_wndCaptionBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL CreateCaptionBar();
public:
	afx_msg void OnClose();
	afx_msg void OnCreatefromtemplateCube();
	afx_msg void OnGameobjectCreateempty();
	afx_msg void OnCreatefromtemplateSphere();
	afx_msg void OnCreatefromtemplateDirlight();
	afx_msg void OnCreatefromtemplatePointlight();
	afx_msg void OnCreatefromtemplateSpotlight();
	afx_msg void OnCreatefromtemplatePlane();
	afx_msg void OnViewShowgrid();
	afx_msg void OnUpdateViewShowgrid(CCmdUI *pCmdUI);
	afx_msg void OnEditAppsettings();
//	afx_msg void OnProjectImport();
	afx_msg void OnImportImportmaxmesh();
	afx_msg void OnUpdateCreatefromtemplateCube(CCmdUI *pCmdUI);
};


