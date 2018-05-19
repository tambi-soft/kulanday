#include "db_adapter_meta.h"

DbAdapterMeta::DbAdapterMeta(QDir *decks_path, QObject *parent) : QObject(parent)
{
    if (! QSqlDatabase::contains(deck_name))
    {
        QSqlDatabase::addDatabase("QSQLITE", deck_name);
    }
    
    this->db = QSqlDatabase::database(deck_name);
    
    QString path = decks_path->absolutePath() + "/decks_meta.sqlite";
    this->db.setDatabaseName(path);
    
    if (! db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        //qDebug() << "Database: connection ok";
    }
    
    initializeTables();
}

// maps the selected field name to the fetched value
QList<QMap<QString,QVariant>> DbAdapterMeta::dbIteratorToMapList(QSqlQuery query)
{
    QList<QMap<QString,QVariant>> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        QMap<QString,QVariant> map;
        for (int i = 0; i <= rec.count(); ++i)
        {
            QString name = rec.fieldName(i);
            QString value = query.value(i).toString();
            
            map[name] = value;
        }
        
        result.append(map);
    }
    
    return result;
}

QMap<QString,QString> DbAdapterMeta::dbIteratorToMap(QSqlQuery query)
{
    QMap<QString,QString> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        result[ query.value(0).toString() ] = query.value(1).toString();
    }
    
    return result;
}

void DbAdapterMeta::initializeTables()
{
    QSqlQuery query_deck("CREATE TABLE IF NOT EXISTS decks (rowid INTEGER PRIMARY KEY AUTOINCREMENT, deck_name TEXT UNIQUE, status TEXT, last_learned NUMERIC, last_modified NUMERIC)", this->db);
}

void DbAdapterMeta::populateDecksTable(QStringList decks_names)
{
    foreach (QString deck_name, decks_names)
    {
        QSqlQuery query(this->db);
        query.prepare("INSERT OR IGNORE INTO decks (deck_name, status) VALUES (:deck_name, 'none')");
        query.bindValue(":deck_name", deck_name);
        query.exec();
    }
}

QMap<QString,QString> DbAdapterMeta::selectDecksStati()
{
    QSqlQuery query("SELECT deck_name, status FROM decks", this->db);
    
    return dbIteratorToMap(query);
}

QMap<QString,QString> DbAdapterMeta::selectDecksLearned()
{
    QSqlQuery query("SELECT deck_name, last_learned FROM decks", this->db);
    
    return dbIteratorToMap(query);
}

void DbAdapterMeta::updateDeckStatus(QString deck_name, QString status)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE decks SET status=:status WHERE deck_name=:deck_name");
    query.bindValue(":deck_name", deck_name);
    query.bindValue(":status", status);
    query.exec();
}

void DbAdapterMeta::updateLastLearned(QString deck_name)
{
    QDateTime *dtime = new QDateTime();
    
    QSqlQuery query(this->db);
    query.prepare("UPDATE decks SET last_learned=:last_learned WHERE deck_name=:deck_name");
    query.bindValue(":deck_name", deck_name);
    query.bindValue(":last_learned", dtime->currentDateTime().toTime_t());
    query.exec();
}
