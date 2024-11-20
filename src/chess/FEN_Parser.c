#include <ctype.h>

#include "FEN_Parser.h"


void parseFEN(const char *fen, GameData *gamedata) {
    // Reset gamedata to default values
    memset(gamedata->chessPieces, 0, sizeof(gamedata->chessPieces));
    memset(gamedata->castlingRights, false, sizeof(gamedata->castlingRights));
    gamedata->isWhiteTurn = false;
    gamedata->enPassant = -1; // -1 means no en passant
    gamedata->halfMoveClock = 0;
    gamedata->fullMoveNumber = 1;

    int boardIndex = 0;

    // Parse board setup
    while (*fen && boardIndex < 64) {
        if (*fen == '/') {
            fen++;
        } else if (isdigit(*fen)) {
            boardIndex += *fen - '0';
            fen++;
        } else {
            char piece = *fen++;
            int value = 0;

            switch (piece) {
                case 'p': value = -1; break;
                case 'n': value = -2; break;
                case 'b': value = -3; break;
                case 'r': value = -5; break;
                case 'q': value = -9; break;
                case 'k': value = -100; break;
                case 'P': value = 1; break;
                case 'N': value = 2; break;
                case 'B': value = 3; break;
                case 'R': value = 5; break;
                case 'Q': value = 9; break;
                case 'K': value = 100; break;
            }
            if (value != 0 && boardIndex < 64) {
                gamedata->chessPieces[boardIndex++] = value;
            }
        }
    }

    // Parse turn
    if (*fen == ' ') {
        fen++;
        gamedata->isWhiteTurn = (*fen == 'w');
        fen++;
    }

    // Parse castling rights
    if (*fen == ' ') {
        fen++;
        while (*fen != ' ' && *fen != '\0') {
            switch (*fen) {
                case 'K': gamedata->castlingRights[0] = true; break;
                case 'Q': gamedata->castlingRights[1] = true; break;
                case 'k': gamedata->castlingRights[2] = true; break;
                case 'q': gamedata->castlingRights[3] = true; break;
            }
            fen++;
        }
    }

    // Parse en passant
    if (*fen == ' ') {
        fen++;
        if (*fen != '-') {
            int file = fen[0] - 'a';
            int rank = fen[1] - '1';
            gamedata->enPassant = rank * 8 + file;
            fen += 2;
        } else {
            fen++;
        }
    }

    // Parse half-move clock
    if (*fen == ' ') {
        fen++;
        gamedata->halfMoveClock = atoi(fen);
        while (isdigit(*fen)) fen++;
    }

    // Parse full-move number
    if (*fen == ' ') {
        fen++;
        gamedata->fullMoveNumber = atoi(fen);
    }
}

void saveFEN(const GameData *gamedata, char *fen) {
    int boardIndex = 0;
    
    // Zapisz układ figur na planszy
    for (int row = 0; row < 8; row++) {
        int emptyCount = 0;
        for (int col = 0; col < 8; col++) {
            int piece = gamedata->chessPieces[boardIndex++];
            if (piece == 0) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    *fen++ = '0' + emptyCount;  // Zapisz ilość pustych pól
                    emptyCount = 0;
                }
                // Zapisz figurę
                switch (piece) {
                    case -1: *fen++ = 'p'; break;
                    case -2: *fen++ = 'n'; break;
                    case -3: *fen++ = 'b'; break;
                    case -5: *fen++ = 'r'; break;
                    case -9: *fen++ = 'q'; break;
                    case -100: *fen++ = 'k'; break;
                    case 1: *fen++ = 'P'; break;
                    case 2: *fen++ = 'N'; break;
                    case 3: *fen++ = 'B'; break;
                    case 5: *fen++ = 'R'; break;
                    case 9: *fen++ = 'Q'; break;
                    case 100: *fen++ = 'K'; break;
                }
            }
        }
        if (emptyCount > 0) {
            *fen++ = '0' + emptyCount;  // Zapisz ilość pustych pól
        }
        if (row < 7) {
            *fen++ = '/';  // Rozdziel wiersze planszy
        }
    }

    // Zapisz turę
    *fen++ = ' ';
    *fen++ = gamedata->isWhiteTurn ? 'w' : 'b';
    
    // Zapisz prawa do roszady
    *fen++ = ' ';
    bool hasCastlingRights = false;
    if (gamedata->castlingRights[0]) { *fen++ = 'K'; hasCastlingRights = true; }
    if (gamedata->castlingRights[1]) { *fen++ = 'Q'; hasCastlingRights = true; }
    if (gamedata->castlingRights[2]) { *fen++ = 'k'; hasCastlingRights = true; }
    if (gamedata->castlingRights[3]) { *fen++ = 'q'; hasCastlingRights = true; }
    if (!hasCastlingRights) { *fen++ = '-'; }

    // Zapisz en passant
    *fen++ = ' ';
    if (gamedata->enPassant == -1) {
        *fen++ = '-';
    } else {
        int rank = gamedata->enPassant / 8;
        int file = gamedata->enPassant % 8;
        *fen++ = 'a' + file;
        *fen++ = '1' + rank;
    }

    // Zapisz licznik półruchów
    *fen++ = ' ';
    sprintf(fen, "%d", gamedata->halfMoveClock);
    while (*fen) fen++;

    // Zapisz numer pełnego ruchu
    *fen++ = ' ';
    sprintf(fen, "%d", gamedata->fullMoveNumber);
    while (*fen) fen++;
}
