#ifndef Q_DIRTYDOZEN_WIDGET_H
#define Q_DIRTYDOZEN_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>

#include "db_adapter.h"
#include "q_click_label.h"

class QDirtyDozenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDirtyDozenWidget(QString deck_name, QWidget *parent = nullptr);
    
    void initialize(QString deck_name);
    void clear();
    
private:
    QGridLayout *grid;
    
    int COLUMNS = 4;
    QList<QString> DISPLAY_COMBO_ITEMS;
    QString learn_mode = "image";
    
    QList<QMap<QString,QVariant>> dataset;
    QList<QMap<QString,QVariant>> full_dataset;
    int counter = 1;
    int delay_counter = 1;
    int delay = 2;
    int last_random_audio;
    QMediaPlayer *audioPlayer;
    
    void update();
    
    
signals:
    
public slots:
    
private slots:
    void showAllButtonClicked();
    void labelClicked();
    void playNextAudio();
    void playRandomAudio();
    void playAudio();
    void replayAudioClicked();
    void selectDisplayCurrentIndexChanged();
};

#endif // Q_DIRTYDOZEN_WIDGET_H
