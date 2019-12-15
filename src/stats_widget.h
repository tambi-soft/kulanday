#ifndef STATS_H
#define STATS_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDir>
#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>
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
    int scrollbar_pos;
    
    QVBoxLayout *layout;
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = nullptr;
    
    void showData();
    
    void addPrefixStats(QMap<QString, QVariant> item, QStringList stati_list);
    
signals:
    
public slots:
    void showEvent(QShowEvent */*event*/);
    
private slots:
    void scrollBarRangeChanged(int /*min*/, int max);
};

#endif // STATS_H
