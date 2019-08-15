#include "db_adapter.h"

DbAdapter::DbAdapter(QDir *decks_path, QString deck_name, QObject *parent) : QObject(parent)
{
    this->deck_name = deck_name;
    
    if (! QSqlDatabase::contains(deck_name))
    {
        QSqlDatabase::addDatabase("QSQLITE", deck_name);
    }
    
    this->db = QSqlDatabase::database(deck_name);
    
    QString path = decks_path->absolutePath() + "/" + deck_name + "/database.sqlite";
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
QList<QMap<QString,QVariant>> DbAdapter::dbIteratorToMapList(QSqlQuery query)
{
    QList<QMap<QString,QVariant>> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        QMap<QString,QVariant> map;
        map["[deck_name]"] = this->deck_name;
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
    
    QSqlQuery query_audio("CREATE TABLE IF NOT EXISTS audio (rowid INTEGER PRIMARY KEY AUTOINCREMENT, deck_rowid INTEGER, description TEXT, filename TEXT)", this->db);
}

//void DbAdapter::saveDeckItem(QString name, QString word, QString phonetical, QString translation)
qlonglong DbAdapter::newDeckRow()
{
    QDateTime *dtime = new QDateTime();
    
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO deck (created) VALUES (:created)");
    query.bindValue(":created", dtime->currentDateTime().toTime_t());
    query.exec();
    
    return query.lastInsertId().toLongLong();
}

qlonglong DbAdapter::newAudioRow(qlonglong deck_rowid)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO audio (deck_rowid) VALUES (:deck_rowid)");
    query.bindValue(":deck_rowid", deck_rowid);
    query.exec();
    
    return query.lastInsertId().toLongLong();
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItems()
{
    // concerning the GROUP_CONCAT:
    // sqlite does not support ordering here, so we have to build one ourselves by concating rowid:filename.
    // of course, afterwards we have to split this in code
    QSqlQuery query("SELECT deck.rowid, order_index, name, word, phonetical, translation, svg_filename, image, created, \
                    GROUP_CONCAT(audio_new.rowid || \":\" || audio_new.filename) AS audio\
                     FROM deck\
                    LEFT JOIN (\
                    SELECT rowid, deck_rowid, description, IFNULL(filename, 'none') AS filename FROM audio\
                    ) AS audio_new ON audio_new.deck_rowid = deck.rowid\
                    GROUP BY deck.rowid\
                    ORDER BY order_index", this->db);
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItemsFiltered(QString filter)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT deck.rowid, order_index, name, word, phonetical, translation, svg_filename, image, created, GROUP_CONCAT(audio.filename, \",\") AS audio\
        FROM deck\
        LEFT JOIN audio ON audio.deck_rowid = deck.rowid\
        WHERE name LIKE ? OR word LIKE ? OR phonetical LIKE ? OR translation LIKE ?\
        GROUP BY deck.rowid\
        ORDER BY order_index");
    query.bindValue(0, "%"+filter+"%");
    query.bindValue(1, "%"+filter+"%");
    query.bindValue(2, "%"+filter+"%");
    query.bindValue(3, "%"+filter+"%");
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckDirtyDozenItems(int limit)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT image, rowid, name, word, phonetical, translation, svg_filename, image, filename\
        FROM\
        (\
            SELECT image, deck.rowid AS rowid, name, word, phonetical, translation, svg_filename, image, audio.filename\
            FROM deck\
            JOIN audio ON deck.rowid=audio.deck_rowid\
            ORDER BY RANDOM()\
        ) AS T\
        WHERE T.filename IS NOT NULL\
        GROUP BY rowid\
        ORDER BY RANDOM()\
        LIMIT :limit");
    query.bindValue(":limit", limit);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectDeckItem(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name, word, phonetical, translation, svg_filename, image FROM deck WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMapList(query);
}

void DbAdapter::updateDeckItem(qlonglong rowid, QString name, QString word, QString phonetical, QString translation)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE deck SET name=:name, word=:word, phonetical=:phonetical, translation=:translation WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.bindValue(":name", name);
    query.bindValue(":word", word);
    query.bindValue(":phonetical", phonetical);
    query.bindValue(":translation", translation);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::deleteItem(qlonglong rowid)
{
    QList<QMap<QString,QVariant>> result;
    
    QSqlQuery select_svg(this->db);
    select_svg.prepare("SELECT svg_filename FROM deck WHERE rowid=:rowid");
    select_svg.bindValue(":rowid", rowid);
    select_svg.exec();
    QList<QMap<QString,QVariant>> svg_list = dbIteratorToMapList(select_svg);
    QMap<QString,QVariant> svg_map;
    svg_map.insert("filename", svg_list.at(0)["svg_filename"].toString());
    result.append( svg_map );
    
    QSqlQuery select_image(this->db);
    select_image.prepare("SELECT image FROM deck WHERE rowid = :rowid");
    select_image.bindValue(":rowid", rowid);
    select_image.exec();
    QList<QMap<QString,QVariant>> image_list = dbIteratorToMapList(select_image);
    QMap<QString,QVariant> image_map;
    image_map.insert("filename", image_list.at(0)["image"].toString());
    result.append( image_map );
    
    QSqlQuery select_audio(this->db);
    select_audio.prepare("SELECT filename FROM audio WHERE deck_rowid = :rowid");
    select_audio.bindValue(":rowid", rowid);
    select_audio.exec();
    QList<QMap<QString,QVariant>> audio_list = dbIteratorToMapList(select_audio);
    result.append(audio_list);
    
    
    QSqlQuery delete_deck_row(this->db);
    delete_deck_row.prepare("DELETE FROM deck WHERE rowid = :rowid");
    delete_deck_row.bindValue(":rowid", rowid);
    delete_deck_row.exec();
    
    QSqlQuery delete_audio(this->db);
    delete_audio.prepare("DELETE FROM audio WHERE deck_rowid = :rowid");
    delete_audio.bindValue(":rowid", rowid);
    delete_audio.exec();
    
    return result;
}

void DbAdapter::deleteImage(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE deck SET image=NULL WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::audioFilenamesForDeckRowID(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid, description, filename FROM audio WHERE deck_rowid = :rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMapList(query);
}

void DbAdapter::deleteAudio(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM audio WHERE rowid = :rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

void DbAdapter::insertAudioFilename(qlonglong deck_rowid, qlonglong audio_rowid, QString filename, QString description)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE audio SET deck_rowid = :deck_rowid, filename=:filename, description = :description WHERE rowid = :audio_rowid");
    query.bindValue(":deck_rowid", deck_rowid);
    query.bindValue(":audio_rowid", audio_rowid);
    query.bindValue(":filename", filename);
    query.bindValue(":description", description);
    query.exec();
}

void DbAdapter::insertImageFilename(qlonglong rowid, QString filename)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE deck SET image=:filename WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.bindValue(":filename", filename);
    query.exec();
}
