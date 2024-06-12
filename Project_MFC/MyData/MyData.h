// MyData.h : main header file for the MyData DLL
//

#pragma once

#include "MyPoint.h"
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <shellapi.h>

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#ifdef MYDATA_EXPORTS
#define MYDATA_API __declspec(dllexport)
#else
#define MYDATA_API __declspec(dllimport)
#endif

#include "resource.h"		// main symbols
#include <string>
// CMyDataApp
// See MyData.cpp for the implementation of this class
//

class CMyDataApp : public CWinApp
{
public:
	CMyDataApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#pragma pack(push, 1)

class MYDATA_API MY_DATA : public MY_POINT
{
protected:
	MY_POINT* pTab;
	int capacity;
	int last;
public:
	MY_DATA(int no_it);
	MY_DATA(const MY_DATA& ob);
	~MY_DATA() { Free(); }
	void Free() { if (pTab) delete[] pTab; pTab = NULL; }
	void Init(int no_it);
	void Push(const MY_POINT& tmp);
	int size() { return last; }
	void clear() { last = 0; }
	void clear_all();
	void saveToFile(const char* filename);
	bool loadFromFile(const char* filename);
	std::string OpenFileDialog();
	bool SaveFileDialog(char* filename, DWORD nMaxFile);
	bool exportToCSV(const char* filename);
	void openCSVInExcel(const char* filename);

	MY_POINT* getPtab() {
		return this->pTab;
	}

	void addObject(const MY_POINT& newPoint);
	void getLastObject();
	void removeObject();
	void removeAllObjects();
	void modifyObject(int index);
	void findObjectsByNumber(int number);
	void displayData();
	void exportToCSV(const std::string& filename);
	void quitApplication();

	MY_POINT& operator [] (const int i)
	{
		return pTab[i];
	}

	template <class T, class Key>
	T* find(const T* p_begin, const T* p_end, const Key& k) {
		for (const T* p = p_begin; p != p_end; ++p) {
			if (strcmp(p->name, k) == 0) {
				return const_cast<T*>(p);
			}
		}
		return nullptr;
	}

	void GetMaxMinCoords(double& max_x, double& min_x, double& max_y, double& min_y);

private:
	MY_POINT* allocTab(MY_POINT* pTab, int i);
};

#pragma pack(pop)
