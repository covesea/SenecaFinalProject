#include "gui.hpp"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <cairo.h>
#include <cairo-qt.h>

void TemperatureWidget::setTemperatures(const std::vector<float>& temps, const QString& init_unit) {
    temperatures = temps;
    unit = init_unit;
    update();
}

void TemperatureWidget::toggleUnit() {
    for (float& temp : temperatures) {
        temp = (unit == "C") ? to_fahrenheit(temp) : to_celsius(temp);
    }
    unit = (unit == "C") ? "F" : "C";
    update();
}

void TemperatureWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    cairo_t* cr = qt_begin_cairo_painting(&painter);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36);

    int y = 50;
    for (float temp : temperatures) {
        std::string text = std::to_string(temp) + "°" + unit.toStdString();
        cairo_move_to(cr, 50, y);
        cairo_show_text(cr, text.c_str());
        y += 60;
    }

    qt_end_cairo_painting(&painter, cr);
}