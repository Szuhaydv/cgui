#include <gtk/gtk.h>
#include <stdio.h>

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *container;
    GtkBuilder *builder;
    GtkCssProvider *provider;
    GObject *buttons[7];

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Habit Tracker");
    builder = gtk_builder_new_from_file("./app_layout.ui");
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_resource_path(theme, "./assets");

    for (int i = 0; i < 7; i++) {
        char buttonString[8];
        sprintf(buttonString, "button%d", i + 1);
        printf("%s", buttonString);
        buttons[i] = gtk_builder_get_object(builder, buttonString);
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

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
