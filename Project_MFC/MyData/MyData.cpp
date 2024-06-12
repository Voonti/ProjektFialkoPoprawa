// MyData.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "MyData.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CMyDataApp

BEGIN_MESSAGE_MAP(CMyDataApp, CWinApp)
END_MESSAGE_MAP()


// CMyDataApp construction

CMyDataApp::CMyDataApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMyDataApp object

CMyDataApp theApp;


// CMyDataApp initialization

BOOL CMyDataApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


MY_POINT* MY_DATA::allocTab(MY_POINT* ptab, int n)
{
    try
    {
    if (!ptab)
    {
        ptab = new MY_POINT[n];
        capacity = n;
    }
    }
    catch (std::bad_alloc)
    {
        //CExcept1App* pExp = (CExcept1App*)pExcept;
        //pExcept->PutMessage(1003);
    }

    return ptab;
}

MY_DATA::MY_DATA(int no_it)
{
    pTab = NULL;
    Init(no_it);
}

MY_DATA::MY_DATA(const MY_DATA& ob)
{
    capacity = ob.capacity;
    last = ob.last;
    pTab = NULL;
    pTab = allocTab(pTab, capacity);
    if (pTab)
        std::copy(ob.pTab, ob.pTab + last, pTab);
}

void MY_DATA::Init(int no_it)
{
    capacity = no_it;
    last = 0;
    pTab = allocTab(pTab, capacity);
}

void MY_DATA::Push(const MY_POINT& tmp)
{
    if (last >= capacity)
    {
        int delta = last - capacity;
        int new_capacity = (delta > capacity) ? (capacity + delta) : (2 * capacity);
        int old_capacity(capacity);
        MY_POINT* newTab(NULL);
        newTab = allocTab(newTab, new_capacity);
        std::copy(pTab, pTab + old_capacity, newTab);
        delete[] pTab;
        pTab = newTab;
        //capacity = new_capacity;
    }

    pTab[last++] = tmp;
}


void MY_DATA::clear_all()
{
    delete[] pTab;
    pTab = new MY_POINT[capacity];
}





string MY_DATA::OpenFileDialog()
{
    OPENFILENAME ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Binary Files\0*.bin\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return std::string(ofn.lpstrFile);
    }
    return std::string();
}

bool MY_DATA::SaveFileDialog(char* filename, DWORD nMaxFile)
{
    OPENFILENAME ofn;
    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "All Files\0*.*\0Binary Files\0*.bin\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = nMaxFile;
    ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT;
    ofn.lpstrTitle = "Save As";

    return GetSaveFileName(&ofn);
}

void MY_DATA::addObject(const MY_POINT& newPoint) {
    if (last >= capacity) {
        capacity = capacity * 2 + 1;
        pTab = allocTab(pTab, capacity);
    }
    pTab[last] = newPoint;

    pTab[last].name = new char[32];
    if (newPoint.name) {
        size_t nameLength = strlen(newPoint.name);
        pTab[last].name = new char[nameLength + 1];

        strcpy_s(pTab[last].name, nameLength + 1, newPoint.name);
    }

    ++last;
}

void MY_DATA::getLastObject()
{
    if (size() != 0) {
        MY_POINT* lastPoint = &pTab[last - 1];
        std::cout << "X: " << lastPoint->x << ", Y: " << lastPoint->y << std::endl;
        std::cout << "Name: " << (lastPoint->name ? lastPoint->name : "NULL") << std::endl;
        std::cout << "Number: " << lastPoint->numb << std::endl;
        std::cout << "Color: " << lastPoint->color << std::endl;
    }
    else {
        std::cout << "No objects available." << std::endl;
    }
}

void MY_DATA::removeObject()
{
    if (last > 0) {
        last--;
        MY_POINT& removedPoint = pTab[last];
        delete[] removedPoint.name;
        removedPoint.name = nullptr;
        removedPoint.x = 0;
        removedPoint.y = 0;
        removedPoint.numb = 0;
        removedPoint.color = 0;
    }
    else {
        std::cout << "No objects to remove." << std::endl;
    }
}

void MY_DATA::removeAllObjects()
{
    for (int i = 0; i < last; ++i) {
        delete[] pTab[i].name;  // Free the memory allocated for the name
        pTab[i].name = nullptr; // Set the name pointer to nullptr 
    }
    last = 0;  // Reset the last index
}

void MY_DATA::modifyObject(int index)
{
    if (index >= 0 && index < last) {
        MY_POINT& point = pTab[index];
        // Modify the object's properties as needed, for example:
        point.set(15, 30); // Example modification
        point.name = "ModifiedName"; // Example modification
        point.numb = 42; // Example modification
        point.color = RGB(255, 0, 0); // Example modification
    }
    else {
        std::cout << "Invalid index." << std::endl;
    }
}

void MY_DATA::findObjectsByNumber(int number)
{
    bool found = false;
    for (int i = 0; i < last; ++i) {
        if (pTab[i].numb == number) {
            std::cout << "Found object with number " << number << ": X: " << pTab[i].x << ", Y: " << pTab[i].y << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No objects found with number " << number << "." << std::endl;
    }
}

void MY_DATA::displayData()
{
    for (int i = 0; i < last; ++i) {
        const MY_POINT& point = pTab[i];
        std::cout << "X: " << point.x << ", Y: " << point.y << std::endl;
        std::cout << "Name: " << (point.name ? point.name : "NULL") << std::endl;
        std::cout << "Number: " << point.numb << std::endl;
        std::cout << "Color: " << point.color << std::endl;
    }
}

void MY_DATA::exportToCSV(const std::string& filename)
{
    std::ofstream file(filename);
    file << "X,Y,Name,Number,Color" << std::endl;
    for (int i = 0; i < last; ++i) {
        const MY_POINT& point = pTab[i];
        file << point.x << "," << point.y << "," << (point.name ? point.name : "") << "," << point.numb << "," << point.color << std::endl;
    }
    file.close();
    std::string command = "start excel " + filename;
    system(command.c_str());
}

void MY_DATA::quitApplication()
{
    removeAllObjects();
    exit(0);
}

void MY_DATA::GetMaxMinCoords(double& max_x, double& min_x, double& max_y, double& min_y)
{
    max_x = min_x = max_y = min_y = 0;
    if (pTab && last)
    {
        max_x = min_x = pTab[0].x;
        max_y = min_y = pTab[0].y;

        for (int i = 1; i < last; ++i)
        {
            if (pTab[i].x > max_x)
                max_x = pTab[i].x;
            if (pTab[i].y > max_y)
                max_y = pTab[i].y;
            if (pTab[i].x < min_x)
                min_x = pTab[i].x;
            if (pTab[i].y < min_y)
                min_y = pTab[i].y;
        }
    }
}

bool MY_DATA::exportToCSV(const char* filename) {

    std::ofstream outFile(filename);

    // Write header
    outFile << "x,y,name,numb,color\n";

    // Write data
    for (int i = 0; i < last; ++i) {
        outFile << pTab[i].x << ","
            << pTab[i].y << ","
            << (pTab[i].name ? pTab[i].name : "") << ","
            << pTab[i].numb << ","
            << pTab[i].color << "\n";
    }

    outFile.close();
    return true;

}

std::wstring convertToWideString(const char* str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstrTo[0], size_needed);
    return wstrTo;
}

void MY_DATA::openCSVInExcel(const char* filename) {
    if (filename == nullptr) {
        return;
    }

    // Convert filename to the appropriate string type based on project settings
#ifdef UNICODE
    std::wstring wideFilename = convertToWideString(filename);
    LPCTSTR lpFilename = wideFilename.c_str();
#else
    LPCTSTR lpFilename = filename;
#endif

    // Open the CSV file in Excel
    HINSTANCE result = ShellExecute(0, 0, _T("excel.exe"), lpFilename, 0, SW_SHOW);

    // Check the result
    if ((int)result <= 32) {
        return;
    }
}

