#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    QDir *getDecksPath();
    
signals:
    
public slots:
};

#endif // CONFIG_H
