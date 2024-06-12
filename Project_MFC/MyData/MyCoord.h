#pragma once

#ifdef MYDATA_EXPORTS
#define MYDATA_API __declspec(dllexport)
#else
#define MYDATA_API __declspec(dllimport)
#endif

class MYDATA_API MY_COORD {
protected:
	double x;
	double y;

public:
	MY_COORD(double xCoord, double yCoord) : x(xCoord), y(yCoord) {}

	MY_COORD() : x(0), y(0) {}

	virtual ~MY_COORD();
};