#include <gtk/gtk.h>
#include <stdlib.h>

static void on_power_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *shutdown = gtk_menu_item_new_with_label("Выключить");
    GtkWidget *reboot = gtk_menu_item_new_with_label("Перезагрузить");
    GtkWidget *logout = gtk_menu_item_new_with_label("Выйти");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), shutdown);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), reboot);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), logout);
    gtk_widget_show_all(menu);

    g_signal_connect(shutdown, "activate", G_CALLBACK(+[](GtkMenuItem*, gpointer){
        system("poweroff");
    }), NULL);
    g_signal_connect(reboot, "activate", G_CALLBACK(+[](GtkMenuItem*, gpointer){
        system("reboot");
    }), NULL);
    g_signal_connect(logout, "activate", G_CALLBACK(+[](GtkMenuItem*, gpointer){
        system("pkill -KILL -u $USER");
    }), NULL);

    gtk_menu_popup_at_widget(GTK_MENU(menu), GTK_WIDGET(btn), GDK_GRAVITY_SOUTH, GDK_GRAVITY_NORTH, NULL);
}

void add_power_button(GtkBox *panel_box) {
    GtkWidget *btn = gtk_button_new_with_label("⏻");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_power_clicked), NULL);
    gtk_box_append(panel_box, btn);
}