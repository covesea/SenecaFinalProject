#pragma once
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <vector>
#include <nlohmann/json.hpp>

class TemperatureWidget : public Gtk::DrawingArea {
public:
    void setTemperatures(const std::vector<float>& temps_celsius, const std::vector<std::string>& targets);
    void toggleUnit();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    std::vector<float> temperatures_celsius;
    std::vector<std::string> target_units;
    std::string display_unit = "C";
};

class TemperatureBar : public Gtk::DrawingArea {
public:
    void setTemperature(float temp_celsius);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    float current_temp = 0.0f;
};

class TemperatureWindow : public Gtk::Window {
public:
    TemperatureWindow(const std::string& config_path);

private:
    void update_temperatures();
    Gtk::Box m_main_box;
    Gtk::Box m_content_box;
    Gtk::Label m_title;
    Gtk::Box m_temp_display_box;
    Gtk::Label m_ambient_label;
    Gtk::Label m_object_label;
    Gtk::Button m_button;
    TemperatureWidget m_temp_widget;
    TemperatureBar m_bar_widget;
    Gtk::Frame m_temp_frame;
    nlohmann::json config;
    std::vector<float> current_temps;
    std::string current_unit = "C";
};