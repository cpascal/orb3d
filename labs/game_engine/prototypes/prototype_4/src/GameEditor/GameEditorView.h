
// GameEditorView.h : CGameEditorView ��Ľӿ�
//

#pragma once


class CGameEditorView : public CView
{
protected: // �������л�����
	CGameEditorView();
	DECLARE_DYNCREATE(CGameEditorView)

// ����
public:
	CGameEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CGameEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

#ifndef _DEBUG  // GameEditorView.cpp �еĵ��԰汾
inline CGameEditorDoc* CGameEditorView::GetDocument() const
   { return reinterpret_cast<CGameEditorDoc*>(m_pDocument); }
#endif
