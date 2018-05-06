
#include "q_deck_widget.h"

QDeckOverviewWidget::QDeckOverviewWidget(QDir *decks_path, QString deck_name, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
    , player (new QMediaPlayer)
    , unicodeFonts (new UnicodeFonts)
{
    setLayout(layout);
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    
    //setFocusPolicy(Qt::StrongFocus);
    
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
    this->playing_button = nullptr;
    
    DbAdapter *db_adapter = new DbAdapter(this->decks_path, deck_name);
    this->database = db_adapter;
    
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckItems();
    
    if (data.length() > 0)
    {
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
            QString svg_filename = data.at(i)["svg_filename"].toString();
            
            QLabel *image_widget = new QLabel(this);
            if (image_filename != "")
            {
                QPixmap pixmap(this->decks_path->absolutePath() + "/" + deck_name + "/" + image_filename);
                pixmap = pixmap.scaled(QSize(60, 30), Qt::KeepAspectRatio);
                image_widget->setPixmap(pixmap);
            }
            
            QSvgWidget *svg_widget = new QSvgWidget();
            svg_widget->setFixedSize(0, 0);
            if (svg_filename != "")
            {
                svg_widget->load(this->decks_path->absolutePath() + "/" + deck_name + "/" + svg_filename);
                svg_widget->setFixedSize(60, 30);
            }
            
            table->setCellWidget(i, 0, edit_button);
            table->setCellWidget(i, 1, delete_button);
            
            table->setItem(i, 2, new QTableWidgetItem(order_index));
            table->setItem(i, 3, new QTableWidgetItem(name));
            table->setItem(i, 4, new QTableWidgetItem(word));
            table->setItem(i, 5, new QTableWidgetItem(phonetical));
            table->setItem(i, 6, new QTableWidgetItem(translation));
            
            // apply unicode fonst to cells
            for (int j = 3; j <= 6; ++j)
            {
                QFont font = unicodeFonts->getFontAndSize(table->item(i, j)->text());
                if (font.pointSize() > 20)
                {
                    font.setPointSize(20);
                }
                QTableWidgetItem *item = table->item(i, j);
                item->setFont(font);
            }
            
            table->setCellWidget(i, 7, svg_widget);
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
        QString audio_filename = audio_filenames.at(column)["filename"].toString();
        QPushButton *audio_button = new QPushButton();
        if (audio_filename != "")
        {
            audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        else
        {
            audio_button->setIcon(QIcon::fromTheme("media-record"));
            audio_button->setEnabled(false);
        }
        connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename]{ audioButtonClicked(audio_button, audio_filename); });
        
        table->setCellWidget(table_rowid, column + COLUMN_OFFSET, audio_button);
    }
}
void QDeckOverviewWidget::audioButtonClicked(QPushButton *button, QString audio_filename)
{
    qDebug() << this->playing_button;
    if (this->player->state() == QMediaPlayer::PlayingState && this->playing_button == button)
    {
        this->player->stop();
    }
    else
    {
        if (this->playing_button != nullptr)
        {
            qDebug() << this->playing_button;
            this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        
        this->playing_button = button;
        
        QString audio_path = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + audio_filename;
        QUrl audio_url = QUrl::fromLocalFile(audio_path);
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
    
    table->clear();
    initTableWidget(this->deck_name);
}

void QDeckOverviewWidget::editRowButtonClicked(QString deck_name, int rowid)
{
    emit showDeckItemRequested(deck_name, rowid);
}

void QDeckOverviewWidget::deleteRowButtonClicked(int rowid)
{
    int reply = QMessageBox::question(this, "Delete", "really?", QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        QList<QMap<QString,QVariant>> data_to_delete = this->database->deleteItem(rowid);
        
        for (int i = 0; i < data_to_delete.length(); ++i)
        {
            QString filename = data_to_delete.at(i)["filename"].toString();
            if (filename != "")
            {
                QString filepath = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + filename;
                QFile file(filepath);
                file.remove();
            }
        }
    }
    
    table->clear();
    initTableWidget(this->deck_name);
}

void QDeckOverviewWidget::hideEvent(QHideEvent *event)
{
    this->player->stop();
}

void QDeckOverviewWidget::showEvent(QShowEvent *event)
{
    
}

void QDeckOverviewWidget::refresh()
{
    table->clear();
    initTableWidget(this->deck_name);
}
