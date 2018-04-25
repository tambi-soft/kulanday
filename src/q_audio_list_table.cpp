#include "q_audio_list_table.h"

QAudioListTable::QAudioListTable(QDir *decks_path, QString deck_name, qlonglong deck_rowid, QTableWidget *parent)
    : QTableWidget(parent)
    , player (new QMediaPlayer)
    //, recorder (new QAudioRecorder)
{
    connect(this->player, &QMediaPlayer::stateChanged, this, &QAudioListTable::mediaPlayerStateChanged);
    connect(this, &QAudioListTable::itemChanged, this, &QAudioListTable::onItemChanged);
    
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    this->deck_rowid = deck_rowid;
    this->database = new DbAdapter(this->decks_path, deck_name);
    
    QAudioEncoderSettings settings;
    settings.setCodec("audio/vorbis");
    //this->recorder->setContainerFormat("ogg");
    //this->recorder->setEncodingSettings(settings);
    
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
        this->audio_rowid[i] = data.at(i)["rowid"].toInt();;
        QString audio_filename = data.at(i)["filename"].toString();
                
        QTableWidgetItem *item_description = new QTableWidgetItem(data.at(i)["description"].toString());
        setItem(i, DESCRIPTION_COLUMN, item_description);
        
        QPushButton *audio_button = new QPushButton();
        setCellWidget(i, PLAY_BUTTON_COLUMN, audio_button);
        
        if (data.at(i)["filename"].toString() == "")
        {
            audio_button->setIcon(QIcon::fromTheme("media-record"));
            connect(audio_button, &QPushButton::clicked, this, [this, i, audio_button, audio_filename]{ recordButtonClicked(i, audio_button, audio_filename); });
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
    
    if (this->player->state() == QMediaPlayer::PlayingState)
    {
        this->player->stop();
        this->playing_button = nullptr;
    }
    else
    {
        /*
        if (this->playing_button != nullptr)
        {
            qDebug() << "button1:" << this->playing_button;
            qDebug() << "button2:" << button;
            //this->playing_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        */
        
        this->playing_button = button;
        
        QString audio_path = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + audio_filename;
        QUrl audio_url = QUrl::fromLocalFile(audio_path);
        this->player->setMedia(QMediaContent(audio_url));
        this->player->play();
    }
    
    this->ignore_item_changes = false;
}

void QAudioListTable::recordButtonClicked(int row, QPushButton *button, QString audio_filename)
{
    if (this->recording_row == row)
    {
        qDebug() << "stop recording";
        this->recording_row = -1;
        
        this->arec->recStop();
        
        clear();
        drawAudioTable();
    }
    else
    {
        qDebug() << "recording";
        this->recording_row = row;
        
        button->setIcon(QIcon::fromTheme("media-playback-stop"));
        
        QDateTime *date = new QDateTime();
        uint filename_stmp_int = date->currentDateTime().toTime_t();
        QString filename_stmp = QString::number((int) filename_stmp_int);
        QString filename_rnd = randomString(5);
        QString filename = filename_stmp + "_" + filename_rnd;
        QUrl record_url = QUrl::fromLocalFile(this->decks_path->absolutePath() + "/" + this->deck_name + "/" + filename + ".ogg");
        
        //this->recorder->setOutputLocation(record_url);
        //this->recorder->record();
        
        QTableWidgetItem *item = new QTableWidgetItem(record_url.fileName());
        setItem(row, FILE_NAME_COLUMN, item);
        
        this->arec = new AudioRecorder(record_url.path());
        this->arec->recStart();
        
    }   
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
            QString filepath = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + filename;
            QFile file(filepath);
            file.remove();
        }
        
        this->database->deleteAudio(this->data.at(row)["rowid"].toLongLong());
        this->clear();
        drawAudioTable();
        emit changed();
    }
}

void QAudioListTable::importButtonClicked(int row)
{
    QString audio_url = QFileDialog::getOpenFileName(this, "Import Audio", QDir::homePath());
    if (audio_url != NULL)
    {
        QFile filepath(audio_url);
        QString filename = QUrl(audio_url).fileName();
        
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
        
        qDebug() << "filename:" << filename;
        
        this->database->insertAudioFilename(this->deck_rowid, this->audio_rowid[row], filename, itemAt(row, DESCRIPTION_COLUMN)->text());
        
        clear();
        drawAudioTable();
        emit changed();
    }
}

void QAudioListTable::editButtonClicked(int row)
{
    qDebug() << "edit:" << row;
    
    QString filename = item(row, FILE_NAME_COLUMN)->text();
    QString filepath = this->decks_path->absolutePath() + "/" + this->deck_name + "/" + filename;
    
    QProcess *pro = new QProcess();
    pro->startDetached("audacity " + filepath);
}

void QAudioListTable::newAudioLine()
{
    insertRow(rowCount());
    int db_rowid = this->database->newAudioRow(this->deck_rowid);
    this->audio_rowid[rowCount(), db_rowid];
    
    clear();
    drawAudioTable();
    emit changed();
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
    
    resizeColumnsToContents();
    emit changed();
}

QString QAudioListTable::randomString(int length)
{
    // https://stackoverflow.com/questions/18862963/qt-c-random-string-generation?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
    
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    
       QString randomString;
       for(int i=0; i < length; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
}

void QAudioListTable::stopAudio()
{
    this->player->stop();
}
