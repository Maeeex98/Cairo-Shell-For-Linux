#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

void show_browser_window() {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "My Browser");
    gtk_window_set_default_size(GTK_WINDOW(win), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    GtkWidget *back_btn = gtk_button_new_with_label("<");
    GtkWidget *forward_btn = gtk_button_new_with_label(">");
    GtkWidget *home_btn = gtk_button_new_with_label("🏠");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_append(GTK_BOX(toolbar), back_btn);
    gtk_box_append(GTK_BOX(toolbar), forward_btn);
    gtk_box_append(GTK_BOX(toolbar), home_btn);
    gtk_box_append(GTK_BOX(toolbar), entry);

    WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_append(GTK_BOX(vbox), toolbar);
    gtk_box_append(GTK_BOX(vbox), GTK_WIDGET(webview));
    gtk_window_set_child(GTK_WINDOW(win), vbox);

    // Домашняя страница
    const char *homepage = "https://duckduckgo.com";
    webkit_web_view_load_uri(webview, homepage);
    gtk_editable_set_text(GTK_EDITABLE(entry), homepage);

    // Навигация
    g_signal_connect(back_btn, "clicked", G_CALLBACK(
        [](GtkButton *b, gpointer wv){ webkit_web_view_go_back(WEBKIT_WEB_VIEW(wv)); }
    ), webview);
    g_signal_connect(forward_btn, "clicked", G_CALLBACK(
        [](GtkButton *b, gpointer wv){ webkit_web_view_go_forward(WEBKIT_WEB_VIEW(wv)); }
    ), webview);
    g_signal_connect(home_btn, "clicked", G_CALLBACK(
        [](GtkButton *b, gpointer data){
            WebKitWebView *wv = WEBKIT_WEB_VIEW(data);
            webkit_web_view_load_uri(wv, "https://duckduckgo.com");
        }
    ), webview);
    g_signal_connect(entry, "activate", G_CALLBACK(
        [](GtkEntry *e, gpointer wv){
            const char *uri = gtk_entry_get_text(e);
            webkit_web_view_load_uri(WEBKIT_WEB_VIEW(wv), uri);
        }
    ), webview);

    // Автообновление строки
    g_signal_connect(webview, "notify::uri", G_CALLBACK(
        [](GObject *wv, GParamSpec *pspec, gpointer entry){
            const char *uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(wv));
            gtk_editable_set_text(GTK_EDITABLE(entry), uri ? uri : "");
        }
    ), entry);

    gtk_window_present(GTK_WINDOW(win));
}