#ifndef Q_AUDIO_LIST_WIDGET_H
#define Q_AUDIO_LIST_WIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView> // for hiding headers
#include <QPushButton>
#include <QIcon>

#include <QMessageBox>

#include <QMediaPlayer>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>

#include "db_adapter.h"

#ifdef __linux__

#endif

class QAudioListTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit QAudioListTable(QString deck_name, qlonglong deck_rowid,  QTableWidget *parent = nullptr);
    
    void newAudioLine();
    
private:
    QString deck_name;
    qlonglong deck_rowid;
    
    QList<QMap<QString,QVariant>> data;
    
    bool ignore_item_changes;
    void drawAudioTable();
    
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
    QAudioRecorder *recorder;
    QPushButton *playing_button = nullptr;
    
    void audioButtonClicked(QPushButton *button, QString audio_filename);
    void recordButtonClicked(QPushButton *button, QString audio_filename);
    void mediaPlayerStateChanged(int state);
    
signals:
    
private slots:
    void onItemChanged();
    void deleteButtonClicked(int row);
    void importButtonClicked(int row);
    void editButtonClicked(int row);
    
public slots:
    
};

#endif // Q_AUDIO_LIST_WIDGET_H
