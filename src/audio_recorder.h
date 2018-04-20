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


#ifndef AUDIO_RECORDER_THREAD_H

#include <QThread>



class AudioRecorderThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioRecorderThread(QThread *parent = nullptr);
    
protected:
    
    
    void run();
    
signals:
    
public slots:
    
};

#endif //AUDIO_RECORDER_THREAD_H
