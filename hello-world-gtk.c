#include "gdk/gdk.h"
#include <gtk/gtk.h>
#include <stdio.h>

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *container;
    GtkBuilder *builder;
    GtkCssProvider *provider;
    GObject *buttons[7];
    FILE *dbf;
    int weekly_data[7];

    dbf = fopen("db.txt", "r+");

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Habit Tracker");
    builder = gtk_builder_new_from_file("./app_layout.ui");
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_resource_path(theme, "./assets");

    for (int i = 0; i < 7; i++) {
        fscanf(dbf, "%d", &weekly_data[i]);
        char buttonString[8];
        sprintf(buttonString, "button%d", i + 1);
        buttons[i] = gtk_builder_get_object(builder, buttonString);
        GtkWidget *image = NULL;
        switch (weekly_data[i]) {
            case -1:
                image = gtk_image_new_from_file("./assets/angry.png");
                break;
            case 0:
                image = gtk_image_new_from_file("./assets/sad.png");
                break;
            case 1:
                image = gtk_image_new_from_file("./assets/ok.png");
                break;
            default:
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

    fclose(dbf);

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

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
