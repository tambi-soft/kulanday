#ifndef Q_AUDIO_LIST_WIDGET_H
#define Q_AUDIO_LIST_WIDGET_H

#include <QWidget>
#include <QMediaPlayer>

class QAudioListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAudioListWidget(QWidget *parent = nullptr);
    
private:
    int PLAY_BUTTON_COLUMN = 1;
    int RECORD_BUTTON_COLUMN = 1;
    int DELETE_BUTTON_COLUMN = 2;
    int OPEN_FILE_BUTTON_COLUMN = 3;
    int EDIT_FILE_BUTTON = 4;
    int FILE_NAME_COLUMN = 5;
    
    
    
signals:
    
public slots:
};

#endif // Q_AUDIO_LIST_WIDGET_H
