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
        }
    }
    else if (QSysInfo::kernelType() == "winnt")
    {
        config_dir = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (! config_dir->exists())
        {
            config_dir->mkdir(config_dir->absolutePath());
        }
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
    
    this->settings = new QSettings(config_file->fileName(), QSettings::IniFormat);
    this->deckpath_var = this->settings->value("kulanday/deckpath").toString();
    
    deckpath = new QDir(deckpath_var.replace("$CONFDIR", config_dir->absolutePath()));
    if (! deckpath->exists())
    {
        deckpath->mkdir(deckpath->absolutePath());
    }
}

QDir *Config::getDecksPath()
{
    return this->deckpath;
}

QString Config::getDeckpathString()
{
    return this->deckpath_var;
}

void Config::setDeckpath(QString path)
{
    this->settings->setValue("kulanday/deckpath", path);
}

QString Config::getLastLanguageFilter()
{
    return this->settings->value("kulanday/last_language_filter").toString();
}

void Config::setLastLanguageFilter(QString filter)
{
    this->settings->setValue("kulanday/last_language_filter", filter);
}

QSize Config::getDirtyDozenSize()
{
    int width = this->settings->value("dirty_dozen/dirty_dozen_width").toInt();
    int height = this->settings->value("dirty_dozen/dirty_dozen_height").toInt();
    
    /*
    width=0, height=0 does not make any sense here;
    assuming key not set in config.
    */
    if (width == 0 || height == 0)
    {
        // Creating entries with default values:
        setDirtyDozenSize(QSize(4, 3));
        
        width = this->settings->value("dirty_dozen/dirty_dozen_width").toInt();
        height = this->settings->value("dirty_dozen/dirty_dozen_height").toInt();
    }
    
    return QSize(width, height);
}

void Config::setDirtyDozenSize(QSize size)
{
    this->settings->setValue("dirty_dozen/dirty_dozen_width", size.width());
    this->settings->setValue("dirty_dozen/dirty_dozen_height", size.height());
}

int Config::getDirtyDozenFieldSize()
{
    int size = this->settings->value("dirty_dozen/dirty_dozen_item_size").toInt();
    
    // size=0 makes no sense, probably emty config entry
    if (size == 0)
    {
        size = 200;
        setDirtyDozenFieldSize(size);
    }
    
    return size;
}

void Config::setDirtyDozenFieldSize(int size)
{
    this->settings->setValue("dirty_dozen/dirty_dozen_item_size", size);
}
