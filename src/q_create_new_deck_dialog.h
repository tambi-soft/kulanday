#ifndef Q_CREATE_NEW_DECK_WIDGET_H
#define Q_CREATE_NEW_DECK_WIDGET_H

#include <QDialog>
#include <QDir>

class QCreateNewDeckDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QCreateNewDeckDialog(QDir *decks_path, QDialog *parent = nullptr);
    
signals:
    
public slots:
};

#endif // Q_CREATE_NEW_DECK_WIDGET_H
