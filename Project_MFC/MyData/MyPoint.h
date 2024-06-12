#pragma once

#include "MyCoord.h"
#include <cstring>
#include <string.h>

#ifdef MYDATA_EXPORTS
#define MYDATA_API __declspec(dllexport)
#else
#define MYDATA_API __declspec(dllimport)
#endif

class MYDATA_API MY_POINT : public MY_COORD
{
public:
	double x;
	double y;
	char* name;
    int numb;
    double pointSize;
    double fontSize;
    LOGFONT font;
	COLORREF color;
public:
	MY_POINT() : x(0), y(0), name(nullptr) {}
	MY_POINT(double xx, double yy, COLORREF color, char * name, double pointSize, double fontSize)
        : x(xx), y(yy), name(name), color(color), pointSize(pointSize), fontSize(fontSize){}
	~MY_POINT();
	void set(double xx, double yy) { x = xx; y = yy; }
	MY_POINT get() { return *this; }

    MY_POINT(const MY_POINT& other)
        : x(other.x), y(other.y), numb(other.numb), color(other.color), name(nullptr) {
        if (other.name) {
            size_t len = std::strlen(other.name) + 1;
            name = new char[len];
            strcpy_s(name, len, other.name);
        }
    }
    MY_POINT& operator=(const MY_POINT& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            color = other.color;
            numb = other.numb;
            pointSize = other.pointSize;
            fontSize = other.fontSize;

            if (other.name) {
                size_t len = std::strlen(other.name) + 1;
                name = new char[len];
                strcpy_s(name, len, other.name);
            }
            else {
                name = nullptr;
            }
        }
        return *this;
    }

};
