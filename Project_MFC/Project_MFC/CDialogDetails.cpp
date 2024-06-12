#include "pch.h"
#include "Project_MFC.h"
#include "CDialogDetails.h"
#include "afxdialogex.h"
#include "Project_MFCDoc.h"

IMPLEMENT_DYNAMIC(CDialogDetails, CDialogEx)

CDialogDetails::CDialogDetails(CProjectMFCDoc* pDoc, double pointSize, double fontSize, CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_DETAILS, pParent), m_PointSize(pointSize), m_FontSize(fontSize)
    //, pDocum(nullptr) // Initialize pDocum to nullptr
    //, pDat(nullptr)   // Initialize pDat to nullptr
{
    if (pDoc == nullptr)
    {
        return;
    }

    pExcept = GetExceptPtr();
    pDocum = pDoc;
    pDat = pDocum->pDat;

    if (pDat == nullptr)
    {
        return;
    }

    //memset(&lvi, 0, sizeof(LVITEMA));
}

CDialogDetails::CDialogDetails(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_DETAILS, pParent)
{


}

CDialogDetails::~CDialogDetails()
{
}

void CDialogDetails::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_POINT_SIZE, m_PointSize);
    DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_FontSize);
}

BOOL CDialogDetails::OnInitDialog()
{
    auto pointSize = pDocum->pDat[0].pointSize;
    auto fontSize = pDocum->pDat[0].fontSize;

    if (pointSize > 0 && pointSize < 1000) {
        m_PointSize = pointSize;
    }
    else {
        m_PointSize = m_PointSize;
    }

    if (fontSize > 0 && fontSize < 1000) {
        m_FontSize = fontSize;
    }
    else {
        m_FontSize = m_FontSize;
    }
    CDialogEx::OnInitDialog();

    ASSERT(pDat);

    return TRUE;
}

void CDialogDetails::OnBnClickedOk()
{
    CDialogEx::OnOK();

    try {
        char st[512];
        UpdateData(TRUE);


        for (size_t i = 0; i < pDocum->pDat->size(); ++i) {
            (*pDocum->pDat)[i].pointSize = m_PointSize;
            (*pDocum->pDat)[i].fontSize = m_FontSize;
        }


        UpdateData(FALSE);
        pDocum->SetModifiedFlag();
        pDocum->UpdateAllViews(NULL);
    }
    catch (const std::exception& ex) {
        pExcept->PutMessage(IDS_EXCEPTION_WRONG_DATA);
    }
}

BEGIN_MESSAGE_MAP(CDialogDetails, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDialogDetails::OnBnClickedOk)
END_MESSAGE_MAP()