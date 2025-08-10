#include <gtk/gtk.h>
#include <stdlib.h>
#include "settings.h"
#include "settings_window.h"

static void on_app_clicked(GtkButton *btn, gpointer user_data) {
    const char *cmd = (const char*)user_data;
    if (cmd) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s &", cmd);
        system(buf);
    }
}

static void on_settings_clicked(GtkButton *btn, gpointer user_data) {
    AppSettings *settings = (AppSettings*)user_data;
    show_settings_window(settings);
}

static void on_power_clicked(GtkButton *btn, gpointer user_data) {
    system("poweroff");
}

static const struct {
    const char *name;
    const char *cmd;
} app_list[] = {
    {"Terminal", "gnome-terminal"},
    {"Files", "nautilus"},
    {"Web", "firefox"},
    {"Text Editor", "gedit"},
    {NULL, NULL}
};

void show_start_menu(GtkButton *btn, gpointer user_data) {
    GtkWidget *popover = gtk_popover_new();
    gtk_popover_set_has_arrow(GTK_POPOVER(popover), TRUE);
    gtk_popover_set_pointing_to(GTK_POPOVER(popover), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    for (int i = 0; app_list[i].name; ++i) {
        GtkWidget *app_btn = gtk_button_new_with_label(app_list[i].name);
        g_signal_connect(app_btn, "clicked", G_CALLBACK(on_app_clicked), (gpointer)app_list[i].cmd);
        gtk_box_append(GTK_BOX(vbox), app_btn);
    }

    gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
    GtkWidget *settings_btn = gtk_button_new_with_label("Settings");
    g_signal_connect(settings_btn, "clicked", G_CALLBACK(on_settings_clicked), user_data);
    gtk_box_append(GTK_BOX(vbox), settings_btn);

    GtkWidget *power_btn = gtk_button_new_with_label("Power Off");
    g_signal_connect(power_btn, "clicked", G_CALLBACK(on_power_clicked), NULL);
    gtk_box_append(GTK_BOX(vbox), power_btn);

    gtk_popover_set_child(GTK_POPOVER(popover), vbox);
    gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);
    gtk_popover_popup(GTK_POPOVER(popover));
    gtk_widget_show(popover);
}

void add_start_button(GtkBox *box, AppSettings *settings) {
    GtkWidget *btn = gtk_button_new_with_label("Пуск");
    g_signal_connect(btn, "clicked", G_CALLBACK(show_start_menu), settings);
    gtk_box_append(box, btn);
}