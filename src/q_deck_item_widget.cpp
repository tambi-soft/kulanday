#include "q_deck_item_widget.h"

QDeckItemWidget::QDeckItemWidget(QString deck_name, QWidget *parent) : QWidget(parent)
{
    if (this->database == nullptr)
    {
        this->database = new DbAdapter(deck_name);
    }
    // create an empty row and return the id
    int rowid = this->database->newDeckRow();
    
    QDeckItemWidget(deck_name, rowid);
}

QDeckItemWidget::QDeckItemWidget(QString deck_name, int rowid, QWidget *parent) : QWidget(parent)
{
    this->rowid = rowid;
    initializeGui(deck_name, rowid);
    
    if (this->database == nullptr)
    {
        this->database = new DbAdapter(deck_name);
    }
    QList<QMap<QString,QVariant>> data = database->selectDeckItem(rowid);
    
    this->ignore_item_changes = true;
    this->name_line->setText(data[0]["name"].toString());
    this->word_line->setText(data[0]["word"].toString());
    this->phonetical_line->setText(data[0]["phonetical"].toString());
    this->translation_line->setText(data[0]["translation"].toString());
    this->ignore_item_changes = false;
    
    if (! data[0]["image"].isNull())
    {
        QString image_path = QDir::homePath() + "/.tambi/decks/" + deck_name + "/" + data[0]["image"].toString();
        QPixmap pixmap;
        pixmap.load(image_path);
        QPixmap scaled = pixmap.scaled(IMAGE_SIZE, Qt::KeepAspectRatio);
        
        this->image_view->setPixmap(scaled);
    }
    
    if (! data[0]["svg_filename"].isNull() && data[0]["image"].isNull())
    {
        QSvgWidget *svg_widget = new QSvgWidget();
        
        svg_widget->load(QDir::homePath() + "/.tambi/decks/" + deck_name + "/" + data[0]["svg_filename"].toString());
        svg_widget->setFixedSize(IMAGE_SIZE);
        
        QPixmap pixmap(svg_widget->size());
        svg_widget->render(&pixmap);
        
        this->image_view->setPixmap(pixmap);
    }
}

void QDeckItemWidget::initializeGui(QString deck_name, int rowid)
{
    this->grid = new QGridLayout();
    setLayout(grid);
    
    this->image_view = new QLabel();
    this->name_line = new QLineEdit();
    this->word_line = new QLineEdit();
    this->phonetical_line = new QLineEdit();
    this->translation_line = new QLineEdit();
    this->audio_list_widget = new QAudioListTable(deck_name, rowid);
    
    connect(this->name_line, &QLineEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->word_line, &QLineEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->phonetical_line, &QLineEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->translation_line, &QLineEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    
    QPushButton *import_image_button = new QPushButton("add image from file");
    import_image_button->setIcon(QIcon::fromTheme("document-open"));
    connect(import_image_button, &QPushButton::clicked, this, &QDeckItemWidget::importImageClicked);
    
    QPushButton *delete_image_button = new QPushButton("clear image");
    delete_image_button->setIcon(QIcon::fromTheme("edit-delete"));
    connect(delete_image_button, &QPushButton::clicked, this, &QDeckItemWidget::deleteImageClicked);
    
    QPushButton *new_audio_button = new QPushButton("new audio");
    connect(new_audio_button, &QPushButton::clicked, this, &QDeckItemWidget::newAudioButtonClicked);
    
    //QPushButton *save_button = new QPushButton("save");
    //connect(save_button, &QPushButton::clicked, this, &QDeckItemWidget::saveButtonClicked);
    
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
    grid->addWidget(new_audio_button, 7, 0);
    
    //grid->addWidget(save_button, 7, 3);
    
    if (QSysInfo::kernelType() != "darwin") // font-rendering is quite a mess on mac and looks uselessly uggly here
    {
        QString style = "QLineEdit{ font-size: 20px; }";
        setStyleSheet(style);
    }
}

void QDeckItemWidget::importImageClicked()
{
    
}

void QDeckItemWidget::deleteImageClicked()
{
    
}

void QDeckItemWidget::newAudioButtonClicked()
{
    this->audio_list_widget->newAudioLine();
}

void QDeckItemWidget::onItemChanged()
{
    if (! this->ignore_item_changes)
    {
        QString name = this->name_line->text();
        QString word = this->word_line->text();
        QString phonetical = this->phonetical_line->text();
        QString translation = this->translation_line->text();
        
        this->database->updateDeckItem(rowid, name, word, phonetical, translation);
    }
}
