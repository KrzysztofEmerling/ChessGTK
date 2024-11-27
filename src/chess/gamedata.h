#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <stdbool.h>
#include <stdint.h>

extern const char pieceSymbols[12];

typedef union {
    struct {
        uint64_t WhitePawns;
        uint64_t WhiteKnights;
        uint64_t WhiteBishops;
        uint64_t WhiteRooks;
        uint64_t WhiteQueens;
        uint64_t WhiteKing;

        uint64_t BlackPawns;
        uint64_t BlackKnights;
        uint64_t BlackBishops;
        uint64_t BlackRooks;
        uint64_t BlackQueens;
        uint64_t BlackKing;
    };
    uint64_t pieces[12]; // Tablica do indeksowania
} BitBoard;

typedef struct {
    BitBoard chessPieces; 
    bool castlingRights[4];  // [0] - biała krutka, [1] - biała długa, [2] - czarna krutka, [3] - czarna długa.
    bool isWhiteTurn;     
    int enPassant;
    int halfMoveClock;   // Licznik półruchów
    int fullMoveNumber;  // Numer pełnego ruchu
} GameData;

#endif