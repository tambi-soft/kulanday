#include "db_adapter.h"

DbAdapter::DbAdapter(QString deck_name)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    
    QDir path = QDir(QDir::homePath() + "/.tambi/decks/" + deck_name + "/database.sqlite");
    this->db.setDatabaseName(path.path());
    
    if (! db.open())
    {
      qDebug() << "Error: connection with database fail";
    }
    else
    {
      qDebug() << "Database: connection ok";
    }
}

// maps the selected field name to the fetched value
QList<QMap<QString,QVariant>> DbAdapter::dbIteratorToMapList(QSqlQuery query)
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

void DbAdapter::saveDeckItem(QString name, QString word, QString phonetical, QString translation)
{
    
}

int DbAdapter::getDeckItemRowID(QString name, QString word, QString phonetical)
{
    
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItems()
{
    QSqlQuery query("SELECT rowid, order_index, name, word, phonetical, translation, svg_filename, image, created FROM deck ORDER BY order_index");
    
    return dbIteratorToMapList(query);
}

QList<QVariant> DbAdapter::selectDeckItemsWithAudio()
{
    
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckDirtyDozenItems()
{
    QSqlQuery query("SELECT image, rowid, name, word, translation, svg_filename, image, filename\
        FROM\
        (\
            SELECT image, deck.rowid AS rowid, name, word, translation, svg_filename, image, audio.filename\
            FROM deck\
            JOIN audio ON deck.rowid=audio.deck_rowid\
            ORDER BY RANDOM()\
        ) AS T\
        GROUP BY rowid\
        ORDER BY RANDOM()\
        LIMIT 12");
    
    return dbIteratorToMapList(query);
}

QList<QVariant> DbAdapter::selectDeckItem()
{
    
}

void DbAdapter::updateDeckItem(int rowid, QString name, QString word, QString phonetical, QString translation)
{
    
}

void DbAdapter::deleteItem(int rowid)
{
    
}

void DbAdapter::deleteImage(int rowid)
{
    
}

// saveAudioDict(audio_dict, deck_rowid);

QList<QMap<QString,QVariant>> DbAdapter::audioFilenamesForDeckRowID(int rowid)
{
    QSqlQuery query("SELECT rowid, description, filename FROM audio WHERE deck_rowid = :rowid");
    query.bindValue(":rowid", rowid);
    
    return dbIteratorToMapList(query);
}

int DbAdapter::getMaxAudioCount()
{
    QSqlQuery query("SELECT COUNT(*) AS result FROM audio GROUP BY deck_rowid ORDER BY result DESC LIMIT 1");
    return query.exec();
}

void DbAdapter::deleteAudioItem(int rowid)
{
    
}

void DbAdapter::deleteAudioItemByFilename(QString filename)
{
    
}



void DbAdapter::insertImage(int rowid, QString filename)
{
    
}
