#ifndef DB_ADAPTER_META_H
#define DB_ADAPTER_META_H

#include <QObject>

class DbAdapterMeta : public QObject
{
    Q_OBJECT
public:
    explicit DbAdapterMeta(QObject *parent = nullptr);
    
signals:
    
public slots:
};

#endif // DB_ADAPTER_META_H