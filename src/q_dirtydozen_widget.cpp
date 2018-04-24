#include "q_dirtydozen_widget.h"

QDirtyDozenWidget::QDirtyDozenWidget(QString deck_name, QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
    , audioPlayer (new QMediaPlayer)
    , unicodeFonts (new UnicodeFonts)
{
    DISPLAY_COMBO_ITEMS << "image" << "name" << "word" << "translation";
    
    setLayout(grid);
    
    this->deck_name = deck_name;
    
    initialize(deck_name);
    update();
    playNextAudio();
}

void QDirtyDozenWidget::initialize(QString deck_name)
{
    DbAdapter *db_adapter = new DbAdapter(deck_name);
    this->full_dataset = db_adapter->selectDeckDirtyDozenItems();
    
    //QLabel *select_display_combo_label = new QLabel("select display:");
    //this->grid->addWidget(select_display_combo_label, 0, 0);
    
    this->select_display_combo = new QComboBox();
    select_display_combo->addItems(DISPLAY_COMBO_ITEMS);
    select_display_combo->setCurrentText(this->learn_mode);
    
    connect(select_display_combo, &QComboBox::currentTextChanged, this, &QDirtyDozenWidget::selectDisplayCurrentTextChanged);
    
    this->grid->addWidget(select_display_combo, 0, 0);
    
    QPushButton *replay_audio_button = new QPushButton("replay audio");
    replay_audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
    connect(replay_audio_button, &QPushButton::clicked, this, &QDirtyDozenWidget::replayAudioClicked);
    this->grid->addWidget(replay_audio_button, 0, this->COLUMNS-1);
    
    this->show_all_button = new QPushButton("show all");
    connect(show_all_button, &QPushButton::clicked, this, &QDirtyDozenWidget::showAllButtonClicked);
    this->grid->addWidget(show_all_button, int(12 / this->COLUMNS +2), 0);
    
    QPushButton *shuffle_button = new QPushButton("shuffle");
    shuffle_button->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
    connect(shuffle_button, &QPushButton::clicked, this, &QDirtyDozenWidget::onShuffleButtonClicked);
    this->grid->addWidget(shuffle_button, int(12 / this->COLUMNS +2), this->COLUMNS-1);
}

void QDirtyDozenWidget::update()
{
    clear();
    
    this->dataset = this->full_dataset.mid(0, this->counter);
    // we want to play the new item before shuffle
    // (after it is hard to know wich one is the new)
    playNextAudio();
    QMap<QString,QVariant> new_item = this->dataset.last();
    qDebug() << new_item;
    this->dataset = shuffleList(this->dataset);
    
    for (int i = 0; i < this->dataset.length(); ++i)
    {
        QClickLabel *label = new QClickLabel();
        
        if (this->select_display_combo->currentText() == "image")
        {
            QPixmap *pixmap = new QPixmap();
            QString image_path = QDir::homePath() + "/.tambi/decks/" + deck_name + "/" + dataset.at(i)["image"].toString();
            QFile image_file(image_path);
            if (image_file.exists() && dataset.at(i)["image"].toString() != "")
            {
                pixmap->load(image_path);
            }
            else
            {
                QString svg_path = QDir::homePath() + "/.tambi/decks/" + deck_name + "/" + dataset.at(i)["svg_filename"].toString();
                QFile svg_file(svg_path);
                if (svg_file.exists())
                {
                    pixmap->load(svg_path);
                }
            }
            QPixmap scaled_pixmap = pixmap->scaled(QSize(200, 200), Qt::KeepAspectRatio);
            label->setGeometry(scaled_pixmap.rect());
            label->setPixmap(scaled_pixmap);
        }
        else if (this->select_display_combo->currentText() == "name")
        {
            label->setText(dataset.at(i)["name"].toString());
        }
        else if (this->select_display_combo->currentText() == "word")
        {
            label->setText(dataset.at(i)["word"].toString());
        }
        else if (this->select_display_combo->currentText() == "translation")
        {
            label->setText(dataset.at(i)["translation"].toString());
        }
        
        if (dataset.at(i) == new_item)
        {
            label->setStyleSheet("QLabel { background-color : #00a194; }");
        }
        
        QFont font = unicodeFonts->getFontAndSize(label->text());
        label->setFont(font);
        
        label->setAlignment(Qt::AlignCenter);
        connect(label, &QClickLabel::clicked, this, [this, i]{ labelClicked(dataset.at(i)["rowid"].toInt()); });
        
        int row = i + this->COLUMNS;
        this->grid->addWidget(label, int(row / COLUMNS), row % COLUMNS);
        
    }
    
    //playRandomAudio();
}

void QDirtyDozenWidget::clear()
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        QWidget *widget = layout()->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        //if (class_name != "QLabel" && class_name != "QComboBox" && class_name != "QPushButton")
        if (class_name == "QClickLabel")
        {
            widget->setVisible(false);
            widget->deleteLater();
        }
    }
}

void QDirtyDozenWidget::clearStyteSheet()
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        QWidget *widget = layout()->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        if (class_name == "QClickLabel")
        {
            widget->setStyleSheet("");
        }
    }
}

void QDirtyDozenWidget::showAllButtonClicked()
{
    if (this->show_all_button->text() == "show all")
    {
        this->counter = this->full_dataset.length();
        
        this->show_all_button->setText("start over");
        
    }
    else
    {
        this->show_all_button->setText("show all");
        
        this->counter = 1;
        this->delay_counter = 0;
        this->delay = 2;
        
        clear();
    }
    
    update();
}

void QDirtyDozenWidget::labelClicked(int rowid)
{
    clearStyteSheet();
    
    // only proceed if answer was correct
    if (rowid == this->current_audio_deck_id)
    {
        this->delay_counter++;
        
        if (this->full_dataset.length() > this->counter)
        {
            if (this->delay_counter >= this->delay)
            {
                if (this->delay_counter == 1)
                {
                    this->delay = 5;
                }
                else if (this->delay_counter == 2)
                {
                    this->delay = 5;
                }
                else
                {
                    this->delay = 10;
                }
                
                this->delay_counter = 0;
                
                this->counter++;
                this->update();
                
                // moved to "update()":
                //playNextAudio();
            }
            else
            {
                playRandomAudio();
            }
        }
        else
        {
            playRandomAudio();
        }
    }
}

void QDirtyDozenWidget::playNextAudio()
{
    int selector = this->dataset.length() -1;
    playAudio(selector);
}

void QDirtyDozenWidget::playRandomAudio()
{
    int selector = qrand() % this->dataset.length();
    
    // we do not want the same audio played multiple times in a row
    if (this->counter > 1)
    {
        int depth = 0;
        while (selector == this->last_random_audio)
        {
            selector = qrand() % this->dataset.length();
            
            depth++;
            // to avoid infinite loops with just one element
            if (depth > 20)
            {
                break;
            }
        }
    }
    this->last_random_audio = selector;
    
    playAudio(selector);
}

void QDirtyDozenWidget::playAudio(int selector)
{
    QString audio_filename = this->dataset.at(selector)["filename"].toString();
    this->current_audio_deck_id = this->dataset.at(selector)["rowid"].toInt();
    
    QString audio_path = QDir::homePath() + "/.tambi/decks/" + this->deck_name + "/" + audio_filename;
    QUrl audio_url = QUrl::fromLocalFile(audio_path);
    this->audioPlayer->setMedia(QMediaContent(audio_url));
    this->audioPlayer->play();
}

void QDirtyDozenWidget::replayAudioClicked()
{
    audioPlayer->play();
}

void QDirtyDozenWidget::selectDisplayCurrentTextChanged(QString text)
{
    clear();
    this->learn_mode = text;
    update();
}

void QDirtyDozenWidget::onShuffleButtonClicked()
{
    clear();
    
    this->full_dataset = shuffleList(this->full_dataset);
    
    update();
}

QList<QMap<QString,QVariant>> QDirtyDozenWidget::shuffleList(QList<QMap<QString,QVariant>> list)
{
    for (int i = 0; i < 100; ++i)
    {
        int rnd = qrand() % list.length();
        QMap<QString,QVariant> item = list.at(rnd);
        
        list.removeAt(rnd);
        list.append(item);
    }
    
    return list;
}

void QDirtyDozenWidget::hideEvent(QHideEvent *event)
{
    audioPlayer->stop();
}
