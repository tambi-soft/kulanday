#ifndef Q_CREATE_NEW_DECK_WIDGET_H
#define Q_CREATE_NEW_DECK_WIDGET_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QGridLayout>

class QCreateNewDeckDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit QCreateNewDeckDialog(QDir *decks_path, QFileDialog *parent = nullptr);
    
private:
    //QGridLayout *grid;
    //QDir *new_deck_dir;
    
signals:
    
public slots:
};

#endif // Q_CREATE_NEW_DECK_WIDGET_H
