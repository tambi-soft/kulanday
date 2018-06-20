#include "search_widget.h"

SearchWidget::SearchWidget(QDir *decks_path, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    
    
    QDeckOverviewWidget *deck_widget = new QDeckOverviewWidget(decks_path);
    layout->addWidget(deck_widget);
    
    setLayout(layout);
}
