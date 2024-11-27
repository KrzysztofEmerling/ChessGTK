#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include <gtk/gtk.h>
#include "bitboards.h"



void parseFEN(const char *fen, GameData *gamedata);
void saveFEN(const GameData *gamedata, char *fen);

#endif