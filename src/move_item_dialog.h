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

class MoveItemDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MoveItemDialog(QDir *decks_path, QDialog *parent = nullptr);
    
private:
    //QVBoxLayout *scroll_layout;
    //QScrollArea *scroll_area;
    //QWidget *scroll_widget;
    //QGridLayout *grid;
    
signals:
    
public slots:
    
private slots:
    void onMoveButton();
    void onCancelButton();
};

#endif // MOVE_ITEM_DIALOG_H
