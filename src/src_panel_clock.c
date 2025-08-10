#include <gtk/gtk.h>
#include <time.h>

static gboolean update_time(GtkLabel *label) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%H:%M:%S", tm);
    gtk_label_set_text(label, buf);
    return G_SOURCE_CONTINUE;
}

void add_clock_to_panel(GtkBox *panel_box) {
    GtkWidget *clock_label = gtk_label_new("");
    gtk_box_append(panel_box, clock_label);
    g_timeout_add_seconds(1, (GSourceFunc)update_time, clock_label);
}