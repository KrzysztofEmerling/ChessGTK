#include <gtk/gtk.h>
#include "chess.h"


static GameData gameData;
static SquareCords prev_selected = {-1, -1};
static GameState current_state = WHITE_SELECTING_PIECE;
 

GtkWidget *draw_board(void) {
    GtkWidget *grid = gtk_grid_new();

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            GtkWidget *button = gtk_button_new();
            gtk_widget_set_size_request(button, 100, 100);
            // Dodanie klasy CSS do przycisku
            if ((row + col) % 2) 
            {
                gtk_widget_add_css_class(button, "square1");
            } 
            else 
            {
                gtk_widget_add_css_class(button, "square0");
            }

            SquareCords *coords = g_new(SquareCords, 1);
            coords->row = row;
            coords->col = col;

            // Przypisanie współrzędnych do przycisku jako user_data
            g_signal_connect(button, "clicked", G_CALLBACK(on_square_clicked), coords);

            // Dodanie przycisku do siatki
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        }
    }

    return grid;
}


void on_square_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(button));

    SquareCords *coords = (SquareCords *)user_data; 
    int row = coords->row;
    int col = coords->col;
    int index = row * 8 + col;

    switch (current_state)
    {
    case WHITE_SELECTING_PIECE:
        remove_selection(grid);
        if(gameData.chessPieces[index] > 0)
        {
            select_squere(grid, button, row, col);
            current_state = WHITE_MOVING_PIECE;
        }
        break;

    case WHITE_MOVING_PIECE:
        if(gameData.chessPieces[index] <= 0)
        {
            move_piece(index);  
            draw_pieces(grid);   
            remove_selection(grid);

            current_state = BLACK_SELECTING_PIECE;
        }
        else
        {
            remove_selection(grid);
            current_state = WHITE_SELECTING_PIECE;
        }
        break;

    case BLACK_SELECTING_PIECE:
        if(gameData.chessPieces[index] < 0)
        {
            select_squere(grid, button, row, col);
            current_state = BLACK_MOVING_PIECE;
        }
        else
        {
            remove_selection(grid);
        }
        break;

    case BLACK_MOVING_PIECE:
        if(gameData.chessPieces[index] >= 0)
        {
            move_piece(index);  
            draw_pieces(grid);   
            current_state = WHITE_SELECTING_PIECE;
        }
        else
        {
            remove_selection(grid);
            current_state = BLACK_SELECTING_PIECE;
        }
        break;
    
    default:
        break;
    }
}


void select_squere(GtkWidget *grid, GtkButton *button, int row, int col) {

    if (prev_selected.row != -1) 
    {
        GtkWidget *prev_button = gtk_grid_get_child_at(GTK_GRID(grid), prev_selected.col, prev_selected.row);
        if (prev_button) 
        {
            gtk_widget_remove_css_class(prev_button, "selected");
        }
    }

    gtk_widget_add_css_class(GTK_WIDGET(button), "selected");
    prev_selected.row = row;
    prev_selected.col = col;
}

void remove_selection(GtkWidget *grid) {

    if (prev_selected.row != -1) 
    {
        GtkWidget *prev_button = gtk_grid_get_child_at(GTK_GRID(grid), prev_selected.col, prev_selected.row);
        if (prev_button) 
        {
            gtk_widget_remove_css_class(prev_button, "selected");
        }
    }
    prev_selected.row = -1;
}



void move_piece(int index)
{
    int p_index = prev_selected.row * 8 + prev_selected.col;
    gameData.chessPieces[index] = gameData.chessPieces[p_index];
    gameData.chessPieces[p_index] = 0;
}


void draw_piece(GtkWidget *button, int piece) {
    const char *chessSymbol = NULL;
    bool blackPiece = piece < 0;
    if (blackPiece) piece *= -1;

    switch (piece) {
        case 1: chessSymbol = "♟"; break; // Pion
        case 2: chessSymbol = "♞"; break; // Skoczek
        case 3: chessSymbol = "♝"; break; // Goniec
        case 5: chessSymbol = "♜"; break; // Wieża
        case 9: chessSymbol = "♛"; break; // Królowa
        case 100: chessSymbol = "♚"; break; // Król
        default: chessSymbol = ""; break;
    }

    if (chessSymbol != NULL) {
        GtkWidget *label = gtk_label_new(chessSymbol);

        // Dodaj klasę CSS do etykiety
        if(blackPiece) gtk_widget_add_css_class(label, "chess-piece-black");
        else gtk_widget_add_css_class(label, "chess-piece-white");

        // Ustaw etykietę jako dziecko przycisku
        gtk_button_set_child(GTK_BUTTON(button), label);
    }
}


void draw_pieces(GtkWidget *grid) {
    int index = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);

            // Jeśli przycisk istnieje
            if (button != NULL && index < 64) 
            {
                if (gameData.chessPieces[index] == 0) 
                {
                    GtkWidget *child = gtk_button_get_child(GTK_BUTTON(button));
                    if (child != NULL) 
                    {
                        if (GTK_IS_LABEL(child)) 
                        {
                            gtk_label_set_text(GTK_LABEL(child), " ");
                        }
                    }
                } 
                else 
                {
                    draw_piece(button, gameData.chessPieces[index]);
                }
            }

            index++;
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void parseFEN(const char *fen) {
    int index = 0; // Indeks w tablicy 64-elementowej
    for (int i = 0; fen[i] != '\0' && index < 64; i++) {
        char c = fen[i];

        if (isdigit(c)) {
            // Puste pola: dodaj tyle zer, ile wynosi cyfra
            int emptySpaces = c - '0';
            for (int j = 0; j < emptySpaces; j++) {
                gameData.chessPieces[index++] = 0;
            }
        } else if (c == '/') {
            // Separator między rzędami - ignorujemy
            continue;
        } else {
            // Figury
            switch (c) {
                case 'P': gameData.chessPieces[index++] = 1; break;
                case 'N': gameData.chessPieces[index++] = 2; break;
                case 'B': gameData.chessPieces[index++] = 3; break;
                case 'R': gameData.chessPieces[index++] = 5; break;
                case 'Q': gameData.chessPieces[index++] = 9; break;
                case 'K': gameData.chessPieces[index++] = 100; break;
                case 'p': gameData.chessPieces[index++] = -1; break;
                case 'n': gameData.chessPieces[index++] = -2; break;
                case 'b': gameData.chessPieces[index++] = -3; break;
                case 'r': gameData.chessPieces[index++] = -5; break;
                case 'q': gameData.chessPieces[index++] = -9; break;
                case 'k': gameData.chessPieces[index++] = -100; break;
                default: break; // Ignoruj nieznane znaki
            }
        }
    }
}