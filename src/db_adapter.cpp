#include "db_adapter.h"

DbAdapter::DbAdapter(QString deck_name, QObject *parent) : QObject(parent)
{
    this->deck_name = deck_name;
    
    if (! QSqlDatabase::contains(deck_name))
    {
        QSqlDatabase::addDatabase("QSQLITE", deck_name);
    }
    
    this->db = QSqlDatabase::database(deck_name);
    
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
    
    initializeTables();
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

void DbAdapter::initializeTables()
{
    QSqlQuery query_deck("CREATE TABLE IF NOT EXISTS deck (rowid INTEGER PRIMARY KEY AUTOINCREMENT, order_index INTEGER, name TEXT, word TEXT, phonetical TEXT, translation TEXT, svg_filename TEXT, image TEXT, created NUMERIC, known NUMERIC, priority NUMERIC, changed NUMERIC)", this->db);
    query_deck.exec();
    
    QSqlQuery query_audio("CREATE TABLE IF NOT EXISTS audio (rowid INTEGER PRIMARY KEY AUTOINCREMENT, deck_rowid INTEGER, description TEXT, filename TEXT)", this->db);
    query_audio.exec();
}

void DbAdapter::saveDeckItem(QString name, QString word, QString phonetical, QString translation)
{
    
}

int DbAdapter::getDeckItemRowID(QString name, QString word, QString phonetical)
{
    
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItems()
{
    QSqlQuery query("SELECT rowid, order_index, name, word, phonetical, translation, svg_filename, image, created FROM deck ORDER BY order_index", this->db);
    
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
        LIMIT 12", this->db);
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItem(int rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name, word, phonetical, translation, svg_filename, image FROM deck WHERE rowid = :rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMapList(query);
}

void DbAdapter::updateDeckItem(int rowid, QString name, QString word, QString phonetical, QString translation)
{
    
}

QList<QVariant> DbAdapter::deleteItem(int rowid)
{
    QList<QVariant> result;
    
    QSqlQuery select_svg(this->db);
    select_svg.prepare("SELECT svg_filename FROM deck WHERE rowid = :rowid");
    select_svg.bindValue(":rowid", rowid);
    select_svg.exec();
    QList<QMap<QString,QVariant>> svg_list = dbIteratorToMapList(select_svg);
    result.append( svg_list.at(0)["svg_filename"].toString() );
    
    QSqlQuery select_image(this->db);
    select_image.prepare("SELECT image FROM deck WHERE rowid = :rowid");
    select_image.bindValue(":rowid", rowid);
    select_image.exec();
    QList<QMap<QString,QVariant>> image_list = dbIteratorToMapList(select_image);
    result.append( image_list.at(0)["image"].toString() );
    
    QSqlQuery select_audio(this->db);
    select_audio.prepare("SELECT filename FROM audio WHERE deck_rowid = :rowid");
    select_audio.bindValue(":rowid", rowid);
    select_audio.exec();
    QList<QMap<QString,QVariant>> audio_list = dbIteratorToMapList(select_audio);
    qDebug() << audio_list;
    
    qDebug() << ":::::::::::";
    
    /*
    QSqlQuery delete_deck_row(this->db);
    delete_deck_row.prepare("DELETE FROM deck WHERE rowid = :rowid");
    delete_deck_row.bindValue(":rowid", rowid);
    delete_deck_row.exec();
    
    QSqlQuery delete_audio(this->db);
    delete_audio.prepare("DELETE FROM audio WHERE deck_rowid = :rowid");
    delete_audio.bindValue(":rowid", rowid);
    delete_audio.exec();
    */
    return result;
}

void DbAdapter::deleteImage(int rowid)
{
    
}

// saveAudioDict(audio_dict, deck_rowid);

QList<QMap<QString,QVariant>> DbAdapter::audioFilenamesForDeckRowID(int rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid, description, filename FROM audio WHERE deck_rowid = :rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMapList(query);
}

int DbAdapter::getMaxAudioCount()
{
    QSqlQuery query("SELECT COUNT(*) AS result FROM audio GROUP BY deck_rowid ORDER BY result DESC LIMIT 1", this->db);
    
    QList<QMap<QString,QVariant>> list = dbIteratorToMapList(query);
    
    if (list.length() > 0 && list.at(0).contains("result"))
    {
        return list.at(0)["result"].toInt();
    }
    else
    {
        return 0;
    }
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
