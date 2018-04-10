#include "q_deck_item_widget.h"

QDeckItemWidget::QDeckItemWidget(QString deck_name, QWidget *parent) : QWidget(parent)
{
    this->database = new DbAdapter(deck_name);
    initializeGui();
}

QDeckItemWidget::QDeckItemWidget(QString deck_name, int rowid, QWidget *parent) : QWidget(parent)
{
    this->database = new DbAdapter(deck_name);
    initializeGui();
    
    QList<QMap<QString,QVariant>> data = database->selectDeckItem(rowid);
    qDebug() << data;
    qDebug() << data.length();
}

void QDeckItemWidget::initializeGui()
{
    this->grid = new QGridLayout();
    setLayout(grid);
    
    this->image_view = new QLabel();
    this->name_line = new QLineEdit();
    this->word_line = new QLineEdit();
    this->phonetical_line = new QLineEdit();
    this->translation_line = new QLineEdit();
    this->audio_list_widget = new QAudioListWidget();
    
    QPushButton *import_image_button = new QPushButton("add image from file");
    import_image_button->setIcon(QIcon::fromTheme("document-open"));
    connect(import_image_button, &QPushButton::clicked, this, &QDeckItemWidget::importImageClicked);
    
    QPushButton *delete_image_button = new QPushButton("clear image");
    delete_image_button->setIcon(QIcon::fromTheme("edit-delete"));
    connect(delete_image_button, &QPushButton::clicked, this, &QDeckItemWidget::deleteImageClicked);
    
    QPushButton *new_audio_button = new QPushButton("new audio");
    connect(new_audio_button, &QPushButton::clicked, this, &QDeckItemWidget::newAudioButtonClicked);
    
    QPushButton *save_button = new QPushButton("save");
    connect(save_button, &QPushButton::clicked, this, &QDeckItemWidget::saveButtonClicked);
    
    QLabel *name_label = new QLabel("name:");
    QLabel *word_label = new QLabel("word:");
    QLabel *phonetical_label = new QLabel("phonetical:");
    QLabel *translation_label = new QLabel("translation:");
    
    grid->addWidget(import_image_button, 0, 1);
    grid->addWidget(delete_image_button, 0, 2);
    
    grid->addWidget(image_view, 1, 0, 1, 4, Qt::AlignCenter);
    
    grid->addWidget(name_label, 2, 0);
    grid->addWidget(name_line, 2, 1, 1, 3);
    
    grid->addWidget(word_label, 3, 0);
    grid->addWidget(word_line, 3, 1, 1, 3);
    
    grid->addWidget(phonetical_label, 4, 0);
    grid->addWidget(phonetical_line, 4, 1, 1, 3);
    
    grid->addWidget(translation_label, 5, 0);
    grid->addWidget(translation_line, 5, 1, 1, 3);
    
    grid->addWidget(audio_list_widget, 6, 0, 1, 4);
}

void QDeckItemWidget::importImageClicked()
{
    
}

void QDeckItemWidget::deleteImageClicked()
{
    
}

void QDeckItemWidget::newAudioButtonClicked()
{
    
}

void QDeckItemWidget::saveButtonClicked()
{
    
}
