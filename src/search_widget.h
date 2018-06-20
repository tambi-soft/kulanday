#ifndef Q_SEARCH_WIDGET_H
#define Q_SEARCH_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QLineEdit>
#include <QPushButton>

#include "deck_widget.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QDir *decks_path, QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout;
    QLineEdit *input_line;
    QPushButton *start_search_button;
    QDeckOverviewWidget *deck_widget;
    
signals:
    
public slots:
};

#endif // Q_SEARCH_WIDGET_H
