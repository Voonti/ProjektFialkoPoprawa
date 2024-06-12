// CDialogInputData.cpp : implementation file
//

#include "pch.h"
#include "Project_MFC.h"
#include "CDialogInputData.h"
#include "afxdialogex.h"
#include "CDialogDetails.h"

// CDialogInputData dialog

IMPLEMENT_DYNAMIC(CDialogInputData, CDialogEx)

CDialogInputData::CDialogInputData(CProjectMFCDoc* pDoc, CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_INPUT_DATA, pParent)
    , m_X(0)
    , m_Y(0)
    , m_Color(RGB(0, 0, 0))
    , m_Name(_T(""))
    , pDocum(nullptr) // Initialize pDocum to nullptr
    , pDat(nullptr)   // Initialize pDat to nullptr
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

    memset(&lvi, 0, sizeof(LVITEMA));
}



CDialogInputData::~CDialogInputData()
{
    m_smallImageList.DeleteImageList();
}

void CDialogInputData::DoDataExchange(CDataExchange* pDX)
{
    CStringA tempName;
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
    DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
}

BEGIN_MESSAGE_MAP(CDialogInputData, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CDialogInputData::OnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_MOD, &CDialogInputData::OnClickedButtonMod)
    ON_BN_CLICKED(IDC_BUTTON_DEL, &CDialogInputData::OnClickedButtonDel)
    ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_CTRL, &CDialogInputData::OnItemchangingListCtrl)
    ON_BN_CLICKED(IDOK, &CDialogInputData::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_COLOR, &CDialogInputData::OnClickedButtonColor)
    ON_BN_CLICKED(IDC_BUTTON_EXCEL, &CDialogInputData::OnClickedButtonExcel)
    ON_BN_CLICKED(IDC_BUTTON_DETAILS, &CDialogInputData::OnBnClickedButtonDetails)
END_MESSAGE_MAP()

BOOL CDialogInputData::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    VERIFY(m_ColorBox.SubclassDlgItem(IDC_STATIC_COLOR, this));

    CString strx, stry, strColor, strName, strPointSize, strFontSize;
    lvi.mask = LVIF_TEXT;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iImage = 0;

    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ProjectMFCTYPE));
    m_smallImageList.Create(16, 16, FALSE, 1, 0);
    m_smallImageList.Add(hIcon);
    m_ListCtrl.SetImageList(&m_smallImageList, LVSIL_SMALL);

    CRect rectListCtrl;
    m_ListCtrl.GetClientRect(&rectListCtrl);

    int list_ctrl_width = rectListCtrl.Width();
    int column_width = list_ctrl_width / 6;

    m_ListCtrl.InsertColumn(0, "X", LVCFMT_LEFT, column_width, 0);
    m_ListCtrl.InsertColumn(1, "Y", LVCFMT_LEFT, column_width, 1);
    m_ListCtrl.InsertColumn(2, "color", LVCFMT_LEFT, column_width, 2);
    m_ListCtrl.InsertColumn(3, "name", LVCFMT_LEFT, column_width, 3);
    m_ListCtrl.InsertColumn(4, "size", LVCFMT_LEFT, column_width, 4);
    m_ListCtrl.InsertColumn(5, "font", LVCFMT_LEFT, column_width, 5);

    ASSERT(pDat);
    int no_item = pDat->size();
    lvi.iSubItem = 0;
    m_ListCtrl.SetItemCount(no_item);

    for (int i = 0; i < no_item; i++)
    {
        lvi.iItem = i;
        strx.Format(_T("%.2f"), (*pDat)[i].x); // strx bêdzie zawieraæ "50.0"
        stry.Format(_T("%.2f"), (*pDat)[i].y); // stry bêdzie zawieraæ "5.0"
        strPointSize.Format(_T("%.2f"), (*pDat)[i].pointSize); // stry bêdzie zawieraæ "5.0"
        strFontSize.Format(_T("%.2f"), (*pDat)[i].fontSize); // stry bêdzie zawieraæ "5.0"
        strColor.Format("%02X%02X%02X", GetRValue((*pDat)[i].color), GetGValue((*pDat)[i].color), GetBValue((*pDat)[i].color)); // Correct RGB order
        strName = (*pDat)[i].name;
        lvi.pszText = " ";
        lvi.cchTextMax = (int)(strx.GetLength() + stry.GetLength() + strColor.GetLength());
        m_ListCtrl.InsertItem(&lvi);
        m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
        m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
        m_ListCtrl.SetItemText(lvi.iItem, 2, strColor);
        m_ListCtrl.SetItemText(lvi.iItem, 3, strName);
        m_ListCtrl.SetItemText(lvi.iItem, 4, strPointSize);
        m_ListCtrl.SetItemText(lvi.iItem, 5, strFontSize);
    }

    return TRUE;
}

void CDialogInputData::OnBnClickedButtonDetails()
{
    //CProjectMFCDoc* pDoc = GetDocument();
    char st[512];

    m_ListCtrl.GetItemText(0, 4, st, sizeof(st));
    double pointSize = atof(st);

    m_ListCtrl.GetItemText(0, 5, st, sizeof(st));
    double fontSize = atof(st);

    CDialogDetails dlg(pDocum, pointSize, fontSize);
    dlg.DoModal();

    CString strPointSize, strFontSize;

    ASSERT(pDat);
    int no_item = m_ListCtrl.GetItemCount();
    for (int i = 0; i < no_item; i++)
    {
        strPointSize.Format(_T("%.2f"), dlg.m_PointSize);
        strFontSize.Format(_T("%.2f"), dlg.m_FontSize);
        m_ListCtrl.SetItemText(lvi.iItem, 4, strPointSize);
        m_ListCtrl.SetItemText(lvi.iItem, 5, strFontSize);
    }
}

void CDialogInputData::ModifyData()
{
    try {
        char st[512];
        UpdateData(TRUE);
        const int no_it = m_ListCtrl.GetItemCount();

        if (pDat)
            delete pDat;
        pDat = new MY_DATA(no_it);
        pDocum->pDat = pDat;

        for (int nItem = 0; nItem < no_it; ++nItem)
        {
            m_ListCtrl.GetItemText(nItem, 0, st, sizeof(st));
            double x = atof(st);
            m_ListCtrl.GetItemText(nItem, 1, st, sizeof(st));
            double y = atof(st);
            m_ListCtrl.GetItemText(nItem, 2, st, sizeof(st));

            int r, g, b;
            sscanf_s(st, "%02X%02X%02X", &r, &g, &b);
            COLORREF color = RGB(r, g, b);
        
            m_ListCtrl.GetItemText(nItem, 3, st, sizeof(st));
            char* name = new char[strlen(st) + 1];  // Allocate memory for name
            strcpy_s(name, strlen(st) + 1, st);

            m_ListCtrl.GetItemText(nItem, 4, st, sizeof(st));
            double pointSize = atof(st);

            m_ListCtrl.GetItemText(nItem, 5, st, sizeof(st));
            double fontSize = atof(st);

            //double pointSize = 16.0; //dupa change

            //double fontSize = 16.0;//dupa change


            pDat->Push({ x, y, color, name, pointSize, fontSize });
        }

        UpdateData(FALSE);
        pDocum->SetModifiedFlag();
        pDocum->UpdateAllViews(NULL);
    }
    catch (const std::exception& ex) {
        pExcept->PutMessage(IDS_EXCEPTION_WRONG_DATA);
    }
}



void CDialogInputData::OnBnClickedOk()
{
    CDialogEx::OnOK();
    ModifyData();
}

void CDialogInputData::OnClickedButtonAdd()
{
    UpdateData(TRUE);
    CString strx, stry, strColor, strName, strPointSize, strFontSize;
    strx.Format(_T("%.2f"), m_X);
    stry.Format(_T("%.2f"), m_Y);
    strPointSize.Format(_T("%.2f"), 16.0);
    strFontSize.Format(_T("%.2f"), 16.0);
    strColor.Format("%02X%02X%02X", GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color)); // If storing color as text
    strName = m_Name;

    lvi.iItem = m_ListCtrl.GetItemCount();
    lvi.pszText = " ";
    lvi.cchTextMax = (int)(strx.GetLength() + stry.GetLength() + strColor.GetLength());
    m_ListCtrl.InsertItem(&lvi);
    m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
    m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
    m_ListCtrl.SetItemText(lvi.iItem, 2, strColor); // If storing color as text
    m_ListCtrl.SetItemText(lvi.iItem, 3, strName);
    m_ListCtrl.SetItemText(lvi.iItem, 4, strPointSize);
    m_ListCtrl.SetItemText(lvi.iItem, 5, strFontSize);

    UpdateData(FALSE);
    pDocum->SetModifiedFlag();
    pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnClickedButtonMod()
{
    if (m_SelItem < 0 || m_SelItem >= m_ListCtrl.GetItemCount())
        return;

    UpdateData(TRUE);
    CString strx, stry, strColor, strName, strPointSize, strFontSize;
    strx.Format(_T("%.2f"), m_X);
    stry.Format(_T("%.2f"), m_Y);
    strPointSize.Format(_T("%.2f"), 16.0);
    strFontSize.Format(_T("%.2f"), 16.0);
    strColor.Format("%02X%02X%02X", GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color)); // Correct RGB order
    strName = m_Name;

    m_ListCtrl.SetItemText(m_SelItem, 0, strx);
    m_ListCtrl.SetItemText(m_SelItem, 1, stry);
    m_ListCtrl.SetItemText(m_SelItem, 2, strColor);
    m_ListCtrl.SetItemText(m_SelItem, 3, strName);
    m_ListCtrl.SetItemText(m_SelItem, 4, strPointSize);
    m_ListCtrl.SetItemText(m_SelItem, 5, strFontSize);

    UpdateData(FALSE);
    ModifyData();
}

void CDialogInputData::OnClickedButtonDel()
{
    if (m_SelItem < 0 || m_SelItem >= m_ListCtrl.GetItemCount())
        return;

    m_ListCtrl.DeleteItem(m_SelItem);
    UpdateData(FALSE);
    pDocum->pDat->removeObject();
    pDocum->SetModifiedFlag();
    pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    *pResult = 0;

    if (pNMLV->uChanged & LVIF_STATE && !(pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_SELECTED))
    {
        m_SelItem = pNMLV->iItem;

        char st[512];
        m_ListCtrl.GetItemText(m_SelItem, 0, st, sizeof(st));
        m_X = atof(st);
        m_ListCtrl.GetItemText(m_SelItem, 1, st, sizeof(st));
        m_Y = atof(st);

        m_ListCtrl.GetItemText(m_SelItem, 2, st, sizeof(st));

        int r, g, b;
        sscanf_s(st, "%02X%02X%02X", &r, &g, &b);
        COLORREF color = RGB(r, g, b);

        m_ColorBox.SetItem(m_SelItem);
        m_ColorBox.Invalidate();
        m_ColorBox.SetColor(color);

        m_ListCtrl.GetItemText(m_SelItem, 3, st, sizeof(st));
        m_Name = st;

        UpdateData(FALSE);
    }
}

void CDialogInputData::OnClickedButtonColor()
{
    CColorDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        m_Color = dlg.GetColor();
        m_ColorBox.SetColor(m_Color);
        m_ColorBox.Invalidate();
    }
}

void CDialogInputData::OnClickedButtonExcel()
{
    try {
        if (pDocum->pDat == nullptr || pDocum->pDat->size() == 0) {
            throw std::runtime_error("");
        }

        pDocum->pDat->exportToCSV("data.csv");
        pDocum->pDat->openCSVInExcel("data.csv");
    }
    catch (const std::exception& ex) {
        pExcept->PutMessage(IDS_EXCEPTION_NO_POINTS);
    }
}

///////////////////////////////////////////////////////////////////////////////
//   CColorBox

BEGIN_MESSAGE_MAP(CColorBox, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CColorBox::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    CBrush newBrush(color);
    CBrush* oldBrush = dc.SelectObject(&newBrush);
    dc.Rectangle(&rect);
    dc.SelectObject(oldBrush);
}

void CColorBox::SetItem(int i)
{
    COLORREF colors[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
    color = colors[i % _countof(colors)];
}






