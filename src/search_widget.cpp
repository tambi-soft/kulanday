#include "search_widget.h"

SearchWidget::SearchWidget(QDir *decks_path, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , input_line (new QLineEdit)
{
    this->decks_path = decks_path;
    
    QPushButton *start_search_button = new QPushButton("search");
    connect(start_search_button, &QPushButton::clicked, this, &SearchWidget::onSearchButtonClicked);
    
    connect(this->input_line, &QLineEdit::returnPressed, this, &SearchWidget::onSearchButtonClicked);
    
    QHBoxLayout *h_layout = new QHBoxLayout;
    QWidget *h_widget = new QWidget();
    h_widget->setLayout(h_layout);
    h_layout->addWidget(input_line);
    h_layout->addWidget(start_search_button);
    layout->addWidget(h_widget);
    
    setLayout(layout);
}

void SearchWidget::onSearchButtonClicked()
{
    if (this->deck_widget != nullptr)
    {
        this->deck_widget->deleteLater();
    }
    
    QString search_pattern = this->input_line->text();
    this->deck_widget = new QDeckOverviewWidget(search_pattern, this->decks_path);
    
    this->layout->addWidget(this->deck_widget);
}
