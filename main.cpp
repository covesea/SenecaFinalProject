#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "config.hpp"
#include "sensor.hpp"
#include "temp_logic.hpp"
#include "gui.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto config = load_config("config.json");

    std::vector<float> temps;
    for (const auto& sensor_config : config["sensors"]) {
        std::string path = sensor_config["path"];
        float raw_value = read_temperature_from_sysfs(path);
        float value = convert_using_json(sensor_config, raw_value); // remains Celsius
        temps.push_back(value);
    }

    QString initial_unit = QString::fromStdString(config["sensors"][0]["target_unit"]);

    QWidget window;
    QVBoxLayout* layout = new QVBoxLayout;

    TemperatureWidget* tempWidget = new TemperatureWidget;
    tempWidget->setTemperatures(temps, initial_unit);

    QPushButton* toggleButton = new QPushButton("Toggle °C/°F");
    QObject::connect(toggleButton, &QPushButton::clicked, tempWidget, &TemperatureWidget::toggleUnit);

    layout->addWidget(tempWidget);
    layout->addWidget(toggleButton);
    window.setLayout(layout);
    window.resize(300, 250);
    window.show();

    return app.exec();
}