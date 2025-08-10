#include <gtk/gtk.h>
void on_theme_toggle(GtkButton *btn, gpointer user_data) {
    static gboolean dark = FALSE;
    dark = !dark;
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", dark, NULL);
}

void add_theme_toggle(GtkBox *panel_box) {
    GtkWidget *btn = gtk_button_new_with_label("🌗");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_theme_toggle), NULL);
    gtk_box_append(panel_box, btn);
}