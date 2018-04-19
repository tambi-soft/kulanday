#ifndef AUDIO_RECORDER_H
#define AUDIO_RECORDER_H

#include <QObject>

#include <gst/gst.h>
#include <glib.h>

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(QObject *parent = nullptr);
    
signals:
    
public slots:
};

#endif // AUDIO_RECORDER_H
