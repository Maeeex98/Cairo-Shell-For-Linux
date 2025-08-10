#include <gtk/gtk.h>
#include "settings.h"
void show_browser_window(); // из browser.c

int main(int argc, char *argv[]) {
    gtk_init();

    AppSettings settings;
    load_settings(&settings);

    GtkWidget *win = gtk_window_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_window_set_child(GTK_WINDOW(win), box);

    // Кнопка браузера
    if (settings.browser_enabled) {
        GtkWidget *browser_btn = gtk_button_new_with_label("🌐 Browser");
        g_signal_connect(browser_btn, "clicked", G_CALLBACK(
            [](GtkButton *btn, gpointer user_data){
                show_browser_window();
            }
        ), NULL);
        gtk_box_append(GTK_BOX(box), browser_btn);
    }

    gtk_window_present(GTK_WINDOW(win));
    gtk_main();
    return 0;
}