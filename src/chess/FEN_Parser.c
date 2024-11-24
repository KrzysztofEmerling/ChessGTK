#include <ctype.h>
#include "FEN_Parser.h"


void parseFEN(const char *fen, GameData *gamedata) {
    // Reset gamedata to default values
    memset(gamedata->chessPieces.pieces, 0, sizeof(gamedata->chessPieces.pieces));
    memset(gamedata->castlingRights, false, sizeof(gamedata->castlingRights));
    gamedata->isWhiteTurn = true;
    gamedata->enPassant = 0; // 0 means no en passant
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
            switch (piece) {
                case 'P': 
                    gamedata->chessPieces.WhitePawns += get_index_mask(boardIndex);
                    break;
                case 'p': 
                    gamedata->chessPieces.BlackPawns += get_index_mask(boardIndex);
                    break;

                case 'N': 
                    gamedata->chessPieces.WhiteKnights += get_index_mask(boardIndex);
                    break;
                case 'n':                     
                    gamedata->chessPieces.BlackKnights += get_index_mask(boardIndex);
                    break;

                case 'B':              
                    gamedata->chessPieces.WhiteBishops += get_index_mask(boardIndex);
                    break;
                case 'b': 
                    gamedata->chessPieces.BlackBishops += get_index_mask(boardIndex);
                    break;

                case 'R':
                    gamedata->chessPieces.WhiteRooks += get_index_mask(boardIndex);
                    break;
                case 'r': 
                    gamedata->chessPieces.BlackRooks += get_index_mask(boardIndex);
                    break;

                case 'Q':
                    gamedata->chessPieces.WhiteQueens += get_index_mask(boardIndex);
                    break;
                case 'q': 
                    gamedata->chessPieces.BlackQueens += get_index_mask(boardIndex);
                    break;

                case 'K':
                    gamedata->chessPieces.WhiteKing += get_index_mask(boardIndex);
                    break;
                case 'k':
                    gamedata->chessPieces.BlackKing += get_index_mask(boardIndex);
                    break;
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
    static const char pieceSymbols[12] = {
        'P', 'R', 'B', 'N', 'Q', 'K',  // Białe figury
        'p', 'r', 'b', 'n', 'q', 'k'   // Czarne figury
    };

    for (int row = 0; row < 8; row++) {
        int emptyCount = 0;

        for (int col = 0; col < 8; col++) {
            int index = row * 8 + col; // Indeks pola
            char pieceChar = 0;

            // Szukamy figury na danym polu w bitboardach
            for (int i = 0; i < 12; i++) {
                if (gamedata->chessPieces.pieces[i] & get_index_mask(index)) {
                    pieceChar = pieceSymbols[i];
                    break; // Znaleziono figurę
                }
            }

            if (pieceChar == 0) {
                // Pole jest puste
                emptyCount++;
            } else {
                // Jeśli są puste pola, zapisz ich liczbę
                if (emptyCount > 0) {
                    *fen++ = '0' + emptyCount;
                    emptyCount = 0;
                }
                // Zapisz figurę
                *fen++ = pieceChar;
            }
        }

        // Jeśli na końcu wiersza są puste pola, zapisz ich liczbę
        if (emptyCount > 0) {
            *fen++ = '0' + emptyCount;
        }

        // Dodaj separator wierszy
        if (row < 7) {
            *fen++ = '/';
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
    if (gamedata->enPassant == 0) {
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
