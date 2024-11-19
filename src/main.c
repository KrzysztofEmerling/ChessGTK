#include <gtk/gtk.h>
#include "ui.h"
#include "theme.h"
#include "chess.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("my.simple.chess.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
