#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QSettings>
#include <QSize>

#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    QDir *getDecksPath();
    QString getDeckpathString();
    void setDeckpath(QString path);
    QString getAudioEditorPath();
    void setAudioEditorPath(QString path);
    QString getLastLanguageFilter();
    void setLastLanguageFilter(QString filter);
    QSize getDirtyDozenSize();
    void setDirtyDozenSize(QSize size);
    int getDirtyDozenFieldSize();
    void setDirtyDozenFieldSize(int size);
    double getDirtyDozenAspectRatio();
    void setDirtyDozenAspectRatio(double aspect);
    
private:
    QString deckpath_var;
    QSettings *settings;
    QDir *deckpath;
    
signals:
    
public slots:
};

#endif // CONFIG_H
