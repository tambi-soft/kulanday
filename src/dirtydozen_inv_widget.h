#ifndef Q_LEARN_WIDGET_H
#define Q_LEARN_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QToolTip>
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
#include <QSize>

#include "db_adapter.h"
#include "unicode_fonts.h"
#include "config.h"

class QInvDirtyDozenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QInvDirtyDozenWidget(QDir *decks_path, QString deck_name, Config *config, QWidget *parent = nullptr);
    void initialize();
    void clear();
        
private:
    Config *config;
    QGridLayout *grid;
    UnicodeFonts *unicodeFonts;
    
    QDir *decks_path;
    
    int COLUMNS = 4;
    int ROWS = 3;
    QList<QString> DISPLAY_COMBO_ITEMS;
    QString learn_mode = "image";
    QList<QPushButton*> button_list;
    
    QString deck_name;
    QList<QMap<QString,QVariant>> dataset;
    int counter = 1;
    int delay_counter = 0;
    int delay = 2;
    int last_random_audio;
    QComboBox *select_display_combo;
    QPushButton *show_all_button;
    QMediaPlayer *audioPlayer;
    
    void update();
    QList<QMap<QString, QVariant> > shuffleList(QList<QMap<QString, QVariant> > list);
    QPushButton *button_last_play = nullptr;
        
protected:
    void hideEvent(QHideEvent *event);
        
signals:
        
public slots:
        
private slots:
    void displayButtonClicked(int rowid, QPushButton *button);
    void displayButtonReleased(int rowid, QPushButton *button);
    void showWrittenForm(int selector, QPushButton *button);
    void playAudio(int selector);
    void selectDisplayCurrentTextChanged(QString text);
    void onShuffleButtonClicked();
    void onNewSetButtonClicked();
};

#endif // Q_LEARN_WIDGET_H
