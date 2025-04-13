#include "gui.hpp"
#include "temp_logic.hpp"
#include "sensor.hpp"
#include "config.hpp"
#include <cairomm/context.h>
#include <sstream>
#include <glibmm/main.h>

void TemperatureWidget::setTemperatures(const std::vector<float>& temps_celsius, const std::vector<std::string>& targets) {
    temperatures_celsius = temps_celsius;
    target_units = targets;
    queue_draw();
}

void TemperatureWidget::toggleUnit() {
    display_unit = (display_unit == "C") ? "F" : "C";
    queue_draw();
}

bool TemperatureWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->set_source_rgb(0.1, 0.1, 0.1);
    cr->paint();
    cr->set_source_rgb(1, 1, 1);
    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(36);

    int y = 50;
    for (size_t i = 0; i < temperatures_celsius.size(); ++i) {
        float display_temp = (display_unit == "F") ? to_fahrenheit(temperatures_celsius[i]) : temperatures_celsius[i];
        std::ostringstream oss;
        oss << display_temp << "°" << display_unit;
        cr->move_to(50, y);
        cr->show_text(oss.str());
        y += 60;
    }
    return true;
}

void TemperatureBar::setTemperature(float temp_celsius) {
    current_temp = temp_celsius;
    queue_draw();
}

bool TemperatureBar::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();

    cr->set_source_rgb(0.2, 0.2, 0.2);
    cr->paint();

    Cairo::RefPtr<Cairo::LinearGradient> gradient = Cairo::LinearGradient::create(0, 0, 0, height);
    gradient->add_color_stop_rgb(0.0, 1.0, 0.0, 0.0); // Red at top
    gradient->add_color_stop_rgb(1.0, 0.0, 0.0, 1.0); // Blue at bottom

    cr->set_source(gradient);
    cr->rectangle(0, 0, width, height);
    cr->fill();

    float min_temp = 0.0f;
    float max_temp = 37.0f;
    float clamped_temp = std::min(std::max(current_temp, min_temp), max_temp);
    float normalized = (clamped_temp - min_temp) / (max_temp - min_temp);
    float bar_y = height * (1.0f - normalized);

    cr->set_source_rgb(1, 1, 1); // white
    cr->rectangle(0, bar_y - 2, width, 4);
    cr->fill();

    return true;
}

TemperatureWindow::TemperatureWindow(const std::string& config_path)
    : m_main_box(Gtk::ORIENTATION_VERTICAL), m_content_box(Gtk::ORIENTATION_HORIZONTAL), m_temp_display_box(Gtk::ORIENTATION_VERTICAL),
      m_button("Toggle °C/°F") {
    set_title("Temperature Viewer");
    set_default_size(400, 300);

    m_title.set_text("Sensor Readings:");
    m_title.set_margin_top(10);
    m_title.set_margin_bottom(10);
    m_main_box.pack_start(m_title, Gtk::PACK_SHRINK);

    config = load_config(config_path);
    update_temperatures();

    m_button.signal_clicked().connect([this]() {
        m_temp_widget.toggleUnit();
        current_unit = (current_unit == "C") ? "F" : "C";
        update_temperatures();
    });

    m_temp_display_box.pack_start(m_ambient_label, Gtk::PACK_SHRINK);
    m_temp_display_box.pack_start(m_object_label, Gtk::PACK_SHRINK);

    m_content_box.pack_start(m_temp_widget);
    m_temp_frame.set_label("Ambient Temp");
    m_temp_frame.add(m_bar_widget);
    m_content_box.pack_start(m_temp_frame);
    m_content_box.pack_start(m_temp_display_box);

    m_main_box.pack_start(m_content_box);
    m_main_box.pack_start(m_button, Gtk::PACK_SHRINK);

    add(m_main_box);
    show_all_children();

    Glib::signal_timeout().connect_seconds([this]() {
        update_temperatures();
        return true;
    }, 2);
}

void TemperatureWindow::update_temperatures() {
    std::vector<float> temps_c;
    std::vector<std::string> targets;
    for (const auto& sensor_config : config["sensors"]) {
        std::string path = sensor_config["path"];
        float raw_c = read_temperature_from_sysfs(path);
        temps_c.push_back(raw_c);
        targets.push_back(sensor_config.value("target_unit", "C"));
    }
    current_temps = temps_c;
    m_temp_widget.setTemperatures(temps_c, targets);

    if (!temps_c.empty()) {
        m_bar_widget.setTemperature(temps_c[0]);
        float ambient_display = (current_unit == "F") ? to_fahrenheit(temps_c[0]) : temps_c[0];
        float object_display = (temps_c.size() > 1) ? ((current_unit == "F") ? to_fahrenheit(temps_c[1]) : temps_c[1]) : 0.0f;

        std::ostringstream ambient_text;
        ambient_text << "Ambient temperature: " << ambient_display << "°" << current_unit;
        std::ostringstream object_text;
        object_text << "Object temperature: " << object_display << "°" << current_unit;

        m_ambient_label.set_text(ambient_text.str());
        m_object_label.set_text(object_text.str());
    }
}
