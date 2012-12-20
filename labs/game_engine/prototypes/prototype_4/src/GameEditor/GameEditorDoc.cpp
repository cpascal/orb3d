
// GameEditorDoc.cpp : CGameEditorDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "GameEditor.h"
#endif

#include "AppContext.h"
#include "GameEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameEditorDoc

IMPLEMENT_DYNCREATE(CGameEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameEditorDoc, CDocument)
END_MESSAGE_MAP()


// CGameEditorDoc ����/����

CGameEditorDoc::CGameEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CGameEditorDoc::~CGameEditorDoc()
{
}

BOOL CGameEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	ProjectPtr pProject = AppContext::GetProject();

	if(pProject)
	{
		pProject->Close();
		AppContext::SetProject(ProjectPtr());
		pProject.reset();
	}

	pProject = ProjectPtr(new Project);

	if(pProject->New() == false)
	{
		AppContext::OutputInfo(L"Failed to create new project.");
		return TRUE;
	}
	AppContext::SetProject(pProject);

	AppContext::OutputInfo(L"Project created.");

	UpdateAllViews(NULL);

	return TRUE;
}


// CGameEditorDoc ���л�

void CGameEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}

}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CGameEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CGameEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CGameEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGameEditorDoc ���

#ifdef _DEBUG
void CGameEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameEditorDoc ����


BOOL CGameEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	ProjectPtr pProject = AppContext::GetProject();

	if(pProject == nullptr)
	{
		pProject = ProjectPtr(new Project);
	}
	else
	{
		pProject->Close();
	}

	if(pProject->Load(lpszPathName) == false)
	{
		AppContext::OutputInfo(L"Failed to open project.");
		return FALSE;
	}

	AppContext::SetProject(pProject);

	AppContext::OutputInfo(L"Project openned.");

	return TRUE;
}


BOOL CGameEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���


	ProjectPtr pProject = AppContext::GetProject();

	if(pProject == nullptr)
	{
		return FALSE;
	}
	
	if(pProject->Save(lpszPathName) == false)
	{
		AppContext::OutputInfo(L"Failed to save project.");
		return FALSE;
	}
	AppContext::OutputInfo(L"Project saved.");
	return TRUE;

	//return CDocument::OnSaveDocument(lpszPathName);
}
