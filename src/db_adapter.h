#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>

class DbAdapter : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase db;
    QString deck_name;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    
    void initializeTables();
public:
    explicit DbAdapter(QDir *decks_path, QString deck_name, QObject *parent = nullptr);
    
    qlonglong newDeckRow();
    qlonglong newAudioRow(int deck_rowid);
    QList<QMap<QString,QVariant>> selectDeckItems();
    QList<QVariant> selectDeckItemsWithAudio();
    QList<QMap<QString,QVariant>> selectDeckDirtyDozenItems();
    QList<QMap<QString,QVariant>> selectDeckItem(int rowid);
    void updateDeckItem(int rowid, QString name, QString word, QString phonetical, QString translation);
    QList<QMap<QString, QVariant> > deleteItem(int rowid);
    void deleteImage(int rowid);
    QList<QMap<QString,QVariant>> audioFilenamesForDeckRowID(qlonglong rowid);
    int getMaxAudioCount();
    void deleteAudio(int rowid);
    void insertAudioFilename(qlonglong deck_rowid, int audio_rowid, QString filename, QString description);
    void insertImageFilename(int rowid, QString filename);
};

#endif // DB_ADAPTER_H
