#include "q_audio_list_table.h"

QAudioListTable::QAudioListTable(QString deck_name, qlonglong deck_rowid, QTableWidget *parent)
    : QTableWidget(parent)
    , player (new QMediaPlayer)
    , recorder (new QAudioRecorder)
{
    connect(this->player, &QMediaPlayer::stateChanged, this, &QAudioListTable::mediaPlayerStateChanged);
    connect(this, &QAudioListTable::itemChanged, this, &QAudioListTable::onItemChanged);
    
    this->deck_name = deck_name;
    this->deck_rowid = deck_rowid;
    this->database = new DbAdapter(deck_name);
    
    QAudioEncoderSettings settings;
    settings.setCodec("audio/vorbis");
    this->recorder->setContainerFormat("ogg");
    this->recorder->setEncodingSettings(settings);
    
    drawAudioTable();
}
    
void QAudioListTable::drawAudioTable()
{
    this->ignore_item_changes = true;
    
    this->data = this->database->audioFilenamesForDeckRowID(this->deck_rowid);
    
    setColumnCount(COLUMN_COUNT);
    setRowCount(data.length());
    horizontalHeader()->hide();
    
    for (int i = 0; i < rowCount(); ++i)
    {
        QString audio_filename = data.at(i)["filename"].toString();
                
        QTableWidgetItem *item_description = new QTableWidgetItem(data.at(i)["description"].toString());
        setItem(i, DESCRIPTION_COLUMN, item_description);
        
        QPushButton *audio_button = new QPushButton();
        setCellWidget(i, PLAY_BUTTON_COLUMN, audio_button);
        
        if (data.at(i)["filename"].toString() == "")
        {
            audio_button->setIcon(QIcon::fromTheme("media-record"));
            connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename]{ recordButtonClicked(audio_button, audio_filename); });
        }
        else
        {
            audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
            connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename]{ audioButtonClicked(audio_button, audio_filename); });
        }
        this->playing_button = audio_button;
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        setCellWidget(i, DELETE_BUTTON_COLUMN, button_delete);
        
        connect(button_delete, &QPushButton::clicked, this, [this, i]{ deleteButtonClicked(i); });
        
        QPushButton *button_import = new QPushButton();
        button_import->setIcon(QIcon::fromTheme("document-open"));
        setCellWidget(i, IMPORT_FILE_BUTTON_COLUMN, button_import);
        
        connect(button_import, &QPushButton::clicked, this, [this, i]{ importButtonClicked(i); });
        if (data.at(i)["filename"].toString() != "")
        {
            button_import->setEnabled(false);
        }
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme(":audacity_icon"));
        setCellWidget(i, EDIT_FILE_BUTTON, button_edit);
        if (data.at(i)["filename"].toString() == "")
        {
            button_edit->setEnabled(false);
        }
        
        connect(button_edit, &QPushButton::clicked, this, [this, i]{ editButtonClicked(i); });
        
        QTableWidgetItem *item_filename = new QTableWidgetItem(audio_filename);
        item_filename->setFlags(item_filename->flags() ^ Qt::ItemIsEditable);
        setItem(i, FILE_NAME_COLUMN, item_filename);
    }
    
    resizeColumnsToContents();
    
    this->ignore_item_changes = false;
}

void QAudioListTable::audioButtonClicked(QPushButton *button, QString audio_filename)
{
    this->ignore_item_changes = true;
    
    if (this->player->state() == QMediaPlayer::PlayingState && this->playing_button == button)
    {
        this->player->stop();
    }
    else
    {
        if (this->playing_button != nullptr)
        {
            qDebug() << "button:" << this->playing_button;
            this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        
        this->playing_button = button;
        
        QString audio_path = QDir::homePath() + "/.tambi/decks/" + this->deck_name + "/" + audio_filename;
        QUrl audio_url = QUrl::fromLocalFile(audio_path);
        this->player->setMedia(QMediaContent(audio_url));
        this->player->play();
    }
    
    this->ignore_item_changes = false;
}

void QAudioListTable::recordButtonClicked(QPushButton *button, QString audio_filename)
{
    qDebug() << "recording";
    button->setIcon(QIcon::fromTheme("media-playback-stop"));
    
    QUrl record_url = QUrl::fromLocalFile(QDir::homePath() + "/.tambi/decks/" + this->deck_name + "/" + "aaaa.ogg");
    this->recorder->setOutputLocation(record_url);
    this->recorder->record();
}

void QAudioListTable::mediaPlayerStateChanged(int state)
{
    this->ignore_item_changes = true;
    
    if (state == QMediaPlayer::StoppedState)
    {
        this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
    }
    else if (state == QMediaPlayer::PlayingState)
    {
        this->playing_button->setIcon(QIcon::fromTheme("media-playback-stop"));
    }
    
    this->ignore_item_changes = false;
}

void QAudioListTable::deleteButtonClicked(int row)
{
    int reply = QMessageBox::question(this, "Delete", "really?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString filename = this->item(row, FILE_NAME_COLUMN)->text();
        if (filename != "")
        {
            QString filepath = QDir::homePath() + "/.tambi/decks/" + this->deck_name + "/" + filename;
            QFile file(filepath);
            file.remove();
        }
        
        this->database->deleteAudio(this->data.at(row)["rowid"].toLongLong());
        this->clear();
        drawAudioTable();
    }
}

void QAudioListTable::importButtonClicked(int row)
{
    qDebug() << "import:" << row;
}

void QAudioListTable::editButtonClicked(int row)
{
    qDebug() << "edit:" << row;
}

void QAudioListTable::newAudioLine()
{
    insertRow(rowCount());
    this->database->newAudioRow(this->deck_rowid);
    clear();
    drawAudioTable();
}

void QAudioListTable::onItemChanged()
{
    if (! this->ignore_item_changes)
    {
        qDebug() << "item changed";
        
        for (int i = 0; i < this->rowCount(); ++i)
        {
            QString description = item(i, DESCRIPTION_COLUMN)->text();
            QString filename = item(i, FILE_NAME_COLUMN)->text();
            qlonglong audio_rowid = this->data.at(i)["rowid"].toLongLong();
            
            this->database->insertAudioFilename(this->deck_rowid, audio_rowid, filename, description);
        }
    }
}
