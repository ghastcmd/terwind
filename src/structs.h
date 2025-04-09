#pragma once

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
    float x_pos;
    float y_pos;
    bool i, j;

    float another_x_pos;
    float another_y_pos;

    bool can_rotate;
} GameVars_t, GameVariables_t, CanvasVariables_t;