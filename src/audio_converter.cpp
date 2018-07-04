#include "audio_converter.h"

AudioConverter::AudioConverter(QDir *decks_path, QObject *parent) : QObject(parent)
{
    this->decks_path = decks_path;
}

void AudioConverter::convertAudioToProperOggVorbis(QString deck_name)
{
    QProcess pro;
    QString ffmpeg_path = "";
    if (QSysInfo::kernelType() == "winnt")
    {
        ffmpeg_path = "./ffmpeg-20180702-win32-static/bin/";
    }
    
    QDir *deck = new QDir(this->decks_path->absolutePath() + "/" + deck_name);
    QStringList file_names = deck->entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Type);
    
    deck->mkdir("tmp");
    
    foreach (QString file_name, file_names)
    {
        QString filepath = this->decks_path->absolutePath() + "/" + deck_name + "/" + file_name;
        QFile audio(filepath);
        QFileInfo info(audio);
        if (info.suffix() == "ogg")
        {
            QString command_a = ffmpeg_path + "ffmpeg -i " + filepath + " " + deck->absolutePath() + "/tmp/" + info.baseName() + ".wav";
            pro.start(command_a);
            pro.waitForFinished(-1);
            if (pro.exitStatus() == QProcess::NormalExit)
            {            
                audio.remove();
                
                QString command_b = ffmpeg_path + "ffmpeg -i " + deck->absolutePath() + "/tmp/" + info.baseName() + ".wav" + " " + filepath;
                pro.start(command_b);
                pro.waitForFinished(-1);
            }
        }
    }
    
    QString delpath = deck->absolutePath() + "/tmp";
    QDir(deck->absolutePath() + "/tmp").removeRecursively();
}
