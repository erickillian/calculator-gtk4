#include "gui.h"
#include "core/calculator.h"

GUI::GUI() {
    app = gtk_application_new("org.example.MyCalculator", G_APPLICATION_FLAGS_NONE);
}

GUI::~GUI() {
    g_object_unref(app);
}

int GUI::run(int argc, char *argv[]) {
    g_signal_connect(app, "activate", G_CALLBACK(+[](GtkApplication *app, gpointer user_data) {
        static_cast<GUI*>(user_data)->onActivate(app);
    }), this);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    return status;
}

// Function to add a CSS class to a widget
void GUI::add_css_class(GtkWidget *widget, const gchar *class_name) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, class_name);
}

GtkWidget* GUI::create_button(const char* label, const char* css_class) {
    GtkWidget *button = gtk_button_new_with_label(label);
    add_css_class(button, css_class);
    return button;
}

void GUI::onActivate(GtkApplication *app) {
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_maximize(GTK_WINDOW(window));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/gui/css/calculator_style.css");

    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(cssProvider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Create display label
    display_label = gtk_label_new("0");
    // Set display label properties (e.g., expand, fill, padding)
    // and add to grid at position (0,0) spanning 4 columns and 1 row
    gtk_grid_attach(GTK_GRID(grid), display_label, 0, 0, 4, 1);

    // Define button labels in the order they should appear on the grid
    const char* button_labels[] = {
        "⌫", "(", ")", "mod", "π",
        "7", "8", "9", "÷", "√",
        "4", "5", "6", "x", "x²",
        "1", "2", "3", "-", "^", 
        "0", ".", "%", "+", "=",
    };
    
    // Define classes for buttons
    const char* button_classes[25] = {
        "button-delete", "button-operation", "button-operation", "button-operation", "button-operation",
        "button", "button", "button", "button-operation", "button-operation",
        "button", "button", "button", "button-operation", "button-operation",
        "button", "button", "button", "button-operation", "button-operation",
        "button", "button-operation", "button-operation", "button-operation", "button-equal",
    };

    // Place buttons on the grid
    for (int i = 0; i < 25; ++i) {
        GtkWidget *button = create_button(button_labels[i], button_classes[i]);
        int row = (i / 5) + 1; // +1 to account for the display row
        int col = i % 5;
        int width = 1;
        int height = 1;

        gtk_grid_attach(GTK_GRID(grid), button, col, row, width, height);
    }

    gtk_widget_show(window);
}

void GUI::on_number_button_clicked(GtkButton *button, gpointer user_data) {
    // Implement number button clicked logic
    // ...
}

void GUI::on_operation_button_clicked(GtkButton *button, gpointer user_data) {
    // Implement operation button clicked logic
    // ...
}
