#include "search_widget.h"

SearchWidget::SearchWidget(QDir *decks_path, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , input_line (new QLineEdit)
{
    QPushButton *start_search_button = new QPushButton("search");
    
    QHBoxLayout *h_layout = new QHBoxLayout;
    QWidget *h_widget = new QWidget();
    h_widget->setLayout(h_layout);
    h_layout->addWidget(input_line);
    h_layout->addWidget(start_search_button);
    layout->addWidget(h_widget);
    
    QDeckOverviewWidget *deck_widget = new QDeckOverviewWidget("buildin", decks_path);
    layout->addWidget(deck_widget);
    
    setLayout(layout);
}
