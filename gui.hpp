#pragma once
#include <QWidget>
#include <vector>

class TemperatureWidget : public QWidget {
    Q_OBJECT
    std::vector<float> temperatures;
    QString unit = "C";

public:
    void setTemperatures(const std::vector<float>& temps, const QString& init_unit);
    void toggleUnit();

protected:
    void paintEvent(QPaintEvent* event) override;
};