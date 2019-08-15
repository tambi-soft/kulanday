#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>
#include <QDateTime>

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
    qlonglong newAudioRow(qlonglong deck_rowid);
    QList<QMap<QString,QVariant>> selectDeckItems();
    QList<QMap<QString,QVariant>> selectDeckItemsFiltered(QString filter);
    QList<QMap<QString,QVariant>> selectDeckDirtyDozenItems();
    QList<QMap<QString,QVariant>> selectDeckItem(qlonglong rowid);
    void updateDeckItem(qlonglong rowid, QString name, QString word, QString phonetical, QString translation);
    QList<QMap<QString, QVariant> > deleteItem(qlonglong rowid);
    void deleteImage(qlonglong rowid);
    QList<QMap<QString,QVariant>> audioFilenamesForDeckRowID(qlonglong rowid);
    void deleteAudio(qlonglong rowid);
    void insertAudioFilename(qlonglong deck_rowid, qlonglong audio_rowid, QString filename, QString description);
    void insertImageFilename(qlonglong rowid, QString filename);
};

#endif // DB_ADAPTER_H
