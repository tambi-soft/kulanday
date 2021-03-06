#include "audio_recorder.h"

AudioRecorder::AudioRecorder(QObject *parent)
    : QObject(parent)
    , recorder (new QAudioRecorder)
{
    
}

void AudioRecorder::setContainerFormat(const QString &container)
{
    this->recorder->setContainerFormat(container);
}

void AudioRecorder::setEncodingSettings(const QAudioEncoderSettings &audio, const QVideoEncoderSettings &video, const QString &container)
{
    this->recorder->setEncodingSettings(audio, video, container);
}

void AudioRecorder::setOutputLocation(const QUrl &location)
{
    this->recorder->setOutputLocation(location);
}

void AudioRecorder::record()
{
    this->recorder->record();
}

void AudioRecorder::stop()
{
    this->recorder->stop();
}
