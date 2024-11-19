#ifndef CHESS_H
#define CHESS_H

#include <gtk/gtk.h>

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

typedef struct {
    int chessPieces[64]; 
    bool castlingRights[4];  // [0] - biała krutka, [1] - biała długa, [2] - czarna krutka, [3] - czarna długa.
    bool isWhiteTurn;     
    char enPassant[3];   // [0] - linia upowarznionego piona, [1] - linia 1 możliwego do zbicia piona, [2] - linia 2 ewentualnego możliwego bicia
    int halfMoveClock;   // Licznik półruchów
    int fullMoveNumber;  // Numer pełnego ruchu
} GameData;
static GameData gameData;
static SquareCords prev_selected;
static GameState current_state;


void select_squere(GtkWidget *grid, GtkButton *button, int row, int col);
void remove_selection(GtkWidget *grid);
void move_piece(int index);

void draw_pieces(GtkWidget *grid);
void draw_piece(GtkWidget *button, int piece);
void on_square_clicked(GtkButton *button, gpointer user_data);
GtkWidget *draw_board(void);
#endif // CHESS_H
