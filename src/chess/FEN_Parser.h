#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include <gtk/gtk.h>

typedef struct {
    int chessPieces[64]; 
    bool castlingRights[4];  // [0] - biała krutka, [1] - biała długa, [2] - czarna krutka, [3] - czarna długa.
    bool isWhiteTurn;     
    int enPassant;
    int halfMoveClock;   // Licznik półruchów
    int fullMoveNumber;  // Numer pełnego ruchu
} GameData;

void parseFEN(const char *fen, GameData *gamedata);
void saveFEN(const GameData *gamedata, char *fen);

#endif