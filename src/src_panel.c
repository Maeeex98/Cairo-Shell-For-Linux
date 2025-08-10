#include <gtk/gtk.h>
#include <gtk-layer-shell/gtk-layer-shell.h>
#include <time.h>

static GtkLabel *clock_label = NULL;

static gboolean update_clock(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    char buf[9]; // HH:MM:SS\0
    strftime(buf, sizeof(buf), "%H:%M:%S", tm_struct);
    gtk_label_set_text(clock_label, buf);
    return TRUE; // Continue calling
}

int main(int argc, char *argv[]) {
    gtk_init();

    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Panel with Clock");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 40);

    gtk_layer_init_for_window(GTK_WINDOW(window));
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Часы
    clock_label = GTK_LABEL(gtk_label_new("00:00:00"));
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(clock_label));
    update_clock(NULL);
    g_timeout_add_seconds(1, update_clock, NULL);

    gtk_window_present(GTK_WINDOW(window));
    gtk_main();
    return 0;
}