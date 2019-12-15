#ifndef DIRTYDOZENWRITEWIDGET_H
#define DIRTYDOZENWRITEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDir>

#include "db_adapter.h"
#include "config.h"

class DirtydozenWriteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirtydozenWriteWidget(QDir *deckpath, QString deckname, Config *config, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QDir *deckpath;
    QString deckname;
    Config *config;
    
signals:
    
public slots:
};

#endif // DIRTYDOZENWRITEWIDGET_H
