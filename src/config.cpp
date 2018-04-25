#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    
}

QDir *Config::getDecksPath()
{
    QDir *dir = new QDir(QDir::homePath() + "/.tambi/decks/");
    return dir;
}
