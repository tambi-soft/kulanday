
#include "deck_widget.h"

QDeckOverviewWidget::QDeckOverviewWidget(QString filter, QDir *decks_path, QWidget *parent)
    : QWidget(parent)
{
    this->searchMode = true;
    
    initGui();
    
    this->decks_path = decks_path;
    
    QList<QMap<QString,QVariant>> result;
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    foreach (QString deck_name, decks_names) {
        
        this->db_adapter = new DbAdapter(decks_path, deck_name);
        QList<QMap<QString,QVariant>> data = this->db_adapter->selectDeckItemsFiltered(filter);
        
        for (int i = 0; i < data.length(); ++i)
        {
            QMap<QString,QVariant> set = data.at(i);
            result.append(set);
        }
        
    }
    
    populateTableWidget(result);
}

QDeckOverviewWidget::QDeckOverviewWidget(QDir *decks_path, QString deck_name, QWidget *parent)
    : QWidget(parent)
{
    this->searchMode = false;
    
    initGui();
    
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::initGui()
{
    layout = new QGridLayout();
    table = new QTableWidget();
    player = new QMediaPlayer();
    unicodeFonts = new UnicodeFonts();
    chk_name = new QCheckBox();
    chk_word = new QCheckBox();
    chk_phonetical = new QCheckBox();
    chk_translation = new QCheckBox();
    chk_svg = new QCheckBox();
    chk_image = new QCheckBox();
            
    setLayout(layout);
    
    //setFocusPolicy(Qt::StrongFocus);
    
    QPushButton *refresh_button = new QPushButton("refresh");
    connect(refresh_button, &QPushButton::clicked, this, &QDeckOverviewWidget::refreshTable);
    
    QPushButton *new_item_button = new QPushButton("new item");
    connect (new_item_button, &QPushButton::clicked, this, &QDeckOverviewWidget::newItemButtonClicked);
    
    connect (player, &QMediaPlayer::stateChanged, this, &QDeckOverviewWidget::mediaPlayerStateChanged);
    
    QWidget *chk_widget = new QWidget();
    QHBoxLayout *chk_layout = new QHBoxLayout();
    chk_widget->setLayout(chk_layout);
    chk_layout->addWidget(new QLabel("name:"), 0, Qt::AlignRight);
    chk_layout->addWidget(chk_name, 0, Qt::AlignLeft);
    chk_layout->addWidget(new QLabel("word:"), 1, Qt::AlignRight);
    chk_layout->addWidget(chk_word, 0, Qt::AlignLeft);
    chk_layout->addWidget(new QLabel("phonetical:"), 1, Qt::AlignRight);
    chk_layout->addWidget(chk_phonetical, 0, Qt::AlignLeft);
    chk_layout->addWidget(new QLabel("translation:"), 1, Qt::AlignRight);
    chk_layout->addWidget(chk_translation, 0, Qt::AlignLeft);
    chk_layout->addWidget(new QLabel("svg:"), 1, Qt::AlignRight);
    chk_layout->addWidget(chk_svg, 0, Qt::AlignLeft);
    chk_layout->addWidget(new QLabel("image:"), 1, Qt::AlignRight);
    chk_layout->addWidget(chk_image, 0, Qt::AlignLeft);
    
    chk_name->setChecked(true);
    chk_word->setChecked(true);
    chk_phonetical->setChecked(true);
    chk_translation->setChecked(true);
    chk_svg->setChecked(true);
    chk_image->setChecked(true);
    
    connect(chk_name, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    connect(chk_word, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    connect(chk_phonetical, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    connect(chk_translation, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    connect(chk_svg, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    connect(chk_image, &QCheckBox::clicked, this, &QDeckOverviewWidget::refresh);
    
    if (! this->searchMode)
    {
        layout->addWidget(chk_widget, 0, 0, 1, 2);
    }
    layout->addWidget(table, 1, 0, 1, 2);
    if (! this->searchMode)
    {
        layout->addWidget(refresh_button, 2, 0);
        layout->addWidget(new_item_button, 2, 1);
    }
}

QList<QMap<QString,QVariant>> QDeckOverviewWidget::fetchDeckData()
{
    this->db_adapter = new DbAdapter(this->decks_path, this->deck_name);
    this->database = db_adapter;
    
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckItems();
    
    return data;
}

void QDeckOverviewWidget::populateTableWidget(QList<QMap<QString,QVariant>> data)
{
    // to avoid pointing to a deleted object after reinitializing
    this->playing_button = nullptr;
    
    if (data.length() > 0)
    {
        table->setRowCount(data.length());
        
        table->setColumnCount(COLUMN_OFFSET + 1000);
        
        // insert data
        for (int i = 0; i < data.length(); ++i)
        {
            int rowid = data.at(i)["rowid"].toInt();
            
            QString deck_name = data.at(i)["[deck_name]"].toString();
            
            QPushButton *edit_button = new QPushButton();
            edit_button->setIcon(QIcon::fromTheme("document-properties"));
            connect(edit_button, &QPushButton::clicked, this, [this, deck_name, rowid]{ editRowButtonClicked(deck_name, rowid); });
            
            // move item to another deck
            QPushButton *move_button = new QPushButton();
            move_button->setIcon(QIcon::fromTheme("document-send"));
            connect(move_button, &QPushButton::clicked, this, [this, deck_name, rowid]{ moveItem(deck_name, rowid); });
            
            QPushButton *delete_button = new QPushButton();
            delete_button->setIcon(QIcon::fromTheme("edit-delete"));
            connect(delete_button, &QPushButton::clicked, this, [this, rowid, data, i, deck_name]{ deleteRowButtonClicked(rowid, data.at(i), deck_name); });
            
            QString order_index = data.at(i)["order_index"].toString();
            QString name = data.at(i)["name"].toString();
            QString word = data.at(i)["word"].toString();
            QString phonetical = data.at(i)["phonetical"].toString();
            QString translation = data.at(i)["translation"].toString();
            
            name = cropText(name);
            word = cropText(word);
            phonetical = cropText(phonetical);
            translation = cropText(translation);
            
            QString image_filename = data.at(i)["image"].toString();
            QString svg_filename = data.at(i)["svg_filename"].toString();
            
            QLabel *image_widget = new QLabel(this);
            if (image_filename != "" && chk_image->isChecked())
            {
                QPixmap pixmap(this->decks_path->absolutePath() + "/" + deck_name + "/" + image_filename);
                pixmap = pixmap.scaled(QSize(60, 30), Qt::KeepAspectRatio);
                image_widget->setPixmap(pixmap);
            }
            
            QSvgWidget *svg_widget = new QSvgWidget();
            svg_widget->setFixedSize(0, 0);
            if (svg_filename != "" && chk_svg->isChecked())
            {
                svg_widget->load(this->decks_path->absolutePath() + "/" + deck_name + "/" + svg_filename);
                svg_widget->setFixedSize(60, 30);
            }
            
            table->setItem(i, 0, new QTableWidgetItem(deck_name));
            table->setCellWidget(i, 1, edit_button);
            table->setCellWidget(i, 2, move_button);
            table->setCellWidget(i, 3, delete_button);
            
            table->setItem(i, 4, new QTableWidgetItem(order_index));
            
            if (chk_name->isChecked())
            {
                table->setItem(i, 5, new QTableWidgetItem(name));
            }
            if (chk_word->isChecked())
            {
                table->setItem(i, 6, new QTableWidgetItem(word));
            }
            if (chk_phonetical->isChecked())
            {
                table->setItem(i, 7, new QTableWidgetItem(phonetical));
            }
            if (chk_translation->isChecked())
            {
                table->setItem(i, 8, new QTableWidgetItem(translation));
            }
            
            table->setCellWidget(i, 9, svg_widget);
            table->setCellWidget(i, 10, image_widget);
            
            appendPlayButtons(i, data, deck_name);
        }
    }
    
    //table->horizontalHeader()->hide();
    QStringList labels;
    labels << "deck" << "" << "" << "" << "" << "name" << "word" << "phon." << "trans." << "svg" << "image" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    table->setHorizontalHeaderLabels(labels);
    
    table->resizeColumnsToContents();
    //table->resizeRowsToContents();
    
    table->setColumnCount(COLUMN_OFFSET + this->max_audio_count);
    
    if (this->searchMode)
    {
        table->setColumnHidden(1, true);
        table->setColumnHidden(2, true);
        table->setColumnHidden(3, true);
        table->setColumnHidden(4, true);
    }
    else
    {
        table->setColumnHidden(0, true);
    }
}

void QDeckOverviewWidget::appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> data, QString deck_name)
{
    QStringList audio_files = data.at(table_rowid)["audio"].toString().split(",");
    this->max_audio_count = std::max(audio_files.length(), this->max_audio_count);
    
    for (int column = 0; column < audio_files.length(); ++column)
    {
        QString audio_filename = audio_files.at(column);
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
        
        connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename, deck_name]{ audioButtonClicked(audio_button, audio_filename, deck_name); });
        
        table->setCellWidget(table_rowid, column + COLUMN_OFFSET, audio_button);
    }
}
void QDeckOverviewWidget::audioButtonClicked(QPushButton *button, QString audio_filename, QString deck_name)
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
        
        QString audio_path = this->decks_path->absolutePath() + "/" + deck_name + "/" + audio_filename;
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
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::refreshTable()
{
    table->clear();
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::editRowButtonClicked(QString deck_name, int rowid)
{
    emit showDeckItemRequested(deck_name, rowid);
}

void QDeckOverviewWidget::deleteRowButtonClicked(int rowid, QMap<QString,QVariant> data_row, QString deck_name)
{
    QString message = "delete " + data_row["name"].toString() + "|" + data_row["word"].toString() + "|" + data_row["phonetical"].toString() + "|" + data_row["translation"].toString() + "?";
    int reply = QMessageBox::question(this, "Delete", message, QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        QList<QMap<QString,QVariant>> data_to_delete = this->database->deleteItem(rowid);
        
        for (int i = 0; i < data_to_delete.length(); ++i)
        {
            QString filename = data_to_delete.at(i)["filename"].toString();
            if (filename != "")
            {
                QString filepath = this->decks_path->absolutePath() + "/" + deck_name + "/" + filename;
                QFile file(filepath);
                file.remove();
            }
        }
    }
    
    table->clear();
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::moveItem(QString deck_name, qlonglong rowid)
{
    
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
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

QString QDeckOverviewWidget::cropText(QString text)
{
    QString result;
    
    if (text.length() > 30)
    {
        result = text.left(15) + "[...]" + text.right(10);
    }
    else
    {
        result = text;
    }
    
    return result;
}
