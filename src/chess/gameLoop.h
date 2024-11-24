#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <gtk/gtk.h>
#include "FEN_Parser.h"
#include "bitboards.h"


typedef struct {
    int row;
    int col;
} SquareCords;
typedef enum {
    WHITE_SELECTING_PIECE,
    WHITE_MOVING_PIECE,
    WHITE_IN_CHECK,

    BLACK_SELECTING_PIECE,
    BLACK_MOVING_PIECE,
    BLACK_IN_CHECK
} GameState;



static GameData gameData;
static SquareCords prev_selected;
static GameState current_state;

GtkWidget *draw_board(void);
void start_game(char *fen);

void on_square_clicked(GtkButton *button, gpointer user_data);

void select_squere(GtkWidget *grid, GtkButton *button, int row, int col);
void remove_selection(GtkWidget *grid);
void move_piece(int index);

void draw_piece(GtkWidget *button, char piece);
void draw_pieces(GtkWidget *grid);

#endif
