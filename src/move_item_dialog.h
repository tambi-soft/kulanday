#ifndef MOVE_ITEM_DIALOG_H
#define MOVE_ITEM_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QDir>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>

#include "db_adapter.h"

class MoveItemDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MoveItemDialog(QDir *decks_path, QString deck_name, qlonglong rowid, QDialog *parent = nullptr);
    
private:
    QDir *decks_path;
    QString deck_name;
    qlonglong rowid;
    
    QVBoxLayout *scroll_layout;
    
signals:
    void deleteRow(qlonglong rowid, QString deck_name);
    
public slots:
    
private slots:
    void onMoveButton();
    void onCancelButton();
};

#endif // MOVE_ITEM_DIALOG_H
