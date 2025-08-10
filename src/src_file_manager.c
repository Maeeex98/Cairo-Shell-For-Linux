#include <gtk/gtk.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "file_manager.h"

void show_file_manager_window(const char *dir) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "File Manager");
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

    GtkWidget *list = gtk_list_box_new();
    gtk_window_set_child(GTK_WINDOW(win), list);

    DIR *d = opendir(dir);
    if (d) {
        struct dirent *entry;
        while ((entry = readdir(d))) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            GtkWidget *row = gtk_label_new(entry->d_name);
            gtk_list_box_append(GTK_LIST_BOX(list), row);
        }
        closedir(d);
    }

    gtk_window_present(GTK_WINDOW(win));
}