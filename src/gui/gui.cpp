#include "gui.h"

#include <stdio.h>

GUI::GUI() {
    app = gtk_application_new("org.example.MyCalculator", G_APPLICATION_FLAGS_NONE);
    expression_parser = ExpressionParser();
    current_input = "";
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

void GUI::setupCssProvider(const gchar *path) {
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, path);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(cssProvider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
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
    // gtk_window_maximize(GTK_WINDOW(window));

    setupCssProvider("src/gui/css/style.css");

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    display_label = gtk_label_new(current_input.c_str());
    gtk_widget_set_name(display_label, "display-label");
    gtk_widget_set_hexpand(display_label, TRUE);
    // gtk_widget_set_vexpand(display_label, TRUE);
    gtk_grid_attach(GTK_GRID(grid), display_label, 0, 0, 5, 1);

    static const std::map<ButtonType, std::string> BUTTON_CLASS_MAP = {
        {GUI::ButtonType::DELETE, "button-delete"},
        {GUI::ButtonType::OPERATION, "button-operation"},
        {GUI::ButtonType::EQUAL, "button-equal"},
        {GUI::ButtonType::NUMBER, "button-number"},
        {GUI::ButtonType::CLEAR, "button-clear"}
    };

    const char* button_labels[25] = {
        "⌫", "(", ")", "Clear", "Clear",
        "7", "8", "9", "÷", "√",
        "4", "5", "6", "x", "x²",
        "1", "2", "3", "-", "^",
        "0", ".", "mod", "+", "=",
    };

    static const ButtonType button_types[25] = {
        GUI::ButtonType::DELETE, GUI::ButtonType::OPERATION,    GUI::ButtonType::OPERATION,   GUI::ButtonType::CLEAR, GUI::ButtonType::CLEAR,
        GUI::ButtonType::NUMBER, GUI::ButtonType::NUMBER,       GUI::ButtonType::NUMBER,      GUI::ButtonType::OPERATION, GUI::ButtonType::OPERATION,
        GUI::ButtonType::NUMBER, GUI::ButtonType::NUMBER,       GUI::ButtonType::NUMBER,      GUI::ButtonType::OPERATION, GUI::ButtonType::OPERATION,
        GUI::ButtonType::NUMBER, GUI::ButtonType::NUMBER,       GUI::ButtonType::NUMBER,      GUI::ButtonType::OPERATION, GUI::ButtonType::OPERATION,
        GUI::ButtonType::NUMBER, GUI::ButtonType::NUMBER,       GUI::ButtonType::OPERATION,   GUI::ButtonType::OPERATION, GUI::ButtonType::EQUAL,
    };

    for (int i = 0; i < 25; ++i) {
        auto type = button_types[i];
        auto css_class = BUTTON_CLASS_MAP.at(type);
        GtkWidget *button = create_button(button_labels[i], css_class.c_str());
        gtk_grid_attach(GTK_GRID(grid), button, i % 5, i / 5 + 1, 1, 1);
        // Connect each button to the callback function
        switch(type) {
            case GUI::ButtonType::DELETE:
                g_signal_connect(button, "clicked", G_CALLBACK(on_delete_button_clicked), this);
                break;
            case GUI::ButtonType::OPERATION:
                g_signal_connect(button, "clicked", G_CALLBACK(on_operation_button_clicked), this);
                break;
            case GUI::ButtonType::EQUAL:
                g_signal_connect(button, "clicked", G_CALLBACK(on_equal_button_clicked), this);
                break;
            case GUI::ButtonType::NUMBER:
                g_signal_connect(button, "clicked", G_CALLBACK(on_number_button_clicked), this);
                break;
            case GUI::ButtonType::CLEAR:
                g_signal_connect(button, "clicked", G_CALLBACK(on_clear_button_clicked), this);
                break;
        }
    }

    gtk_widget_show(window);
}

void GUI::updateDisplay(const std::string& text) {
    current_input += text;
    gtk_label_set_text(GTK_LABEL(display_label), current_input.c_str());
}

void GUI::clearDisplay() {
    current_input.clear();
    gtk_label_set_text(GTK_LABEL(display_label), current_input.c_str());
}

void GUI::eraseDisplay() {
    if (current_input.empty()) {
        return;
    }
    current_input.erase(current_input.size() - 1);
    gtk_label_set_text(GTK_LABEL(display_label), current_input.c_str());
}

std::string GUI::getDisplay() {
    return current_input;
}

void GUI::on_number_button_clicked(GtkButton *button, gpointer user_data) {
    GUI* gui = static_cast<GUI*>(user_data);
    // Retrieve the label (number) of the button that was clicked
    const char *number = gtk_button_get_label(button);
    gui->updateDisplay(number);
}

void GUI::on_operation_button_clicked(GtkButton *button, gpointer user_data) {
    GUI* gui = static_cast<GUI*>(user_data);
    // Retrieve the label (number) of the button that was clicked
    const char *number = gtk_button_get_label(button);
    if (strcmp(number, "mod") == 0) {
        number = "%";
    } else if (strcmp(number, "÷") == 0) {
        number = "/";
    } else if (strcmp(number, "x") == 0) {
        number = "*";
    } else if (strcmp(number, "x²") == 0) {
        number = "^2";
    } else if (strcmp(number, "^") == 0) {
        number = "^";
    } else if (strcmp(number, "√") == 0) {
        number = "^(1/2)";
    }
    gui->updateDisplay(number);
}

void GUI::on_delete_button_clicked(GtkButton *button, gpointer user_data) {
    GUI* gui = static_cast<GUI*>(user_data);
    gui->eraseDisplay();
}

void GUI::on_equal_button_clicked(GtkButton *button, gpointer user_data) {
    GUI* gui = static_cast<GUI*>(user_data);
    std::string expression = gui->getDisplay();    
    gui->clearDisplay();
    gui->updateDisplay(std::to_string(gui->expression_parser.parseAndCalculate(expression)));
}

void GUI::on_clear_button_clicked(GtkButton *button, gpointer user_data) {
    GUI* gui = static_cast<GUI*>(user_data);
    gui->clearDisplay();
}
