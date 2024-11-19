#include <gtk/gtk.h>
#include <stdlib.h>  // Dla realpath

static GtkCssProvider *css_provider = NULL;

void load_css(const char *css_file) {
    if (css_provider == NULL) {
        css_provider = gtk_css_provider_new();
    }

    GdkDisplay *display = gdk_display_get_default();

    // Sprawdzenie, czy plik CSS istnieje
    if (g_file_test(css_file, G_FILE_TEST_EXISTS)) {
        // Wypisanie absolutnej ścieżki do pliku CSS
        char *absolute_path = realpath(css_file, NULL);
        if (absolute_path != NULL) {
            //g_print("Loading CSS file: %s\n", absolute_path);
            free(absolute_path);  // Zwolnienie pamięci
        } else {
            g_warning("Failed to get absolute path for: %s\n", css_file);
        }

        // Załadowanie pliku CSS
        gtk_css_provider_load_from_path(css_provider, css_file);

        // Dodajemy styl CSS do aplikacji
        gtk_style_context_add_provider_for_display(
            display, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    } else {
        g_warning("CSS file does not exist: %s", css_file);
    }
}

void on_theme_changed(GObject *settings, GParamSpec *pspec, gpointer user_data) {
    GtkSettings *gtk_settings = GTK_SETTINGS(settings);
    char *theme_name = NULL;

    g_object_get(gtk_settings, "gtk-theme-name", &theme_name, NULL);

    if (theme_name) {
        //g_print("System theme changed to: %s\n", theme_name);

        if (g_str_has_suffix(theme_name, "dark"))
        {
            load_css("themes/dark.css");  // Załaduj ciemny motyw
        } 
        else 
        {
            load_css("themes/light.css");  // Załaduj jasny motyw
        }

        g_free(theme_name);
    }
}
