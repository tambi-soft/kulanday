#include "move_item_dialog.h"

MoveItemDialog::MoveItemDialog(QDir *decks_path, QDialog *parent)
    : QDialog(parent)
//    , scroll_layout (new QVBoxLayout)
//    , scroll_widget (new QWidget)
//    , grid (new QGridLayout)
{
    //this->setGeometry(100, 100, 260, 260);
    
    QWidget *top_widget = new QWidget();
    QGridLayout *top_layout = new QGridLayout();
    setLayout(top_layout);
    top_layout->addWidget(top_widget, 0, 0, 1, 2);
    
    //top_widget->setGeometry(10, 10, 400, 500);
    QScrollArea *scroll_area = new QScrollArea(top_widget);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_area->setWidgetResizable(true);
    //scroll_area->setGeometry(10, 10, 400, 500);
    
    QWidget *widget = new QWidget();
    scroll_area->setWidget(widget);
    
    QVBoxLayout *scroll_layout = new QVBoxLayout();
    widget->setLayout(scroll_layout);
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);    
    foreach (QString deck, decks_names)
    {
        QRadioButton *radio = new QRadioButton(deck);
        scroll_layout->addWidget(radio);
    }
    
    QPushButton *move_button = new QPushButton("move");
    QPushButton *cancel_button = new QPushButton("cancel");
    top_layout->addWidget(move_button, 1, 0);
    top_layout->addWidget(cancel_button, 1, 1);
}

void MoveItemDialog::onMoveButton()
{
    
}

void MoveItemDialog::onCancelButton()
{
    destroy();
}
