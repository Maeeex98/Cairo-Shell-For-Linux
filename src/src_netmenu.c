#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static void get_ip_mac(char *iface, char *ipbuf, size_t iplen, char *macbuf, size_t maclen) {
    // Получить IP-адрес через ip
    char cmd[128], line[256];
    sprintf(cmd, "ip -f inet addr show %s | grep inet | awk '{print $2}' | cut -d/ -f1", iface);
    FILE *f = popen(cmd, "r");
    if (f && fgets(line, sizeof(line), f)) {
        snprintf(ipbuf, iplen, "%s", line);
        ipbuf[strcspn(ipbuf, "\n")] = 0;
    } else {
        snprintf(ipbuf, iplen, "N/A");
    }
    if (f) pclose(f);

    // Получить MAC-адрес через cat
    sprintf(cmd, "cat /sys/class/net/%s/address", iface);
    f = popen(cmd, "r");
    if (f && fgets(line, sizeof(line), f)) {
        snprintf(macbuf, maclen, "%s", line);
        macbuf[strcspn(macbuf, "\n")] = 0;
    } else {
        snprintf(macbuf, maclen, "N/A");
    }
    if (f) pclose(f);
}

static void on_disconnect_clicked(GtkButton *btn, gpointer iface) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nmcli device disconnect %s", (char*)iface);
    system(cmd);
}

static void on_connect_clicked(GtkButton *btn, gpointer iface) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nmcli device connect %s", (char*)iface);
    system(cmd);
}

static void show_net_menu(GtkButton *btn, gpointer user_data) {
    GtkWidget *menu = gtk_popover_new();
    gtk_popover_set_has_arrow(GTK_POPOVER(menu), TRUE);
    gtk_popover_set_pointing_to(GTK_POPOVER(menu), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Используем nmcli для поиска активных интерфейсов
    FILE *f = popen("nmcli -t -f DEVICE,STATE device | grep -P 'connected|disconnected' | head -n1", "r");
    char line[128], iface[64], state[32];
    if (f && fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^:]:%s", iface, state);
        // Название интерфейса
        char ip[64], mac[64];
        get_ip_mac(iface, ip, sizeof(ip), mac, sizeof(mac));
        char buf[256];
        snprintf(buf, sizeof(buf), "Интерфейс: %s\nСтатус: %s\nIP: %s\nMAC: %s", iface, state, ip, mac);

        GtkWidget *label = gtk_label_new(buf);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_append(GTK_BOX(box), label);

        if (strcmp(state, "connected") == 0) {
            GtkWidget *btn_disc = gtk_button_new_with_label("Отключиться");
            g_signal_connect(btn_disc, "clicked", G_CALLBACK(on_disconnect_clicked), g_strdup(iface));
            gtk_box_append(GTK_BOX(box), btn_disc);
        } else {
            GtkWidget *btn_conn = gtk_button_new_with_label("Подключиться");
            g_signal_connect(btn_conn, "clicked", G_CALLBACK(on_connect_clicked), g_strdup(iface));
            gtk_box_append(GTK_BOX(box), btn_conn);
        }
    } else {
        GtkWidget *label = gtk_label_new("Нет активных сетей");
        gtk_box_append(GTK_BOX(box), label);
    }
    if (f) pclose(f);

    gtk_popover_set_child(GTK_POPOVER(menu), box);
    gtk_popover_set_position(GTK_POPOVER(menu), GTK_POS_BOTTOM);
    gtk_popover_popup(GTK_POPOVER(menu));
    gtk_popover_popup(GTK_POPOVER(menu)); // для gtk4 workaround

    gtk_widget_show(menu);
}

// Пример интеграции с панелью (добавить кнопку)
void add_net_button(GtkBox *box) {
    GtkWidget *btn = gtk_button_new_with_label("🌐 Интернет");
    g_signal_connect(btn, "clicked", G_CALLBACK(show_net_menu), NULL);
    gtk_box_append(box, btn);
}