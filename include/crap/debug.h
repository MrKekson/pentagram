#pragma once

// #define OP0 18
// #define OP1 5
// #define OP2 4
// #define OP3 2

class DLED
{
private:
    static int _DBGLED[];

public:
    DLED();
    static void Blink(int l, int d = 5);
};
