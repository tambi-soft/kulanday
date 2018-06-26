#include "move_item_dialog.h"

MoveItemDialog::MoveItemDialog(QDir *decks_path, QString deck_name, QDialog *parent)
    : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    QGridLayout *top_layout = new QGridLayout;
    
    QWidget *scroll_widget = new QWidget;
    QScrollArea *scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(scroll_widget);
    
    QVBoxLayout *scroll_layout = new QVBoxLayout;
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);    
    foreach (QString deck, decks_names)
    {
        if (deck != deck_name)
        {
            QRadioButton *radio = new QRadioButton(deck);
            scroll_layout->addWidget(radio);
        }
    }
    
    scroll_widget->setLayout(scroll_layout);
    
    top_layout->addWidget(scroll_area, 0, 0, 1, 2);
    
    QPushButton *move_button = new QPushButton("move");
    QPushButton *cancel_button = new QPushButton("cancel");
    top_layout->addWidget(move_button, 1, 0);
    top_layout->addWidget(cancel_button, 1, 1);
    
    connect(move_button, &QPushButton::clicked, this, &MoveItemDialog::onMoveButton);
    connect(cancel_button, &QPushButton::clicked, this, &MoveItemDialog::onCancelButton);
    
    setLayout(top_layout);
}

void MoveItemDialog::onMoveButton()
{
    
}

void MoveItemDialog::onCancelButton()
{
    close();
}
