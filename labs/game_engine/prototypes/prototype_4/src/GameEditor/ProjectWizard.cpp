// ProjectWizard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "ProjectWizard.h"
#include "afxdialogex.h"


// CProjectWizard �Ի���

IMPLEMENT_DYNAMIC(CProjectWizard, CDialogEx)

CProjectWizard::CProjectWizard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectWizard::IDD, pParent)
{

}

CProjectWizard::~CProjectWizard()
{
}

void CProjectWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProjectWizard, CDialogEx)
END_MESSAGE_MAP()


// CProjectWizard ��Ϣ�������


void CProjectWizard::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}
