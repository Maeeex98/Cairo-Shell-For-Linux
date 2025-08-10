#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define GAME_MODE_FLAG "~/.local/share/your-shell/disable_gamemode"

int steam_installed() {
    return access("/usr/bin/steam", X_OK) == 0
        || access("/usr/local/bin/steam", X_OK) == 0;
}

gboolean gamemode_permanently_disabled() {
    char *flag_path = g_strdup_printf("%s", GAME_MODE_FLAG);
    gboolean exists = access(flag_path, F_OK) == 0;
    g_free(flag_path);
    return exists;
}

void disable_gamemode_permanently() {
    char *flag_path = g_strdup_printf("%s", GAME_MODE_FLAG);
    FILE *f = fopen(flag_path, "w");
    if (f) fclose(f);
    g_free(flag_path);
}

void on_game_mode_clicked(GtkButton *btn, gpointer user_data) {
    if (steam_installed()) {
        // Запуск Steam в Big Picture
        system("steam -tenfoot &"); // -tenfoot это Big Picture Mode
    } else {
        // Диалог установки Steam
        GtkWidget *dialog = gtk_message_dialog_new(
            NULL, 0, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
            "Steam не установлен. Установить сейчас?"
        );
        int result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_window_destroy(GTK_WINDOW(dialog));
        if (result == GTK_RESPONSE_YES) {
            // Попытаться установить Steam (например, через xdg-open)
            system("xdg-open https://store.steampowered.com/about/");
        } else {
            disable_gamemode_permanently();
            // Можно показать сообщение: "Game Mode больше не будет отображаться"
        }
    }
}

void add_game_mode_button(GtkBox *panel_box) {
    if (gamemode_permanently_disabled())
        return; // Не показывать кнопку, если был отказ
    GtkWidget *btn = gtk_button_new_with_label("🎮 Game Mode");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_game_mode_clicked), NULL);
    gtk_box_append(panel_box, btn);
}