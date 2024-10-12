#include "SandRecvAll.h"


int SandRecvAll::sand_all(SOCKET s, char* buf, int len, int flags)
{
    int total = 0;
    int n{};

    while (total < len)
    {
        n = send(s, buf + total, len - total, flags);
        if (n == -1 || n == 0)
        {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

int SandRecvAll::recv_all(SOCKET s, char* buf, int len, int flags)
{
    int total = 0;
    int n{};

    while (total < len)
    {
        n = recv(s, buf + total, len - total, flags);
        if (n == -1 || n == 0)
        {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

