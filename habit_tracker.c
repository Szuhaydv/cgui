#include "gdk/gdk.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "habit_tracker.h"

int main (int argc, char **argv) {
    FILE *dbf;
    int habit_data[7];
    initDatabase(dbf, habit_data);

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(initDisplay), habit_data);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

void newEntry(FILE *dbf, struct tm *last_monday, struct tm *current_day) {
        fprintf(dbf, "%d-%d-%d ", last_monday->tm_year+1900, last_monday->tm_mon+1, last_monday->tm_mday);
        for (int i = last_monday->tm_yday; i <= current_day->tm_yday; i++) {
            fprintf(dbf, "%d ", 0);
        }
        fputc('\n', dbf);
}

void initDisplay(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *container;
    GtkBuilder *builder;
    GtkCssProvider *provider;
    GObject *buttons[7];
    int *habit_data = (int *)user_data;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Habit Tracker");
    builder = gtk_builder_new_from_file("./app_layout.ui");
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_resource_path(theme, "./assets");

    for (int i = 0; i < 7; i++) {
        char buttonString[8];
        sprintf(buttonString, "button%d", i + 1);
        buttons[i] = gtk_builder_get_object(builder, buttonString);
        GtkWidget *image = NULL;
        // select correct image based on data
        switch (habit_data[i]) {
            case 1:
                image = gtk_image_new_from_file("./assets/angry.png");
                break;
            case 2:
                image = gtk_image_new_from_file("./assets/sad.png");
                break;
            case 3:
                image = gtk_image_new_from_file("./assets/ok.png");
                break;
        }
        if (image != NULL) {
            gtk_image_set_pixel_size(GTK_IMAGE(image), 64);
            gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
            gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
            gtk_button_set_child(GTK_BUTTON(buttons[i]), image);
        }
        GdkCursor* cursor = gdk_cursor_new_from_name("pointer", NULL);
        gtk_widget_set_cursor(GTK_WIDGET(buttons[i]), cursor);
        gtk_widget_add_css_class(GTK_WIDGET(buttons[i]), buttonString);
    }

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "./styles.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(window),
                                                GTK_STYLE_PROVIDER(provider),
                                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


    container = GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));
    gtk_widget_add_css_class(container, "main_box");
    gtk_window_set_child(GTK_WINDOW(window), container);
    gtk_window_present(GTK_WINDOW(window));
}

void initDatabase(FILE *dbf, int* habit_data) {
    for (int i = 0; i < 7; i++) {
        habit_data[i] = -1;
    }

    time_t current_time = time(NULL);
    struct tm *current_day = localtime(&current_time);
    struct tm *last_monday = malloc(sizeof(struct tm));
    *last_monday = *current_day;
    int days_to_subtract = (current_day->tm_wday == 0) ? 6 : current_day->tm_wday - 1;
    last_monday->tm_mday -= days_to_subtract;
    mktime(last_monday);

    dbf = fopen("db.txt", "r+");
    if (dbf == NULL) {
        printf("Error: No db file. Creating...\n");
        dbf = fopen("db.txt", "w+");
        newEntry(dbf, last_monday, current_day);
        free(last_monday);
        return;
    }
    struct tm *last_entry = malloc(sizeof(struct tm));
    *last_entry = *current_day;
    int year, month, day;
    fscanf(dbf, "%d-%d-%d", &year, &month, &day);
    last_entry->tm_year = year - 1900;
    last_entry->tm_mon = month - 1;
    last_entry->tm_mday = day;
    fgetc(dbf);
    mktime(last_entry);

    if (
        last_entry->tm_year == last_monday->tm_year &&
        last_entry->tm_mon == last_monday->tm_mon &&
        last_entry->tm_mday == last_monday->tm_mday
    ) {
        // this will get corrupted on the week that spans from 1 year to next
        int day_diff = current_day->tm_yday - last_entry->tm_yday;
        for (int i = 0; i <= day_diff; i++) {
            int c = fgetc(dbf);
            if (c == '\n' || c == EOF) {
                fseek(dbf, -1, SEEK_CUR);
                for (;i <= day_diff; i++) {
                    fprintf(dbf, "%d ", 0);
                    habit_data[i] = c - '0';
                }
                if (c == EOF)
                    fputc('\n', dbf);
                break;
            }
            habit_data[i] = c - '0';
            fgetc(dbf);
        }
    } else {
        newEntry(dbf, last_monday, current_day);
    }

    // could be later optimised for binary file
    fclose(dbf);
}
