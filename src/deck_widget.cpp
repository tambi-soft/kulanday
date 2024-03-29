
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

QDeckOverviewWidget::QDeckOverviewWidget(QDir *decks_path, QString deck_name, Config *config, QWidget *parent)
    : QWidget(parent)
{
    this->searchMode = false;
    
    this->config = config;

    initGui();
    
    this->decks_path = decks_path;
    this->deck_name = deck_name;
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::initGui()
{
    connect(this->scroll_area->verticalScrollBar(), &QScrollBar::rangeChanged, this, &QDeckOverviewWidget::scrollBarRangeChanged);
    layout = new QGridLayout();
    this->scroll_area->setWidgetResizable(true);
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
    //chk_layout->addWidget(new QLabel("svg:"), 1, Qt::AlignRight);
    //chk_layout->addWidget(chk_svg, 0, Qt::AlignLeft);
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
    layout->addWidget(this->scroll_area, 1, 0, 1, 2);
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
    this->grid = new QGridLayout;
    this->scroll_widget = new QWidget;
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);
    
    
    // to avoid pointing to a deleted object after reinitializing
    this->playing_button = nullptr;
    
    if (data.length() > 0)
    {
        for (int i = 0; i < data.length(); ++i)
        {
            int rowid = data.at(i)["rowid"].toInt();
            
            QString deck_name = data.at(i)["[deck_name]"].toString();
            
            QPushButton *edit_button = new QPushButton();
            edit_button->setIcon(QIcon::fromTheme("document-properties"));
            connect(edit_button, &QPushButton::clicked, this, [this, deck_name, rowid]{ editRowButtonClicked(deck_name, rowid); });
            edit_button->setToolTip("edit this item");
            edit_button->setMaximumWidth(BUTTON_WIDTH);
            
            // move item to another deck
            QPushButton *move_button = new QPushButton();
            move_button->setIcon(QIcon::fromTheme("document-send"));
            connect(move_button, &QPushButton::clicked, this, [this, deck_name, rowid]{ moveItem(deck_name, rowid); });
            move_button->setToolTip("move this item to another deck");
            move_button->setMaximumWidth(BUTTON_WIDTH);
            
            QPushButton *delete_button = new QPushButton();
            delete_button->setIcon(QIcon::fromTheme("edit-delete"));
            connect(delete_button, &QPushButton::clicked, this, [this, rowid, data, i, deck_name]{ deleteRowButtonClicked(rowid, data.at(i), deck_name); });
            delete_button->setToolTip("delete this item");
            delete_button->setMaximumWidth(BUTTON_WIDTH);
            
            QString order_index = data.at(i)["order_index"].toString();
            QString name = data.at(i)["name"].toString();
            QString word = data.at(i)["word"].toString();
            QString phonetical = data.at(i)["phonetical"].toString();
            QString translation = data.at(i)["translation"].toString();
            
            //name = cropText(name);
            //word = cropText(word);
            //phonetical = cropText(phonetical);
            //translation = cropText(translation);
            
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
            
            this->grid->addWidget(new QLabel(QString::number(i+1)), i, 0);
            if (! this->searchMode)
            {
                this->grid->addWidget(edit_button, i, 1);
                this->grid->addWidget(move_button, i, 2);
                this->grid->addWidget(delete_button, i, 3);
            }
            else
            {
                this->grid->addWidget(new QLabel(this->deck_name), i, 1);
            }
            
            //table->setItem(i, 4, new QTableWidgetItem(order_index));
            
            if (chk_name->isChecked())
            {
                QLabel *label_name = new QLabel(name);
                label_name->setWordWrap(true);
                this->grid->addWidget(label_name, i, 5);
            }
            
            if (chk_word->isChecked())
            {
                QLabel *label_word = new QLabel(word);
                label_word->setWordWrap(true);
                this->grid->addWidget(label_word, i, 6);
            }
            if (chk_phonetical->isChecked())
            {
                QLabel *label_phonetical = new QLabel(phonetical);
                label_phonetical->setWordWrap(true);
                this->grid->addWidget(label_phonetical, i, 7);
            }
            if (chk_translation->isChecked())
            {
                QLabel *label_translation = new QLabel(translation);
                label_translation->setWordWrap(true);
                this->grid->addWidget(label_translation, i, 8);
            }
            
            this->grid->addWidget(image_widget, i, 9);
            
            appendPlayButtons(i, data, deck_name);
        }
    }
    // push all columns to the left for getting the table a bit more compact
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(data.length(), 100);
}

void QDeckOverviewWidget::appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> data, QString deck_name)
{
    QStringList audio_files = data.at(table_rowid)["audio"].toString().split(",");
    this->max_audio_count = std::max(audio_files.length(), this->max_audio_count);
    
    for (int column = 0; column < audio_files.length(); ++column)
    {
        QString audio_filename_ordered = audio_files.at(column);
        QString audio_filename = "";
        if (audio_filename_ordered.contains(":"))
        {
            audio_filename = audio_filename_ordered.split(":").at(1);
        }
        
        QPushButton *audio_button = nullptr;//new QPushButton();
        if (audio_filename == "none")
        {
            audio_button = new QPushButton();
            audio_button->setIcon(QIcon::fromTheme("media-record"));
            audio_button->setEnabled(false);
        }
        else if (audio_filename != "")
        {
            audio_button = new QPushButton();
            audio_button->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        /*
        else
        {
            audio_button->setIcon(QIcon::fromTheme("media-record"));
            audio_button->setEnabled(false);
        }
        */
        
        if (audio_filename != nullptr)
        {
            connect(audio_button, &QPushButton::clicked, this, [this, audio_button, audio_filename, deck_name]{ audioButtonClicked(audio_button, audio_filename, deck_name); });
            // TODO:
            //table->setCellWidget(table_rowid, column + COLUMN_OFFSET, audio_button);
            this->grid->addWidget(audio_button, table_rowid, column + COLUMN_OFFSET);
            audio_button->setMaximumWidth(BUTTON_WIDTH);
        }
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
    
    refreshTable();
}

void QDeckOverviewWidget::refreshTable()
{
    // store the scrollbar position first to be restored in scrollBarRangeChanged afterwards
    this->scrollbar_pos = this->scroll_area->verticalScrollBar()->sliderPosition();

    this->scroll_widget->deleteLater();
    
    QList<QMap<QString,QVariant>> data = fetchDeckData();
    populateTableWidget(data);
}

void QDeckOverviewWidget::scrollBarRangeChanged(int /*min*/, int max)
{
    if (this->scrollbar_pos <= max)
    {
        this->scroll_area->verticalScrollBar()->setSliderPosition(this->scrollbar_pos);
    }
}

void QDeckOverviewWidget::editRowButtonClicked(QString deck_name, int rowid)
{
    emit showDeckItemRequested(deck_name, rowid);
}

void QDeckOverviewWidget::deleteRowButtonClicked(qlonglong rowid, QMap<QString,QVariant> data_row, QString deck_name)
{
    QString message = "delete " + data_row["name"].toString() + "|" + data_row["word"].toString() + "|" + data_row["phonetical"].toString() + "|" + data_row["translation"].toString() + "?";
    int reply = QMessageBox::question(this, "Delete", message, QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        deleteRow(rowid, deck_name);
    }
}

void QDeckOverviewWidget::deleteRow(qlonglong rowid, QString deck_name)
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
    
    refreshTable();
}

void QDeckOverviewWidget::moveItem(QString deck_name, qlonglong rowid)
{
    MoveItemDialog *dialog = new MoveItemDialog(this->decks_path, deck_name, this->config, rowid);
    connect(dialog, &MoveItemDialog::deleteRow, this, &QDeckOverviewWidget::deleteRow);
    
    dialog->exec();
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
    refreshTable();
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
