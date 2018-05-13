#ifndef AUDIO_RECORDER_H
#define AUDIO_RECORDER_H

#include <QObject>

/*
 * We had some trouble with QAudioRecorder on linux, so we used this abstraction of QAudioRecorder to use gstreamer directly on linux and QAudioRecorder on windows and mac. Now this class seems to be deprecated.
 */

#include <QAudioRecorder>

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
    QAudioRecorder *recorder;
    
signals:
    
public slots:
};

#endif // AUDIO_RECORDER_H
