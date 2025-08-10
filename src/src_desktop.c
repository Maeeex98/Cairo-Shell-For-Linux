#include <gtk/gtk.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define DESKTOP_DIR "/home/USERNAME/Desktop"
#define SHELL_DESKTOP_DIR "/home/USERNAME/.local/share/your-shell/desktop"
#define MIGRATED_FLAG "/home/USERNAME/.local/share/your-shell/desktop/.migrated"

void first_run_migrate_files() {
    struct stat st;
    if (stat(MIGRATED_FLAG, &st) == 0) return;
    mkdir(SHELL_DESKTOP_DIR, 0755);

    DIR *d = opendir(DESKTOP_DIR);
    if (!d) return;
    struct dirent *entry;
    while ((entry = readdir(d))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        char src[PATH_MAX], dst[PATH_MAX];
        snprintf(src, sizeof(src), "%s/%s", DESKTOP_DIR, entry->d_name);
        snprintf(dst, sizeof(dst), "%s/%s", SHELL_DESKTOP_DIR, entry->d_name);
        rename(src, dst);
    }
    closedir(d);
    FILE *f = fopen(MIGRATED_FLAG, "w");
    if (f) fclose(f);
}

void open_file(const char *filepath) {
    char cmd[PATH_MAX + 20];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\" &", filepath);
    system(cmd);
}

// DnD: обработка drop внешних файлов
static void on_drop_cb(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data) {
    if (!G_VALUE_HOLDS(value, G_TYPE_FILE)) return;
    GFile *file = g_value_get_object(value);
    char *src_path = g_file_get_path(file);
    const char *filename = g_file_get_basename(file);

    char dst_path[PATH_MAX];
    snprintf(dst_path, sizeof(dst_path), "%s/%s", SHELL_DESKTOP_DIR, filename);
    g_file_copy(file, g_file_new_for_path(dst_path), G_FILE_COPY_NONE, NULL, NULL, NULL, NULL);

    g_free(src_path);
}

// DnD: обработка drag внутри desktop
static GdkContentProvider* on_drag_prepare(GtkWidget *widget, double x, double y, gpointer user_data) {
    GtkIconView *icon_view = GTK_ICON_VIEW(widget);
    int cell = gtk_icon_view_get_item_at_pos(icon_view, (int)x, (int)y);
    if (cell < 0) return NULL;
    GtkTreePath *path = gtk_icon_view_get_path_at_pos(icon_view, (int)x, (int)y);
    GtkTreeModel *model = gtk_icon_view_get_model(icon_view);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        char *filename = NULL;
        gtk_tree_model_get(model, &iter, 1, &filename, -1); // column 1: filename
        GFile *file = g_file_new_for_path(filename);
        GdkContentProvider *provider = gdk_content_provider_new_for_value(g_value_init_with_object(file, G_TYPE_FILE));
        g_object_unref(file);
        g_free(filename);
        return provider;
    }
    return NULL;
}

void create_icon_view(GtkWidget *parent_box) {
    GtkWidget *icon_view = gtk_icon_view_new();
    GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    DIR *d = opendir(SHELL_DESKTOP_DIR);
    if (d) {
        struct dirent *entry;
        while ((entry = readdir(d))) {
            if (entry->d_name[0] == '.') continue;
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, entry->d_name, 1, entry->d_name, -1);
        }
        closedir(d);
    }
    gtk_icon_view_set_model(GTK_ICON_VIEW(icon_view), GTK_TREE_MODEL(store));
    gtk_icon_view_set_text_column(GTK_ICON_VIEW(icon_view), 0);
    gtk_box_append(GTK_BOX(parent_box), icon_view);

    // DnD: Drag source (внутренний dnd)
    GtkDragSource *drag_source = gtk_drag_source_new();
    g_signal_connect(icon_view, "drag-prepare", G_CALLBACK(on_drag_prepare), NULL);
    gtk_widget_add_controller(icon_view, GTK_EVENT_CONTROLLER(drag_source));

    // DnD: Drop target для внешних файлов
    GtkDropTarget *drop_target = gtk_drop_target_new(G_TYPE_FILE, GDK_ACTION_COPY);
    g_signal_connect(drop_target, "drop", G_CALLBACK(on_drop_cb), NULL);
    gtk_widget_add_controller(icon_view, GTK_EVENT_CONTROLLER(drop_target));

    // Открытие файла по двойному клику
    g_signal_connect(icon_view, "item-activated", G_CALLBACK(
        [](GtkIconView *view, GtkTreePath *path, gpointer user_data){
            GtkTreeIter iter;
            GtkTreeModel *model = gtk_icon_view_get_model(view);
            if (gtk_tree_model_get_iter(model, &iter, path)) {
                char *filename = NULL;
                gtk_tree_model_get(model, &iter, 1, &filename, -1);
                char fullpath[PATH_MAX];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", SHELL_DESKTOP_DIR, filename);
                open_file(fullpath);
                g_free(filename);
            }
        }), NULL);
}

int main(int argc, char *argv[]) {
    gtk_init();
    first_run_migrate_files();

    GtkWidget *win = gtk_window_new();
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(win), box);

    create_icon_view(box);

    gtk_window_present(GTK_WINDOW(win));
    gtk_main();
    return 0;
}