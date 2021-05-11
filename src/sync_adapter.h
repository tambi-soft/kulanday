#ifndef SYNCADAPTER_H
#define SYNCADAPTER_H

#include <QObject>

class SyncAdapter : public QObject
{
    Q_OBJECT
public:
    explicit SyncAdapter(QObject *parent = nullptr);
    
signals:
    
};

#endif // SYNCADAPTER_H
