#pragma once

int Clamp(int degNum, int clamp = 360)
{
    int current = degNum;
    if (degNum < 0)
    {
        current = clamp + degNum; // = CData(ww, c);
    }

    if (degNum >= 360)
    {
        current = degNum - clamp; // = CData(ww, c);
    }
    return current;
}
