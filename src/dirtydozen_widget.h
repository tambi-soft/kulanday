#ifndef Q_DIRTYDOZEN_WIDGET_H
#define Q_DIRTYDOZEN_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QPixmap>
#include <QSize>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QFont>
#include <QIcon>
#include <QSlider>
#include <QGroupBox>
#include <QLayout>

#include "db_adapter.h"
#include "unicode_fonts.h"

class QDirtyDozenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDirtyDozenWidget(QDir *decks_path, QString deck_name, QWidget *parent = nullptr);
    
    void initialize(QString deck_name);
    void clear();
    void clearStyleSheet();
    
private:
    QGridLayout *grid;
    QSlider *slider_repeat_times;
    UnicodeFonts *unicodeFonts;
    
    QDir *decks_path;
    
    int COLUMNS = 4;
    QList<QString> DISPLAY_COMBO_ITEMS;
    QString learn_mode = "image";
    QList<QPushButton*> button_list;
    
    QString deck_name;
    QList<QMap<QString,QVariant>> dataset;
    QList<QMap<QString,QVariant>> full_dataset;
    int counter = 1;
    int delay_counter = 0;
    int delay = 2;
    int last_random_audio;
    int current_audio_deck_id;
    QComboBox *select_display_combo;
    QPushButton *show_all_button;
    QMediaPlayer *audioPlayer;
    
    void sliderRepeatTimesChanged(int value);
    
    void update();
    QList<QMap<QString, QVariant> > shuffleList(QList<QMap<QString, QVariant> > list);
    
protected:
    void hideEvent(QHideEvent *event);
    
signals:
    
public slots:
    
private slots:
    void showAllButtonClicked();
    void displayButtonClicked(int rowid, QPushButton *button);
    void playNextAudio();
    void playRandomAudio();
    void playAudio(int selector);
    void replayAudioClicked();
    void selectDisplayCurrentTextChanged(QString text);
    void onShuffleButtonClicked();
};

#endif // Q_DIRTYDOZEN_WIDGET_H
