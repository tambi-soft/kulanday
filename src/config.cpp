#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    QDir *config_dir = new QDir();
    QFile *config_file = new QFile();
    
    if (QSysInfo::kernelType() == "darwin" || QSysInfo::kernelType() == "linux")
    {
        if (QSysInfo::productType() != "ios" || QSysInfo::productType() != "android")
        {
            config_dir->setPath(QDir::homePath() + "/.kulanday/");
            if (! config_dir->exists())
            {
                config_dir->mkdir(config_dir->absolutePath());
            }
            
            config_file->setFileName(config_dir->absolutePath() + "/" + "config.ini");
            if (! config_file->exists())
            {
                QFile::copy(":default_config", config_dir->absolutePath() + "/config.ini");
                QFile::setPermissions(config_file->fileName(),
                                      QFileDevice::ReadOwner |
                                      QFileDevice::ReadGroup |
                                      QFileDevice::ReadOther |
                                      QFileDevice::WriteOwner);
            }
        }
    }
    
    QSettings *settings = new QSettings(config_file->fileName(), QSettings::IniFormat);
    QVariant deckpath_var = settings->value("kulanday/deckpath");
    deckpath = new QDir(QDir::homePath() + "/.kulanday/decks");
    if (! deckpath->exists())
    {
        deckpath->mkdir(deckpath->absolutePath());
    }
    
    
}

QDir *Config::getDecksPath()
{
    return this->deckpath;
}
