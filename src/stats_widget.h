#ifndef STATS_H
#define STATS_H

#include <QObject>
#include <QWidget>

#include <QDir>

class StatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatsWidget(QDir *deckpath, QWidget *parent = nullptr);
    
private:
    QDir *deckpath;
    
signals:
    
public slots:
};

#endif // STATS_H
