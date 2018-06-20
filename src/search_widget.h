#ifndef Q_SEARCH_WIDGET_H
#define Q_SEARCH_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDir>

#include "deck_widget.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QDir *decks_path, QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout;
    QDeckOverviewWidget *deck_widget;
    
signals:
    
public slots:
};

#endif // Q_SEARCH_WIDGET_H
