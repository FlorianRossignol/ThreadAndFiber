#include "parse_int.h"

int parse_int(const char* p)
{

    int sign = 1;
    if (*p == '-')
    {
        sign = -1;
        p++;
    }
    int val = 0;
    while (true)
    {
        const int n = *p;
        if (n >= '0' && n <= '9')
        {
            val *= 10;
            val += n - '0';
            p++;
        }
        else
            break;
    }
    return val * sign;
}

int parse_int_branchless(const char* p)
{
    const int conditional = *p == '-';
    const int sign = -conditional*2+1;
    p = p + conditional;
    
    int val = 0;
    for(int n = *p; (n >= '0') && (n <= '9');)
    {
        val = val * 10 + (n - '0');
        p++;
        n = *p;
    }
    return sign == -1 ? -val : val;
}