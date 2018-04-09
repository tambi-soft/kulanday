#ifndef Q_CREATE_NEW_DECK_WIDGET_H
#define Q_CREATE_NEW_DECK_WIDGET_H

#include <QWidget>
#include <QDir>

class QCreateNewDeckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCreateNewDeckWidget(QDir *decks_path, QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // Q_CREATE_NEW_DECK_WIDGET_H
