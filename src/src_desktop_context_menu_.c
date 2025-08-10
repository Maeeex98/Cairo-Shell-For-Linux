#include <gtk/gtk.h>
#include <stdlib.h>

static void on_create_folder(GtkMenuItem *item, gpointer user_data) {
    system("mkdir ~/Desktop/Новая_папка");
}

static void on_create_shortcut(GtkMenuItem *item, gpointer user_data) {
    system("touch ~/Desktop/Ярлык.desktop");
}

static void on_delete_selected(GtkMenuItem *item, gpointer user_data) {
    // Для примера: удаляем выделенное (user_data — путь)
    char *filepath = (char*)user_data;
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "rm -rf '%s'", filepath);
    system(cmd);
}

void show_desktop_context_menu(GtkWidget *widget, GdkEventButton *event, const char *selected_path) {
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *create_folder = gtk_menu_item_new_with_label("Создать папку");
    GtkWidget *create_shortcut = gtk_menu_item_new_with_label("Создать ярлык");
    GtkWidget *delete = gtk_menu_item_new_with_label("Удалить");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), create_folder);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), create_shortcut);
    if (selected_path)
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete);

    g_signal_connect(create_folder, "activate", G_CALLBACK(on_create_folder), NULL);
    g_signal_connect(create_shortcut, "activate", G_CALLBACK(on_create_shortcut), NULL);
    if (selected_path)
        g_signal_connect(delete, "activate", G_CALLBACK(on_delete_selected), (gpointer)selected_path);

    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*)event);
}