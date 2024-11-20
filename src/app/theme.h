#ifndef THEME_H
#define THEME_H

#include <gtk/gtk.h>
//extern GtkCssProvider *css_provider;

void load_css(const char *css_file);
void on_theme_changed(GObject *settings, GParamSpec *pspec, gpointer user_data);

#endif // THEME_H
