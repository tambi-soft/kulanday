#include "q_learn_widget.h"

QLearnWidget::QLearnWidget(QDir *decks_path, QString deck_name, QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
    , audioPlayer (new QMediaPlayer)
    , unicodeFonts (new UnicodeFonts)
{
    DISPLAY_COMBO_ITEMS << "image" << "name" << "word" << "translation";
    this->decks_path = decks_path;
    
    setLayout(grid);
    
    this->deck_name = deck_name;
    
    initialize(deck_name);
    update();
}

void QLearnWidget::initialize(QString deck_name)
{
    DbAdapter *db_adapter = new DbAdapter(this->decks_path, deck_name);
    this->dataset = db_adapter->selectDeckDirtyDozenItems();
    
    this->select_display_combo = new QComboBox();
    select_display_combo->addItems(DISPLAY_COMBO_ITEMS);
    select_display_combo->setCurrentText(this->learn_mode);
    
    connect(select_display_combo, &QComboBox::currentTextChanged, this, &QLearnWidget::selectDisplayCurrentTextChanged);
    
    this->grid->addWidget(select_display_combo, 0, 0);
    
    QPushButton *shuffle_button = new QPushButton("shuffle");
    shuffle_button->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
    connect(shuffle_button, &QPushButton::clicked, this, &QLearnWidget::onShuffleButtonClicked);
    this->grid->addWidget(shuffle_button, int(12 / this->COLUMNS +2), this->COLUMNS-1);
}

void QLearnWidget::update()
{
    clear();
    
    for (int i = 0; i < this->dataset.length(); ++i)
    {
        qDebug() << i;
        QPushButton *button = new QPushButton();
        
        if (this->select_display_combo->currentText() == "image")
        {
            QPixmap *pixmap = new QPixmap();
            QString image_path = this->decks_path->absolutePath() + "/" + deck_name + "/" + dataset.at(i)["image"].toString();
            QFile image_file(image_path);
            if (image_file.exists() && dataset.at(i)["image"].toString() != "")
            {
                pixmap->load(image_path);
            }
            else
            {
                QString svg_path = this->decks_path->absolutePath() + "/" + deck_name + "/" + dataset.at(i)["svg_filename"].toString();
                QFile svg_file(svg_path);
                if (svg_file.exists())
                {
                    pixmap->load(svg_path);
                }
            }
            QPixmap scaled_pixmap = pixmap->scaled(QSize(200, 200), Qt::KeepAspectRatio);
            QIcon icon(scaled_pixmap);
            button->setIcon(icon);
            button->setIconSize(QSize(190, 190));
        }
        else if (this->select_display_combo->currentText() == "name")
        {
            button->setText(dataset.at(i)["name"].toString());
        }
        else if (this->select_display_combo->currentText() == "word")
        {
            button->setText(dataset.at(i)["word"].toString());
        }
        else if (this->select_display_combo->currentText() == "translation")
        {
            button->setText(dataset.at(i)["translation"].toString());
        }
        
        QFont font = unicodeFonts->getFontAndSize(button->text());
        button->setFont(font);
        
        connect(button, &QPushButton::clicked, this, [this, i]{ displayButtonClicked(i); });
        
        int row = i + this->COLUMNS;
        this->grid->addWidget(button, int(row / COLUMNS), row % COLUMNS);
        
        this->button_list.append(button);
    }
}

void QLearnWidget::clear()
{
    /*
    for (int i = 0; i < layout()->count(); ++i)
    {
        QWidget *widget = layout()->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        if (class_name == "QClickLabel")
        {
            widget->setVisible(false);
            widget->deleteLater();
        }
    }
    */
    foreach (QPushButton* button, this->button_list)
    {
        button->deleteLater();
    }
    
    this->button_list.clear();
}

void QLearnWidget::displayButtonClicked(int rowid)
{
    playAudio(rowid);
}

void QLearnWidget::playAudio(int selector)
{
    QString audio_filename = this->dataset.at(selector)["filename"].toString();
    
    QString audio_path = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + audio_filename;
    QUrl audio_url = QUrl::fromLocalFile(audio_path);
    this->audioPlayer->setMedia(QMediaContent(audio_url));
    this->audioPlayer->play();
}

void QLearnWidget::selectDisplayCurrentTextChanged(QString text)
{
    clear();
    this->learn_mode = text;
    update();
}

void QLearnWidget::onShuffleButtonClicked()
{
    clear();
    this->dataset = shuffleList(this->dataset);
    update();
}

QList<QMap<QString,QVariant>> QLearnWidget::shuffleList(QList<QMap<QString,QVariant>> list)
{
    qDebug() << "shuffle";
    for (int i = 0; i < 100; ++i)
    {
        int rnd = qrand() % list.length();
        QMap<QString,QVariant> item = list.at(rnd);
        
        list.removeAt(rnd);
        list.append(item);
    }
    
    return list;
}

void QLearnWidget::hideEvent(QHideEvent *event)
{
    audioPlayer->stop();
}
