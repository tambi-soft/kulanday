#ifndef STATS_H
#define STATS_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDir>
#include <QGroupBox>
#include <QScrollArea>
#include <QLabel>

#include "db_adapter.h"
#include "db_adapter_meta.h"

class StatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatsWidget(QDir *deckpath, QWidget *parent = nullptr);
    
private:
    QDir *deckpath;
    
    QVBoxLayout *layout = new QVBoxLayout;
    
    // prefix_ -> QMap<QString,QVariant>
    QMap<QString,QMap<QString,QVariant>> data;
    
    void addPrefixStats(QMap<QString, QVariant> item, QStringList stati_list);
    
signals:
    
public slots:
};

#endif // STATS_H
