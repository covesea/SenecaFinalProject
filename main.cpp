#include <gtkmm/application.h>
#include "gui.hpp"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.example.tempviewer");
    TemperatureWindow window("/etc/config.json");
    return app->run(window);
}