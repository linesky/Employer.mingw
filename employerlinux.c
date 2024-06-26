#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//gcc main.c -o employer_application `pkg-config --cflags --libs gtk+-3.0`
void calculate(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[])
{
    GtkWidget *window, *grid;
    GtkWidget *in_label, *in_entry;
    GtkWidget *out_label, *out_entry;
    GtkWidget *price_label, *price_entry;
    GtkWidget *calc_button;
    GtkWidget *result_textview;

    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Employer Application");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create a grid to arrange widgets
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Set the background color to yellow
    GdkRGBA yellow = {1.0, 1.0, 0.0, 1.0};
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &yellow);

    // Create the in label and entry
    in_label = gtk_label_new("In (hh:mm):");
    gtk_grid_attach(GTK_GRID(grid), in_label, 0, 0, 1, 1);
    in_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), in_entry, 1, 0, 1, 1);

    // Create the out label and entry
    out_label = gtk_label_new("Out (hh:mm):");
    gtk_grid_attach(GTK_GRID(grid), out_label, 0, 1, 1, 1);
    out_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), out_entry, 1, 1, 1, 1);

    // Create the price label and entry
    price_label = gtk_label_new("Price:");
    gtk_grid_attach(GTK_GRID(grid), price_label, 0, 2, 1, 1);
    price_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), price_entry, 1, 2, 1, 1);

    // Create the calculate button
    calc_button = gtk_button_new_with_label("=");
    gtk_grid_attach(GTK_GRID(grid), calc_button, 0, 3, 2, 1);

    // Create the result text view
    result_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(result_textview), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(result_textview), GTK_WRAP_WORD);
    gtk_widget_set_vexpand(result_textview, TRUE);
    gtk_grid_attach(GTK_GRID(grid), result_textview, 0, 4, 2, 1);

    // Connect the calculate button to the calculate function
    g_signal_connect(calc_button, "clicked", G_CALLBACK(calculate), (gpointer)grid);

    // Connect the destroy signal of the window to gtk_main_quit to close the application
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void calculate(GtkWidget *widget, gpointer data)
{
    GtkWidget *grid = GTK_WIDGET(data);
    GtkWidget *in_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    GtkWidget *out_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    GtkWidget *price_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    GtkWidget *result_textview = gtk_grid_get_child_at(GTK_GRID(grid), 0, 4);

    const char *in_text = gtk_entry_get_text(GTK_ENTRY(in_entry));
    const char *out_text = gtk_entry_get_text(GTK_ENTRY(out_entry));
    const char *price_text = gtk_entry_get_text(GTK_ENTRY(price_entry));

    int in_hour, in_min, out_hour, out_min;
    float price, total_value;

    if (sscanf(in_text, "%d:%d", &in_hour, &in_min) != 2 ||
        sscanf(out_text, "%d:%d", &out_hour, &out_min) != 2 ||
        sscanf(price_text, "%f", &price) != 1)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Invalid input!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    int total_in_minutes = in_hour * 60 + in_min;
    int total_out_minutes = out_hour * 60 + out_min;
    int diff_minutes = total_out_minutes - total_in_minutes;

    if (diff_minutes < 0)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Out time must be after In time!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    float hours = diff_minutes / 60.0f;
    total_value = hours * price;

    char result[256];
    sprintf(result, "Out - In = %.2f hours * %.2f = %.2f", hours, price, total_value);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(result_textview));
    gtk_text_buffer_set_text(buffer, result, -1);
}

