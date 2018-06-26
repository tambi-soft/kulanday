#include "move_item_dialog.h"

MoveItemDialog::MoveItemDialog(QDir *decks_path, QString deck_name, qlonglong rowid, QDialog *parent)
    : QDialog(parent)
    , scroll_layout (new QVBoxLayout)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    this->rowid = rowid;
    
    QGridLayout *top_layout = new QGridLayout;
    
    QWidget *scroll_widget = new QWidget;
    QScrollArea *scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(scroll_widget);
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);    
    foreach (QString deck, decks_names)
    {
        if (deck != deck_name)
        {
            QRadioButton *radio = new QRadioButton(deck);
            this->scroll_layout->addWidget(radio);
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
    DbAdapter *database = new DbAdapter(this->decks_path, this->deck_name);
    
    // copy text and images
    QList<QMap<QString,QVariant>> data = database->selectDeckItem(rowid);
    qDebug() << data;
    
    
    // copy audio
    QList<QMap<QString,QVariant>> audio_data = database->audioFilenamesForDeckRowID(this->rowid);
    qDebug() << audio_data;
    
    // delete from this deck
    //emit deleteRow(this->rowid, deck_name);
    
    // close dialog after work is done
    close();
}

void MoveItemDialog::onCancelButton()
{
    close();
}
