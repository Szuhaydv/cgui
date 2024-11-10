#include <gtk/gtk.h>

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *container;
    GtkBuilder *builder;
    GtkCssProvider *provider;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Habit Tracker");
    builder = gtk_builder_new_from_file("./app_layout.ui");


    GObject *button1 = gtk_builder_get_object(builder, "button1");
    GObject *button2 = gtk_builder_get_object(builder, "button2");
    GObject *button3 = gtk_builder_get_object(builder, "button3");
    GObject *button4 = gtk_builder_get_object(builder, "button4");
    GObject *button5 = gtk_builder_get_object(builder, "button5");
    GObject *button6 = gtk_builder_get_object(builder, "button6");
    GObject *button7 = gtk_builder_get_object(builder, "button7");
    gtk_widget_add_css_class(GTK_WIDGET(button1), "button1");
    gtk_widget_add_css_class(GTK_WIDGET(button2), "button2");
    gtk_widget_add_css_class(GTK_WIDGET(button3), "button3");
    gtk_widget_add_css_class(GTK_WIDGET(button4), "button4");
    gtk_widget_add_css_class(GTK_WIDGET(button5), "button5");
    gtk_widget_add_css_class(GTK_WIDGET(button6), "button6");
    gtk_widget_add_css_class(GTK_WIDGET(button7), "button7");

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
