#ifndef BITBOARDS_H
#define BITBOARDS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h>

typedef union {
    struct {
        uint64_t WhitePawns;
        uint64_t WhiteRooks;
        uint64_t WhiteBishops;
        uint64_t WhiteKnights;
        uint64_t WhiteQueens;
        uint64_t WhiteKing;

        uint64_t BlackPawns;
        uint64_t BlackRooks;
        uint64_t BlackBishops;
        uint64_t BlackKnights;
        uint64_t BlackQueens;
        uint64_t BlackKing;
    };
    uint64_t pieces[12]; // Tablica do indeksowania
} BitBoard;

static uint64_t IndexBitMasks[64] = {
    1ULL << 0,  1ULL << 1,  1ULL << 2,  1ULL << 3,  1ULL << 4,  1ULL << 5,  1ULL << 6,  1ULL << 7,
    1ULL << 8,  1ULL << 9,  1ULL << 10, 1ULL << 11, 1ULL << 12, 1ULL << 13, 1ULL << 14, 1ULL << 15,
    1ULL << 16, 1ULL << 17, 1ULL << 18, 1ULL << 19, 1ULL << 20, 1ULL << 21, 1ULL << 22, 1ULL << 23,
    1ULL << 24, 1ULL << 25, 1ULL << 26, 1ULL << 27, 1ULL << 28, 1ULL << 29, 1ULL << 30, 1ULL << 31,
    1ULL << 32, 1ULL << 33, 1ULL << 34, 1ULL << 35, 1ULL << 36, 1ULL << 37, 1ULL << 38, 1ULL << 39,
    1ULL << 40, 1ULL << 41, 1ULL << 42, 1ULL << 43, 1ULL << 44, 1ULL << 45, 1ULL << 46, 1ULL << 47,
    1ULL << 48, 1ULL << 49, 1ULL << 50, 1ULL << 51, 1ULL << 52, 1ULL << 53, 1ULL << 54, 1ULL << 55,
    1ULL << 56, 1ULL << 57, 1ULL << 58, 1ULL << 59, 1ULL << 60, 1ULL << 61, 1ULL << 62, 1ULL << 63
};





#endif 