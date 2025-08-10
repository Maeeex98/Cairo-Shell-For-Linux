#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static void on_volume_changed(GtkRange *range, gpointer user_data) {
    int volume = (int)gtk_range_get_value(range);
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "amixer set Master %d%%", volume);
    system(cmd);
}

void add_volume_slider(GtkBox *panel_box) {
    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_range_set_value(GTK_RANGE(slider), 50); // стартовое значение
    g_signal_connect(slider, "value-changed", G_CALLBACK(on_volume_changed), NULL);
    gtk_box_append(panel_box, slider);
}