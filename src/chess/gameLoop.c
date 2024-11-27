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
            move_piece(grid, index);  
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
        if(!is_black_piece(&gameData,index))
        {
            move_piece(grid, index);  
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

void move_piece(GtkWidget *grid, int index)
{
    int col = index % 8, row = index / 8;
    int p_row = prev_selected.row, p_col = prev_selected.col;
    int p_index = p_row * 8 + p_col;

    int type = get_piece_type(&gameData, p_index);
    if (type == -1) {
        g_warning("Error: No piece found at previous position (%d). Move aborted.", p_index);
        return;
    }
    uint64_t mask = ~get_index_mask(p_index);
    gameData.chessPieces.pieces[type] &= mask;
    mask = get_index_mask(index);
    gameData.chessPieces.pieces[type] |= mask;

    // Usuń grafikę figury z poprzedniego pola
    GtkWidget *prev_button = gtk_grid_get_child_at(GTK_GRID(grid), p_col, p_row);
    if (prev_button != NULL && GTK_IS_BUTTON(prev_button)) {
        draw_piece(prev_button, ' ');        
    } 
    else 
    {
        g_warning("No valid button at previous position (%d, %d).", p_col, p_row);
    }

    // Dodaj figurę na nowe pole
    GtkWidget *new_button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);
    if (new_button != NULL && GTK_IS_BUTTON(new_button)) 
    {
        draw_piece(new_button, pieceSymbols[type]);
    } 
    else 
    {
        g_warning("No valid button at new position (%d, %d).", col, row);
    }
}

void capture_piece(GtkWidget *grid, int index)
{
    int col = index % 8, row = index / 8;
    int p_row = prev_selected.row, p_col = prev_selected.col;
    int p_index = p_row * 8 + p_col;

    int type = get_piece_type(&gameData, p_index);
    if (type == -1) {
        g_warning("Error: No piece found at previous position (%d). Move aborted.", p_index);
        return;
    }

    int captured_piece_type = get_piece_type(&gameData, index);
        if (captured_piece_type == -1) {
        g_warning("Error: No piece found at capturet position (%d). Move aborted.", index);
        return;
    }


    uint64_t mask = ~get_index_mask(p_index);
    gameData.chessPieces.pieces[type] &= mask;
    mask = get_index_mask(index);
    gameData.chessPieces.pieces[type] |= mask;

    gamedata.chessPieces.pieces[captured_piece_type] &= ~mask;

    // Usuń grafikę figury z poprzedniego pola
    GtkWidget *prev_button = gtk_grid_get_child_at(GTK_GRID(grid), p_col, p_row);
    if (prev_button != NULL && GTK_IS_BUTTON(prev_button)) {
        draw_piece(prev_button, ' ');        
    } 
    else 
    {
        g_warning("No valid button at previous position (%d, %d).", p_col, p_row);
    }

    // Dodaj figurę na nowe pole
    GtkWidget *new_button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);
    if (new_button != NULL && GTK_IS_BUTTON(new_button)) 
    {
        draw_piece(new_button, pieceSymbols[type]);
    } 
    else 
    {
        g_warning("No valid button at new position (%d, %d).", col, row);
    }
}

void en_passant(GtkWidget *grid, int index);

void castle(GtkWidget *grid, int king_index, int rook_index)
{
    int col = index % 8, row = index / 8;
    int p_row = prev_selected.row, p_col = prev_selected.col;

    if(p_row == 7)
    {
        // o-o
        if(col == 0)
        {

        }
        // o-O-o
        else
        {

        }
    }
    else
    {
        // O-O
        if(col == 0)
        {

        }
        // O-o-O
        else
        {
            
        }
    }
    uint64_t mask = get_index_mask(king_index);
    gameData.chessPieces.pieces[rook_type] |= mask;
    gameData.chessPieces.pieces[king_type] &= ~mask;

    mask = get_index_mask(rook_index);
    gameData.chessPieces.pieces[king_type] |= mask;
    gameData.chessPieces.pieces[rook_type] &= ~mask;

    gamedata.chessPieces.pieces[captured_piece_type] &= ~mask;

    // Usuń grafikę figury z poprzedniego pola
    GtkWidget *prev_button = gtk_grid_get_child_at(GTK_GRID(grid), p_col, p_row);
    if (prev_button != NULL && GTK_IS_BUTTON(prev_button)) {
        draw_piece(prev_button, ' ');        
    } 
    else 
    {
        g_warning("No valid button at previous position (%d, %d).", p_col, p_row);
    }

    // Dodaj figurę na nowe pole
    GtkWidget *new_button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);
    if (new_button != NULL && GTK_IS_BUTTON(new_button)) 
    {
        draw_piece(new_button, pieceSymbols[type]);
    } 
    else 
    {
        g_warning("No valid button at new position (%d, %d).", col, row);
    }
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

    int indices[64];
    size_t count;

    for (int i = 0; i < 12; i++) 
    {
        // Pobierz indeksy figur na planszy dla danego typu
        get_pieces_indices(gameData.chessPieces.pieces[i], indices, &count);

        for (size_t j = 0; j < count; j++) 
        {
            int index = indices[j];
            int col = index % 8;
            int row = index / 8;
            //g_print("%d,%d, piece:%d\n",row,col,i);
            // Pobierz przycisk z siatki
            GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(grid), col, row);

            // Sprawdź, czy widget istnieje
            if (button == NULL) 
            {
                g_warning("No widget found at grid position (%d, %d). Skipping...", col, row);
                continue;
            }

            // Sprawdź, czy widget jest przyciskiem
            if (!GTK_IS_BUTTON(button)) 
            {
                g_warning("Widget at grid position (%d, %d) is not a GtkButton. Skipping...", col, row);
                continue;
            }

            // Narysuj figurę
            draw_piece(button, pieceSymbols[i]);
        }
    }
}
