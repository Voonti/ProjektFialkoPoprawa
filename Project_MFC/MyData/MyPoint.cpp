#pragma once

#include "pch.h"
#include "MyPoint.h"
#include <cstring>

MY_POINT::~MY_POINT()
{
    if (name) {
        delete[] name;
        name = nullptr;
    }
}

