#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>

class DbAdapter
{
private:
    QSqlDatabase db;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
public:
    DbAdapter();
    
    void saveDeckItem(QString name, QString word, QString phonetical, QString translation);
    int getDeckItemRowID(QString name, QString word, QString phonetical);
    QList<QMap<QString,QVariant> > selectDeckItems();
    QList<QVariant> selectDeckItemsWithAudio();
    QList<QMap<QString,QVariant> > selectDeckDirtyDozenItems();
    QList<QVariant> selectDeckItem();
    void updateDeckItem(int rowid, QString name, QString word, QString phonetical, QString translation);
    void deleteItem(int rowid);
    void deleteImage(int rowid);
    // saveAudioDict(audio_dict, deck_rowid);
    QList<QMap<QString,QVariant> > audioFilenamesForDeckRowID(int rowid);
    int getMaxAudioCount();
    void deleteAudioItem(int rowid);
    void deleteAudioItemByFilename(QString filename);
    
    void insertImage(int rowid, QString filename);
};

#endif // DB_ADAPTER_H
