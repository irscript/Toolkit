#include <airkit/Hash/airHash32.h>

namespace air::Hash
{
    uint32 RS(const uint8 *aData, uint nSize)
    {
        uint32 nB = 378551;
        uint32 nA = 63689;
        uint32 nHash = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = nHash * nA + aData[i];
            nA = nA * nB;
        }

        return nHash;
    }

    uint32 JS(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 1315423911;
        for (uint i = 0; i < nSize; ++i)
        {
            nHash ^= ((nHash << 5) + aData[i] + (nHash >> 2));
        }

        return nHash;
    }

    uint32 PJW(const uint8 *aData, uint nSize)
    {
        const uint32 BitsInUnsignedInt = (uint32)(sizeof(uint32) * 8);
        const uint32 ThreeQuarters = (uint32)((BitsInUnsignedInt * 3) / 4);
        const uint32 OneEighth = (uint32)(BitsInUnsignedInt / 8);
        const uint32 HighBits = (uint32)(0xFFFFFFFFU) << (BitsInUnsignedInt - OneEighth);
        uint32 nHash = 0;
        uint32 nTest = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = (nHash << OneEighth) + aData[i];

            if ((nTest = nHash & HighBits) != 0)
            {
                nHash = ((nHash ^ (nTest >> ThreeQuarters)) & (~HighBits));
            }
        }

        return nHash;
    }

    uint32 ELF(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 0;
        uint32 x = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = (nHash << 4) + aData[i];
            if ((x = nHash & 0xF0000000U) != 0)
            {
                nHash ^= (x >> 24);
            }
            nHash &= ~x;
        }

        return nHash;
    }
    uint32 BKDR(const uint8 *aData, uint nSize)
    {
        uint32 nSeed = 131313; /* 31 131 1313 13131 131313 etc.. */
        uint32 nHash = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = (nHash * nSeed) + aData[i];
        }

        return nHash;
    }
    uint32 SDBM(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = aData[i] + (nHash << 6) + (nHash << 16) - nHash;
        }

        return nHash;
    }
    uint32 DJB(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 5381;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = ((nHash << 5) + nHash) + aData[i];
        }

        return nHash;
    }
    uint32 DEK(const uint8 *aData, uint nSize)
    {
        uint32 nHash = nSize;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash = ((nHash << 5) ^ (nHash >> 27)) ^ aData[i];
        }
        return nHash;
    }

    uint32 AP(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 0xAAAAAAAA;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash ^= ((i & 1) == 0) ? ((nHash << 7) ^ aData[i] * (nHash >> 3)) : (~((nHash << 11) + (aData[i] ^ (nHash >> 5))));
        }

        return nHash;
    }
    uint32 BP(const uint8 *aData, uint nSize)
    {
        uint32 nHash = 0;
        for (uint i = 0; i < nSize; ++i)
        {
            nHash = nHash << 7 ^ aData[i];
        }

        return nHash;
    }
    uint32 FNV(const uint8 *aData, uint nSize)
    {
        const uint32 cFnvPrime = 0x811C9DC5;
        uint32 nHash = 0;

        for (uint i = 0; i < nSize; ++i)
        {
            nHash *= cFnvPrime;
            nHash ^= aData[i];
        }

        return nHash;
    }
}