#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>

#include "config.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(Config *config, QWidget *parent = nullptr);
    
private:
    Config *config;
    QVBoxLayout *layout;
    
    void addDirtyDozenSettingsArea();
    void addDeckPathSettingsArea();
    
    void ddWidthChanged(int width);
    void ddHeightChanged(int height);
    
    QSize dd_size;
    
signals:
    

public slots:
    
};

#endif // SETTINGS_WIDGET_H
