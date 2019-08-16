#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>

#include "config.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(Config *config, QWidget *parent = nullptr);
    
private:
    Config *config;
    
signals:
    
public slots:
};

#endif // SETTINGS_WIDGET_H
