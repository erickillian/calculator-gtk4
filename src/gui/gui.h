#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <map>
#include <string>

#include "core/expression_parser.h"

class GUI {
public:
    GUI();
    ~GUI();

    int run(int argc, char *argv[]);

    // Add an enumeration for button types
    enum ButtonType {
        DELETE, OPERATION, EQUAL, NUMBER, CLEAR
    };

private:
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *display_label;
    std::string current_input;
    GtkWidget *display;
    ExpressionParser expression_parser;

    void setupCssProvider(const gchar *path);
    void add_css_class(GtkWidget *widget, const gchar *class_name);
    GtkWidget* create_button(const char* label, const char* css_class);
    void onActivate(GtkApplication *app);
    void updateDisplay(const std::string& text);
    void clearDisplay();
    void eraseDisplay();
    std::string getDisplay();
    static void on_number_button_clicked(GtkButton *button, gpointer user_data);
    static void on_operation_button_clicked(GtkButton *button, gpointer user_data);
    static void on_delete_button_clicked(GtkButton *button, gpointer user_data);
    static void on_equal_button_clicked(GtkButton *button, gpointer user_data);
    static void on_clear_button_clicked(GtkButton *button, gpointer user_data);
};

#endif // GUI_H
