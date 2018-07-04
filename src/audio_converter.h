#ifndef AUDIO_CONVERTER_H
#define AUDIO_CONVERTER_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSysInfo>

#include <QDebug>

class AudioConverter : public QObject
{
    Q_OBJECT
public:
    explicit AudioConverter(QDir *decks_path, QObject *parent = nullptr);
    
    void convertAudioToProperOggVorbis(QString deck_name);
    
private:
    QDir *decks_path;
    
signals:
    
public slots:
};

#endif // AUDIO_CONVERTER_H
