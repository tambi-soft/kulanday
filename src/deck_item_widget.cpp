#include "deck_item_widget.h"

QDeckItemWidget::QDeckItemWidget(QDir *decks_path, QString deck_name, QString last_image_import_path, QWidget *parent) : QWidget(parent)
{
    this->last_image_import_path = last_image_import_path;
    
    if (this->database == nullptr)
    {
        this->database = new DbAdapter(decks_path, deck_name);
    }
    // create an empty row and return the id
    int rowid = this->database->newDeckRow();
    
    populateGui(decks_path, deck_name, rowid);
}

QDeckItemWidget::QDeckItemWidget(QDir *decks_path, QString deck_name, int rowid, QString last_image_import_path, QWidget *parent) : QWidget(parent)
{
    this->last_image_import_path = last_image_import_path;
    
    populateGui(decks_path, deck_name, rowid);
}

void QDeckItemWidget::populateGui(QDir *decks_path, QString deck_name, int rowid)
{
    this->decks_path = decks_path;
    this->rowid = rowid;
    this->deck_name = deck_name;
    
    initializeGui(deck_name, rowid);
    
    if (this->database == nullptr)
    {
        this->database = new DbAdapter(this->decks_path, deck_name);
    }
    QList<QMap<QString,QVariant>> data = database->selectDeckItem(rowid);
    
    this->ignore_item_changes = true;
    this->name_line->setPlainText(data[0]["name"].toString());
    this->word_line->setPlainText(data[0]["word"].toString());
    this->phonetical_line->setPlainText(data[0]["phonetical"].toString());
    this->translation_line->setPlainText(data[0]["translation"].toString());
    this->ignore_item_changes = false;
    
    if (! data[0]["image"].isNull())
    {
        this->image_path = this->decks_path->absolutePath() + "/" + deck_name + "/" + data[0]["image"].toString();
        QPixmap pixmap;
        pixmap.load(this->image_path);
        QPixmap scaled = pixmap.scaled(IMAGE_SIZE, Qt::KeepAspectRatio);
        
        this->image_view->setPixmap(scaled);
        
        this->import_image_button->setEnabled(false);
    }
    
    if (! data[0]["svg_filename"].isNull() && data[0]["image"].isNull())
    {
        QSvgWidget *svg_widget = new QSvgWidget();
        
        svg_widget->load(this->decks_path->absolutePath() + "/" + deck_name + "/" + data[0]["svg_filename"].toString());
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
    this->name_line = new ResizingTextEdit();
    this->word_line = new ResizingTextEdit();
    this->phonetical_line = new ResizingTextEdit();
    this->translation_line = new ResizingTextEdit();
    this->audio_list_widget = new QAudioListTable(this->decks_path, deck_name, rowid);
    
    connect(this->audio_list_widget, &QAudioListTable::changed, this, &QDeckItemWidget::onAudioListChanged);
    
    connect(this->name_line, &ResizingTextEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->word_line, &ResizingTextEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->phonetical_line, &ResizingTextEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    connect(this->translation_line, &ResizingTextEdit::textChanged, this, &QDeckItemWidget::onItemChanged);
    
    this->import_image_button = new QPushButton("add image from file");
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
    this->default_import_path = "";
    QString image_url = QFileDialog::getOpenFileName(this, "Please select an Image File", this->last_image_import_path);
    
    if (image_url != NULL)
    {
        this->last_image_import_path = image_url;
        emit imageImportPathUpdated(this->last_image_import_path);
        QFile filepath(image_url);
        
        QString filename = QUrl(image_url).fileName();
        // copy the image to the deck.
        // if there is already a file with this name, just append an undersore to it (probably before the extension)
        // and try again
        bool success = false;
        while (! success)
        {
            success = filepath.copy(this->decks_path->absolutePath() + "/" + this->deck_name + "/" + filename);
            if (! success)
            {
                filename = filename.replace(".", "_.");
            }
        }
        
        this->database->insertImageFilename(this->rowid, QUrl(image_url).fileName());
        
        QPixmap pixmap;
        pixmap.load(image_url);
        QPixmap scaled = pixmap.scaled(IMAGE_SIZE, Qt::KeepAspectRatio);
        this->image_view->setPixmap(scaled);
        
        this->import_image_button->setEnabled(false);
        this->item_changed = true;
        
        this->image_path = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + QUrl(image_url).fileName();
    }
}

void QDeckItemWidget::deleteImageClicked()
{
    int reply = QMessageBox::question(this, "Delete Image", "sure?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        qDebug() << this->image_path;
        QFile file(this->image_path);
        file.remove();
        
        this->database->deleteImage(this->rowid);
        
        this->image_view->clear();
    }
    
    this->import_image_button->setEnabled(true);
    this->item_changed = true;
}

void QDeckItemWidget::newAudioButtonClicked()
{
    this->audio_list_widget->newAudioLine();
    this->item_changed = true;
}

void QDeckItemWidget::onItemChanged()
{
    if (! this->ignore_item_changes)
    {
        QString name = this->name_line->toPlainText();
        QString word = this->word_line->toPlainText();
        QString phonetical = this->phonetical_line->toPlainText();
        QString translation = this->translation_line->toPlainText();
        
        this->database->updateDeckItem(rowid, name, word, phonetical, translation);
        this->item_changed = true;
    }
}

void QDeckItemWidget::onAudioListChanged()
{
    this->item_changed = true;
}

void QDeckItemWidget::hideEvent(QHideEvent *event)
{
    this->audio_list_widget->stopAudio();
    
    if (this->item_changed)
    {
        this->item_changed = false;
        emit contentsUpdated(this->deck_name);
    }
}
