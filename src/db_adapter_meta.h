#ifndef DB_ADAPTER_META_H
#define DB_ADAPTER_META_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>
#include <QDateTime>

class DbAdapterMeta : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase db;
    QString deck_name;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    QMap<QString,QString> dbIteratorToMap(QSqlQuery query);
    
    void initializeTables();
public:
    explicit DbAdapterMeta(QDir *decks_path, QObject *parent = nullptr);
    
    void populateDecksTable(QStringList decks_names);
    QMap<QString, QString> selectDecksStati();
    QMap<QString, QString> selectDecksLearned();
    void updateDeckStatus(QString deck_name, QString status);
    void updateLastLearned(QString deck_name);
signals:
    
public slots:
};

#endif // DB_ADAPTER_META_H
