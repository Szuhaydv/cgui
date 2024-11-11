#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>

void connectDatabase(FILE*, int*);
void newEntry(FILE *dbf, struct tm *last_monday, struct tm *current_day);
void initDatabase(FILE *dbf, int* weekly_data);
void initDisplay(GtkApplication *app, gpointer user_data);
