#include <gtk/gtk.h>

// Открытие календаря в отдельном окне
void show_calendar_window(GtkButton *btn, gpointer user_data) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "Календарь");
    gtk_window_set_default_size(GTK_WINDOW(win), 280, 250);

    GtkWidget *calendar = gtk_calendar_new();
    gtk_window_set_child(GTK_WINDOW(win), calendar);

    gtk_window_present(GTK_WINDOW(win));
}

// Добавление кнопки с календарём на панель
void add_calendar_button(GtkBox *box) {
    GtkWidget *btn = gtk_button_new_with_label("📅");
    g_signal_connect(btn, "clicked", G_CALLBACK(show_calendar_window), NULL);
    gtk_box_append(box, btn);
}