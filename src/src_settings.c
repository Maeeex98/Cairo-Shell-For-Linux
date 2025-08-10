#include "settings.h"
#include <stdio.h>

void load_settings(AppSettings *s) {
    FILE *f = fopen("settings.conf", "r");
    if (!f) { s->file_manager_enabled = 1; s->browser_enabled = 1; return; }
    fscanf(f, "file_manager_enabled=%d\nbrowser_enabled=%d", &s->file_manager_enabled, &s->browser_enabled);
    fclose(f);
}
void save_settings(const AppSettings *s) {
    FILE *f = fopen("settings.conf", "w");
    if (!f) return;
    fprintf(f, "file_manager_enabled=%d\n", s->file_manager_enabled);
    fprintf(f, "browser_enabled=%d\n", s->browser_enabled);
    fclose(f);
}