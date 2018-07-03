#include "move_item_dialog.h"

MoveItemDialog::MoveItemDialog(QDir *decks_path, QString deck_name, qlonglong rowid, QDialog *parent)
    : QDialog(parent)
    , scroll_layout (new QVBoxLayout)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    this->rowid = rowid;
    
    setWindowTitle("kulanday - move item");
    
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
    // detect witch radiobox is checked
    QString target_deck_name = "";
    for (int i = 0; i < this->scroll_layout->count(); ++i)
    {
        QWidget *widget = this->scroll_layout->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        if (class_name == "QRadioButton")
        {
            QRadioButton *rad = (QRadioButton*)widget;
            if (rad->isChecked())
            {
                target_deck_name = rad->text();
            }
        }
    }
    
    // init db and new deck row entry
    DbAdapter *source_database = new DbAdapter(this->decks_path, this->deck_name);
    
    DbAdapter *target_database = new DbAdapter(this->decks_path, target_deck_name);
    int new_rowid = target_database->newDeckRow();
    
    // copy text and images
    QMap<QString,QVariant> data = source_database->selectDeckItem(rowid).at(0);
    target_database->updateDeckItem(new_rowid, data["name"].toString(), data["word"].toString(), data["phonetical"].toString(), data["translation"].toString());
    
    QFile *image = new QFile(this->decks_path->absolutePath() + "/" + this->deck_name + "/" + data["image"].toString());
    image->copy(this->decks_path->absolutePath() + "/" + target_deck_name + "/" + data["image"].toString());
    
    target_database->insertImageFilename(new_rowid, data["image"].toString());
    
    
    
    // copy audio
    QList<QMap<QString,QVariant>> audio_data = source_database->audioFilenamesForDeckRowID(this->rowid);
    
    for (int i = 0; i < audio_data.length(); ++i)
    {
        QMap<QString,QVariant> data = audio_data.at(i);
        
        QFile *audio = new QFile(this->decks_path->absolutePath() + "/" + this->deck_name + "/" + data["filename"].toString());
        audio->copy(this->decks_path->absolutePath() + "/" + target_deck_name + "/" + data["filename"].toString());
        
        int audio_rowid = target_database->newAudioRow(new_rowid);
        target_database->insertAudioFilename(new_rowid, audio_rowid, data["filename"].toString(), data["description"].toString());
    }
    
    // delete from this deck
    emit deleteRow(this->rowid, deck_name);
    
    // close dialog after work is done
    close();
}

void MoveItemDialog::onCancelButton()
{
    close();
}
