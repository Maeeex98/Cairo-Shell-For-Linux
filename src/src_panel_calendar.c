#include <gtk/gtk.h>

void on_calendar_btn_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *popover = GTK_WIDGET(user_data);
    gtk_popover_popup(GTK_POPOVER(popover));
}

void add_calendar_button_to_panel(GtkBox *panel_box) {
    GtkWidget *calendar_btn = gtk_button_new_with_label("📅");
    GtkWidget *popover = gtk_popover_new(calendar_btn);
    GtkWidget *calendar = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendar);

    g_signal_connect(calendar_btn, "clicked", G_CALLBACK(on_calendar_btn_clicked), popover);

    gtk_box_append(panel_box, calendar_btn);
}