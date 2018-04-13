
#include "q_deck_widget.h"

QDeckOverviewWidget::QDeckOverviewWidget(QString deck_name, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
    , player (new QMediaPlayer)
{
    setLayout(layout);
    this->deck_name = deck_name;
    
    QPushButton *new_item_button = new QPushButton("new item");
    connect (new_item_button, &QPushButton::clicked, this, &QDeckOverviewWidget::newItemButtonClicked);
    
    connect (player, &QMediaPlayer::stateChanged, this, &QDeckOverviewWidget::mediaPlayerStateChanged);
    
    layout->addWidget(table);
    layout->addWidget(new_item_button);
    
    table->horizontalHeader()->hide();
    initTableWidget(deck_name);
}



void QDeckOverviewWidget::initTableWidget(QString deck_name)
{
    DbAdapter *db_adapter = new DbAdapter(deck_name);
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckItems();
    
    if (data.length() > 0)
    {
        //table->setColumnCount(data.at(0).count());
        table->setRowCount(data.length());
        
        int max_audio_count = db_adapter->getMaxAudioCount();
        table->setColumnCount(COLUMN_OFFSET + max_audio_count);
        
        for (int i = 0; i < data.length(); ++i)
        {
            int rowid = data.at(i)["rowid"].toInt(); // needed for SELECTing audio files
            
            QPushButton *edit_button = new QPushButton();
            edit_button->setIcon(QIcon::fromTheme("document-properties"));
            connect(edit_button, &QPushButton::clicked, this, [this, deck_name, rowid]{ editRowButtonClicked(deck_name, rowid); });
            
            QPushButton *delete_button = new QPushButton();
            delete_button->setIcon(QIcon::fromTheme("edit-delete"));
            connect(delete_button, &QPushButton::clicked, this, [this, rowid]{ deleteRowButtonClicked(rowid); });
            
            QString order_index = data.at(i)["order_index"].toString();
            QString name = data.at(i)["name"].toString();
            QString word = data.at(i)["word"].toString();
            QString phonetical = data.at(i)["phonetical"].toString();
            QString translation = data.at(i)["translation"].toString();
            
            QString image_filename = data.at(i)["image"].toString();
            
            QLabel *image_widget = new QLabel(this);
            if (image_filename != "")
            {
                QPixmap pixmap(QDir::homePath() + "/.tambi/decks/" + deck_name + "/" + image_filename);
                pixmap = pixmap.scaled(QSize(60, 30), Qt::KeepAspectRatio);
                image_widget->setPixmap(pixmap);
            }
            
            table->setCellWidget(i, 0, edit_button);
            table->setCellWidget(i, 1, delete_button);
            
            table->setItem(i, 2, new QTableWidgetItem(order_index));
            table->setItem(i, 3, new QTableWidgetItem(name));
            table->setItem(i, 4, new QTableWidgetItem(word));
            table->setItem(i, 5, new QTableWidgetItem(phonetical));
            table->setItem(i, 6, new QTableWidgetItem(translation));
            
            table->setCellWidget(i, 8, image_widget);
            
            QList<QMap<QString,QVariant>> audio_filenames = db_adapter->audioFilenamesForDeckRowID(rowid);
            appendPlayButtons(i, audio_filenames, max_audio_count);
        }
    }
    
    table->resizeColumnsToContents();
}

void QDeckOverviewWidget::appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames, int max_audio_count)
{
    for (int column = 0; column < audio_filenames.length(); ++column)
    {
        QPushButton *audio_button = new QPushButton();
        audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
        
        QString audio_filename = audio_filenames.at(column)["filename"].toString();
        connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename]{ audioButtonClicked(audio_button, audio_filename); });
        
        table->setCellWidget(table_rowid, column + COLUMN_OFFSET, audio_button);
    }
}
void QDeckOverviewWidget::audioButtonClicked(QPushButton *button, QString audio_filename)
{
    if (this->player->state() == QMediaPlayer::PlayingState && this->playing_button == button)
    {
        this->player->stop();
    }
    else
    {
        if (this->playing_button != nullptr)
        {
            this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        
        this->playing_button = button;
        
        QString audio_path = "file://" + QDir::homePath() + "/.tambi/decks/" + this->deck_name + "/" + audio_filename;
        QUrl audio_url = QUrl(audio_path);
        player->setMedia(QMediaContent(audio_url));
        player->play();
    }
}
void QDeckOverviewWidget::mediaPlayerStateChanged(int state)
{
    if (state == QMediaPlayer::StoppedState)
    {
        this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
    }
    else if (state == QMediaPlayer::PlayingState)
    {
        this->playing_button->setIcon(QIcon::fromTheme("media-playback-stop"));
    }
}

void QDeckOverviewWidget::newItemButtonClicked()
{
    emit newDeckItemRequested(deck_name);
}

void QDeckOverviewWidget::editRowButtonClicked(QString deck_name, int rowid)
{
    emit showDeckItemRequested(deck_name, rowid);
}

void QDeckOverviewWidget::deleteRowButtonClicked(int rowid)
{
    
}
