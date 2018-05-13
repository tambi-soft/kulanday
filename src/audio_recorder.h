#ifndef AUDIO_RECORDER_H
#define AUDIO_RECORDER_H

#include <QObject>



#ifdef __linux__
#include "gst_audio_recorder.h"
#else
#include <QAudioRecorder>
#endif

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(QObject *parent = nullptr);
    
    void setContainerFormat(const QString &container);
    void setEncodingSettings(const QAudioEncoderSettings &audio, const QVideoEncoderSettings &video = QVideoEncoderSettings(), const QString &container = QString());
    void setOutputLocation(const QUrl &location);
    void record();
    void stop();

private:
#ifdef __linux__
    GstAudioRecorder *recorder;
#else
    QAudioRecorder *recorder;
#endif
    
signals:
    
public slots:
};

#endif // AUDIO_RECORDER_H
