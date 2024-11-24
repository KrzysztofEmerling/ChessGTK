#include <gtk/gtk.h>

#include "gameLoop.h"

static GameData gameData;
static SquareCords prev_selected = {-1, -1};
static GameState current_state = WHITE_SELECTING_PIECE;
 
void start_game(char *fen)
{
    parseFEN(fen, &gameData);
    if(gameData.isWhiteTurn)
    {
        current_state = WHITE_SELECTING_PIECE;
    }
    else
    {
         current_state = BLACK_SELECTING_PIECE;
    }
}
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
        if(is_white_piece(&gameData,index))
        {
            select_squere(grid, button, row, col);
            current_state = WHITE_MOVING_PIECE;
        }
        break;

    case WHITE_MOVING_PIECE:
        if(!is_white_piece(&gameData,index))
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
        if(is_black_piece(&gameData,index))
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
        if(!is_white_piece(&gameData,index))
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
    int type = get_piece_type(&gameData, p_index);

    uint64_t mask = ~get_index_mask(p_index);
    gameData.chessPieces.pieces[type] &= mask;
    mask = get_index_mask(index);
    gameData.chessPieces.pieces[type] |= mask;
}


void draw_piece(GtkWidget *button, char piece) {
    const char *chessSymbol = NULL;
    bool black_piece = false;

    switch (piece) {
        case 'P': chessSymbol = "♟"; break; 
        case 'p':
            chessSymbol = "♟"; 
            black_piece = true;
            break; 

        case 'N': chessSymbol = "♞"; break; 
        case 'n': chessSymbol = "♞";
            black_piece = true;
            break; 

        case 'B': chessSymbol = "♝"; break; 
        case 'b': chessSymbol = "♝";
            black_piece = true;
            break; 

        case 'R': chessSymbol = "♜"; break; 
        case 'r': chessSymbol = "♜"; 
            black_piece = true;
            break; 
        case 'Q': chessSymbol = "♛"; break; 
        case 'q': chessSymbol = "♛"; 
            black_piece = true;
            break;  

        case 'K': chessSymbol = "♚"; break; 
        case 'k': chessSymbol = "♚"; 
            black_piece = true;
            break; 

        default: chessSymbol = " "; break;
    }

    if (chessSymbol != NULL) 
    {
        GtkWidget *label = gtk_label_new(chessSymbol);

        if(black_piece) gtk_widget_add_css_class(label, "chess-piece-black");
        else gtk_widget_add_css_class(label, "chess-piece-white");

        gtk_button_set_child(GTK_BUTTON(button), label);
    }
}

void draw_pieces(GtkWidget *grid) 
{
    static const char pieceSymbols[12] = {
        'P', 'R', 'B', 'N', 'Q', 'K',  // Białe figury
        'p', 'r', 'b', 'n', 'q', 'k'   // Czarne figury
    };

    int indices[64];
    size_t count;
    for(int i = 0; i < 12; i++)
    {
        get_pieces_indices(gameData.chessPieces.pieces[i], indices, &count);
        for(size_t j = 0; j < count; j++)
        {
            int index = indices[j];
            int col = index % 8;
            int row = (index - col) / 8;

            GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);
            draw_piece(button, pieceSymbols[i]);
        }
    }
}