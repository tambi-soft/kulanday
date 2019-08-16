#include "dirtydozen_widget.h"

QDirtyDozenWidget::QDirtyDozenWidget(QDir *decks_path, QString deck_name, Config *config, QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
//    , slider_repeat (new QSlider)
    , unicodeFonts (new UnicodeFonts)
    , audioPlayer (new QMediaPlayer)
{
    DISPLAY_COMBO_ITEMS << "image" << "name" << "word" << "translation";
    
    this->config = config;
    QSize dd_size = this->config->getDirtyDozenSize();
    this->ROWS = dd_size.height();
    this->COLUMNS = dd_size.width();
    
    this->decks_path = decks_path;
    
    setLayout(grid);
    
    this->deck_name = deck_name;
    
    initialize(deck_name);
    // to avoid crash with empty index
    if (this->full_dataset.length() > 0)
    {
        update();
        playNextAudio();
    }
}

void QDirtyDozenWidget::initialize(QString deck_name)
{
    DbAdapter *db_adapter = new DbAdapter(this->decks_path, deck_name);
    this->full_dataset = db_adapter->selectDeckDirtyDozenItems(this->COLUMNS * this->ROWS);
    
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
    this->grid->addWidget(show_all_button, int(this->COLUMNS * this->ROWS / this->COLUMNS +2), 0);
    
    QGroupBox *slider_group = new QGroupBox("repeats counter");
    QHBoxLayout *slider_layout = new QHBoxLayout();
    slider_layout->setContentsMargins(0, 0, 0, 0);
    this->slider_repeat_times = new QSlider(Qt::Horizontal);
    slider_layout->addWidget(this->slider_repeat_times);
    slider_group->setLayout(slider_layout);
    //this->grid->addWidget(this->slider_repeat_times, int(12 / this->COLUMNS +2), 1, 1, 2);
    this->grid->addWidget(slider_group, int(this->COLUMNS * this->ROWS / this->COLUMNS+2), 1, 1, 2);
    this->slider_repeat_times->setMinimum(1);
    this->slider_repeat_times->setMaximum(21);
    this->slider_repeat_times->setValue(11);
    this->slider_repeat_times->setTickInterval(5);
    this->slider_repeat_times->setTickPosition(QSlider::TicksBothSides);
    connect(this->slider_repeat_times, &QSlider::valueChanged, this, &QDirtyDozenWidget::sliderRepeatTimesChanged);
    
    QPushButton *shuffle_button = new QPushButton("shuffle");
    shuffle_button->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
    connect(shuffle_button, &QPushButton::clicked, this, &QDirtyDozenWidget::onShuffleButtonClicked);
    this->grid->addWidget(shuffle_button, int(this->COLUMNS * this->ROWS / this->COLUMNS +2), this->COLUMNS-1);
}

void QDirtyDozenWidget::sliderRepeatTimesChanged(int value)
{
    this->delay = value;
}

void QDirtyDozenWidget::update()
{
    clear();
    
    // turn button to "start over" when all items are shown
    if (this->full_dataset.length() <= this->counter)
    {
        this->show_all_button->setText("start over");
    }
    
    this->dataset = this->full_dataset.mid(0, this->counter);
    // we want to play the new item before shuffle
    // (after it is hard to know wich one is the new)
    playNextAudio();
    QMap<QString,QVariant> new_item = this->dataset.last();
    this->dataset = shuffleList(this->dataset);
    
    for (int i = 0; i < this->dataset.length(); ++i)
    {
        QPushButton *button = new QPushButton();
        button->setMinimumHeight(190);
        
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
        
        if (dataset.at(i) == new_item)
        {
            button->setStyleSheet("QPushButton { background-color : #00a194; }");
        }
        
        QFont font = unicodeFonts->getFontAndSize(button->text());
        button->setFont(font);
        
        connect(button, &QPushButton::clicked, this, [this, i, button]{ displayButtonClicked(dataset.at(i)["rowid"].toInt(), button); });
        
        int row = i + this->COLUMNS;
        this->grid->addWidget(button, int(row / COLUMNS), row % COLUMNS);
        
        this->button_list.append(button);
    }
}

void QDirtyDozenWidget::clear()
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

void QDirtyDozenWidget::clearStyleSheet()
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        QWidget *widget = layout()->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        //if (class_name == "QClickLabel")
        if (class_name == "QPushButton")
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

void QDirtyDozenWidget::displayButtonClicked(int rowid, QPushButton *button)
{
    clearStyleSheet();
    
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
                    this->delay = 8;
                    //this->delay = this->slider_repeat_times->value();
                }
                else
                {
                    //this->delay = 10;
                    this->delay = this->slider_repeat_times->value();
                }
                
                this->delay_counter = 0;
                
                this->counter++;
                this->update();
            }
            else
            {
                playRandomAudio();
            }
        }
        else
        {
            this->counter = 1;
            
            playRandomAudio();
        }
    }
    // if wrong image was clicked
    else
    {
        button->setStyleSheet("QPushButton { background-color : #ff0000; }");
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
    
    QString audio_path = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + audio_filename;
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
