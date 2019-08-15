#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QSettings>

#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    QDir *getDecksPath();
    QString getLastLanguageFilter();
    void setLastLanguageFilter(QString filter);
    
private:
    QSettings *settings;
    QDir *deckpath;
    
signals:
    
public slots:
};

#endif // CONFIG_H
