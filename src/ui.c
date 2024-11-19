#include <gtk/gtk.h>
#include "ui.h"
#include "theme.h"
#include "chess.h"

void activate(GtkApplication *app, gpointer user_data) 
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *board;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "8x8 Board");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Tworzenie siatki 8x8 wywendruje do startGame();
    board = draw_board();
    gtk_box_append(GTK_BOX(vbox), board);

    draw_pieces(board);
    
    // Podłącz obserwatora motywu systemowego
    GtkSettings *settings = gtk_settings_get_default();
    g_signal_connect(settings, "notify::gtk-theme-name", G_CALLBACK(on_theme_changed), NULL);

    // Załaduj styl oparty na obecnym motywie systemowym
    on_theme_changed(G_OBJECT(settings), NULL, NULL);

    gtk_window_present(GTK_WINDOW(window));
}
