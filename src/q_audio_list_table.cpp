#include "q_audio_list_table.h"

QAudioListTable::QAudioListTable(QString deck_name, qlonglong rowid, QTableWidget *parent)
    : QTableWidget(parent)
    , player (new QMediaPlayer)
{
    connect(this->player, &QMediaPlayer::stateChanged, this, &QAudioListTable::mediaPlayerStateChanged);
    
    this->deck_name = deck_name;
    this->database = new DbAdapter(deck_name);
    QList<QMap<QString,QVariant>> data = this->database->audioFilenamesForDeckRowID(rowid);
    
    setColumnCount(COLUMN_COUNT);
    setRowCount(data.length());
    horizontalHeader()->hide();
    
    for (int i = 0; i < rowCount(); ++i)
    {
        QString audio_filename = data.at(i)["filename"].toString();
                
        QTableWidgetItem *item_description = new QTableWidgetItem(data.at(i)["description"].toString());
        setItem(i, DESCRIPTION_COLUMN, item_description);
        
        QPushButton *audio_button = new QPushButton();
        audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
        setCellWidget(i, PLAY_BUTTON_COLUMN, audio_button);
        
        connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename]{ audioButtonClicked(audio_button, audio_filename); });
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        setCellWidget(i, DELETE_BUTTON_COLUMN, button_delete);
        
        connect(button_delete, &QPushButton::clicked, this, [this, i]{ deleteButtonClicked(i); });
        
        QPushButton *button_import = new QPushButton();
        button_import->setIcon(QIcon::fromTheme("document-open"));
        setCellWidget(i, IMPORT_FILE_BUTTON_COLUMN, button_import);
        
        connect(button_import, &QPushButton::clicked, this, [this, i]{ importButtonClicked(i); });
        button_import->setEnabled(false);
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme(":audacity_icon"));
        setCellWidget(i, EDIT_FILE_BUTTON, button_edit);
        
        connect(button_edit, &QPushButton::clicked, this, [this, i]{ editButtonClicked(i); });
        
        QTableWidgetItem *item_filename = new QTableWidgetItem(audio_filename);
        item_filename->setFlags(item_filename->flags() ^ Qt::ItemIsEditable);
        setItem(i, FILE_NAME_COLUMN, item_filename);
    }
    
    resizeColumnsToContents();
}

void QAudioListTable::audioButtonClicked(QPushButton *button, QString audio_filename)
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
        this->player->setMedia(QMediaContent(audio_url));
        this->player->play();
    }
}

void QAudioListTable::mediaPlayerStateChanged(int state)
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

void QAudioListTable::deleteButtonClicked(int row)
{
    qDebug() << "delete:" << row;
}

void QAudioListTable::importButtonClicked(int row)
{
    qDebug() << "import:" << row;
}

void QAudioListTable::editButtonClicked(int row)
{
    qDebug() << "edit:" << row;
}
