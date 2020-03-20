#pragma once
#include "pch.h"

typedef struct TerminalDimensions
{
    uint32_t width, height;
} TerDim_t, TerminalDimensions_t;

typedef struct TerminalCanvas
{
    TerminalDimensions_t dim;
    char* canvas_grid;
} TerCnv_t, TerminalCanvas_t;

typedef struct GameVariables
{
    bool stop;
    uint32_t i;
} GameVars_t, GameVariables_t;