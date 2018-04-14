#ifndef Q_AUDIO_LIST_WIDGET_H
#define Q_AUDIO_LIST_WIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView> // for hiding headers
#include <QMediaPlayer>
#include <QPushButton>
#include <QIcon>

#include "db_adapter.h"

#ifdef __linux__

#endif

class QAudioListTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit QAudioListTable(QString deck_name, qlonglong rowid,  QTableWidget *parent = nullptr);
    
private:
    QString deck_name;
    
    int COLUMN_COUNT = 6;
    
    int DESCRIPTION_COLUMN = 0;
    int PLAY_BUTTON_COLUMN = 1;
    int RECORD_BUTTON_COLUMN = 1;
    int DELETE_BUTTON_COLUMN = 2;
    int IMPORT_FILE_BUTTON_COLUMN = 3;
    int EDIT_FILE_BUTTON = 4;
    int FILE_NAME_COLUMN = 5;
    
    DbAdapter *database = nullptr;
    QMediaPlayer *player;
    QPushButton *playing_button = nullptr;
    
    void audioButtonClicked(QPushButton *button, QString audio_filename);
    void mediaPlayerStateChanged(int state);
signals:
    
private slots:
    void deleteButtonClicked(int row);
    void importButtonClicked(int row);
    void editButtonClicked(int row);
    
public slots:
    
};

#endif // Q_AUDIO_LIST_WIDGET_H
