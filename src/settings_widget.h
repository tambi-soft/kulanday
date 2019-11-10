#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
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
    void addAudioEditorSettingsArea();
    
    void ddWidthChanged(int width);
    void ddHeightChanged(int height);
    void ddItemSizeChanged(int size);
    void ddItemAspectChanged(double aspect);
    
    QLineEdit *edit_path;
    QLineEdit *edit_audio_editor;
    
    QSize dd_size;
    
signals:
    

public slots:
    void showFolderSelectDialog();
    void saveFolderPath();
    void showAudioEditorSelectDialog();
    void saveAudioEditorPath();
};

#endif // SETTINGS_WIDGET_H
