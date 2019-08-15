#ifndef MOVE_ITEM_DIALOG_H
#define MOVE_ITEM_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QPushButton>
#include <QRadioButton>
#include <QDir>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>

#include "db_adapter.h"
#include "filter_language_combo.h"
#include "config.h"

class MoveItemDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MoveItemDialog(QDir *decks_path, QString deck_name, Config *config, qlonglong rowid, QDialog *parent = nullptr);
    
    void clearDecksList();
    void populateDecksList();
    
private:
    QGridLayout *top_layout;
    QVBoxLayout *scroll_layout;
    
    QWidget *scroll_widget;
    
    QDir *decks_path;
    QString deck_name;
    qlonglong rowid;
    FilterLanguageCombo *combo_name_filter;
    void onComboNameFilterTextChanged(QString text);
    
signals:
    void deleteRow(qlonglong rowid, QString deck_name);
    
public slots:
    
private slots:
    void onMoveButton();
    void onCancelButton();
};

#endif // MOVE_ITEM_DIALOG_H
