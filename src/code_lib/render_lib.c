#include "pch.h"

DLLEXPORT
char get_inclination(float val)
{
    if (val > 2.5f)
    {
        return '\\';
    }
    else if (val >= 0.5f && val <= 2.5f)
    {
        return '-';
    }
    else if (val < 2.5f && val > 0.2f)
    {
        return '/';
    }
    else if (val <= 0.2f && val >= -0.18f)
    {
        return '|';
    }
    else if (val < -0.18f && val >= -0.4f)
    {
        return '\\';
    }
    else if (val < -1.f && val >= -1.5f)
    {
        return '-';
    } 
    else
    {
        return '\\';
    }
}