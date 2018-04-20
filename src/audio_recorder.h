#ifndef AUDIO_RECORDER_H
#define AUDIO_RECORDER_H

#include <QObject>
#include <QDebug>

#include <gst/gst.h>
#include <glib.h>

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(QString record_url, QObject *parent = nullptr);
    void recStart();
    void recStop();
    
protected:
    GstElement *pipeline;
    
private:
    QString record_url;
    
signals:
    
public slots:
};

#endif // AUDIO_RECORDER_H
