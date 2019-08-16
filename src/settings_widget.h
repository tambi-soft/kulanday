#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

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
    
    void showFolderSelectDialog();
    
    QLineEdit *edit_path;
    
    QSize dd_size;
    
signals:
    

public slots:
    
};

#endif // SETTINGS_WIDGET_H
