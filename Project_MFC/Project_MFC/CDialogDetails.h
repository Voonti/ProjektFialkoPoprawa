#pragma once
#include "Project_MFCDoc.h"

class CDialogDetails : public CDialogEx
{
    DECLARE_DYNAMIC(CDialogDetails)

public:
    CDialogDetails(CProjectMFCDoc* pDoc, double pointSize, double fontSize, CWnd* pParent = nullptr);
    CDialogDetails(CWnd* pParent = nullptr);   // standard constructor
    virtual ~CDialogDetails();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_DETAILS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
public:
    CExcept1App* pExcept;
public:
    MY_DATA* pDat;
    CProjectMFCDoc* pDocum;
    double m_PointSize;
    double m_FontSize;
    LOGFONT m_Font;
    afx_msg void OnBnClickedOk();
};