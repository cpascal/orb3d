
// GameEditorView.h : CGameEditorView ��Ľӿ�
//

#pragma once

class GridMesh;

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

	void Render();
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	
	bool			m_bRotating;
private:
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnDestroy();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // GameEditorView.cpp �еĵ��԰汾
inline CGameEditorDoc* CGameEditorView::GetDocument() const
   { return reinterpret_cast<CGameEditorDoc*>(m_pDocument); }
#endif

