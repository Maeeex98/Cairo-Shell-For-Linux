#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Проверка наличия Ollama
int ollama_installed() {
    return access("/usr/bin/ollama", X_OK) == 0
        || access("/usr/local/bin/ollama", X_OK) == 0;
}

// Запрос к Ollama через popen
static char* ask_ollama(const char* prompt) {
    // Модель по умолчанию (замени на нужную)
    const char* model = "phi3";
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "ollama run %s \"%s\" 2>/dev/null", model, prompt);

    FILE *fp = popen(cmd, "r");
    if (!fp) return strdup("Could not start ollama process.");
    char *result = malloc(4096); result[0] = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        strncat(result, buf, 4096 - strlen(result) - 1);
    }
    pclose(fp);
    return result;
}

static void on_ask(GtkButton *btn, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    const char *prompt = gtk_entry_get_text(entry);

    if (!prompt || !*prompt) return;

    GtkWidget *dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "Thinking... Please wait.");
    gtk_widget_show(dialog);
    while (gtk_events_pending()) gtk_main_iteration(); // Обновление окна

    char *response = ask_ollama(prompt);

    gtk_window_destroy(GTK_WINDOW(dialog));
    GtkWidget *resp_dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "%s", response);
    gtk_dialog_run(GTK_DIALOG(resp_dialog));
    gtk_window_destroy(GTK_WINDOW(resp_dialog));
    free(response);
}

void show_ai_helper(GtkButton *btn, gpointer user_data) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "Ollama AI Assistant");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 100);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 7);
    gtk_window_set_child(GTK_WINDOW(win), vbox);

    GtkWidget *label = gtk_label_new("Ask your question to local AI:");
    gtk_box_append(GTK_BOX(vbox), label);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(vbox), entry);

    GtkWidget *btn_ask = gtk_button_new_with_label("Ask AI");
    gtk_box_append(GTK_BOX(vbox), btn_ask);

    g_signal_connect(btn_ask, "clicked", G_CALLBACK(on_ask), entry);

    gtk_window_present(GTK_WINDOW(win));
}

// Добавить AI-кнопку на панель только если Ollama установлен
void add_ai_button_if_ollama(GtkBox *box) {
    if (!ollama_installed()) return;
    GtkWidget *btn = gtk_button_new_with_label("AI Assistant");
    g_signal_connect(btn, "clicked", G_CALLBACK(show_ai_helper), NULL);
    gtk_box_append(box, btn);
}