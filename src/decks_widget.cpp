
#include <src/decks_widget.h>

QDecksOverviewWidget :: QDecksOverviewWidget(Config *config, QDir *decks_path, QWidget */*parent*/)
    : layout (new QGridLayout)
    , combo_status_filter (new QComboBox)
    , table (new QTableWidget)
{
    resize(600, 400);
    setLayout(layout);
    
    this->config = config;
    this->decks_path = decks_path;
    
    this->combo_name_filter = new FilterLanguageCombo(decks_path, config);
    
    COMBO_STATI << "ToDo" << "perfect" << "good" << "mediocre" << "weak" << "poor" << "none";
    COMBO_STATI_FILTER << "[all]" << "ToDo" << "perfect" << "good" << "mediocre" << "weak" << "poor" << "none";
    
    this->database = new DbAdapterMeta(decks_path);
    
    connect(combo_name_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboNameFilterTextChanged);
    
    populateComboStatusFilter(combo_status_filter);
    connect(combo_status_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboStatusFilterTextChanged);
    
    combo_name_filter->setToolTip("filter for decks with the selected prefix");
    combo_status_filter->setToolTip("filter for decks with the selected tag");
    
    QPushButton *refresh_button = new QPushButton("refresh");
    connect(refresh_button, &QPushButton::clicked, this, &QDecksOverviewWidget::refreshTable);
    
    QPushButton *new_markers_deck_button = new QPushButton("create new markers deck");
    connect(new_markers_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewMarkersDeckClicked);
    
    QPushButton *new_simple_deck_button = new QPushButton("create new deck");
    connect(new_simple_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewSimpleDeckClicked);
    
    layout->addWidget(new QLabel("filter:"), 0, 0);
    layout->addWidget(combo_name_filter, 0, 1);
    layout->addWidget(combo_status_filter, 0, 2);
    layout->addWidget(table, 1, 0, 1, 3);
    layout->addWidget(refresh_button, 2, 0);
    //layout->addWidget(new_markers_deck_button, 2, 1);
    layout->addWidget(new_simple_deck_button, 2, 2);
    
    //table->horizontalHeader()->hide();
    //table->verticalHeader()->hide();
    
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewSimpleDeckClicked()
{
    QString default_deck_path = this->decks_path->absolutePath();
    
    QString url_path = QCreateNewDeckDialog::getExistingDirectory(default_deck_path);
    
    emit createNewDeck(QUrl(url_path));
    
    table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewMarkersDeckClicked()
{
    
}

void QDecksOverviewWidget::refreshTable()
{
    table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::populateDecksOverview()
{
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    this->database->populateDecksTable(decks_names);
    QMap<QString,QString> decks_meta = this->database->selectDecksStati();
    QMap<QString,QString> decks_meta_learned = this->database->selectDecksLearned();
    
    this->table->setColumnCount(7);
    this->table->setRowCount(decks_names.length());
    
    int i = -1;
    foreach (QString deck_name, decks_names)
    {
        if (this->combo_name_filter->currentText() == "[all]"
                || deck_name.startsWith(this->combo_name_filter->currentText()))
        {
            QString current_filter_text = this->combo_status_filter->currentText();
            //QComboBox *combo_widget = this->table->cellWidget(0, 4);
            
            if (current_filter_text == "[all]"
                    || current_filter_text == decks_meta[deck_name])
            {
                i++;
                
                QTableWidgetItem *item_name = new QTableWidgetItem(deck_name);
                item_name->setFlags(Qt::ItemIsEnabled);
                
                QPushButton *button_dirty_dozen = new QPushButton();
                button_dirty_dozen->setIcon(QIcon::fromTheme("image-loading"));
                connect(button_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{  tableButtonDirtyDozenClicked(deck_name); });
                button_dirty_dozen->setToolTip("dirty dozen");
                
                QPushButton *button_inv_dirty_dozen = new QPushButton();
                button_inv_dirty_dozen->setIcon(QIcon::fromTheme("image-x-generic"));
                connect(button_inv_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{ tableButtonLearnClicked(deck_name); });
                button_inv_dirty_dozen->setToolTip("inverted dirty dozen");
                
                QPushButton *button_view_deck = new QPushButton();
                button_view_deck->setIcon(QIcon::fromTheme("folder-open"));
                connect(button_view_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonViewDeckClicked(deck_name); });
                button_view_deck->setToolTip("view / edit deck");
                
                QPushButton *button_delete_deck = new QPushButton();
                button_delete_deck->setIcon(QIcon::fromTheme("edit-delete"));
                connect(button_delete_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonDeleteDeckClicked(deck_name); });
                button_delete_deck->setToolTip("delete deck");
                
                QComboBox *combo_status = populateComboStatus(deck_name);
                if (decks_meta[deck_name] != "")
                {
                    combo_status->setCurrentText(decks_meta[deck_name]);
                }
                
                QTableWidgetItem *item_last_learned;
                if (decks_meta_learned[deck_name] == "")
                {
                    item_last_learned = new QTableWidgetItem("never");
                }
                else
                {
                    QDateTime timestamp;
                    timestamp.setTime_t(decks_meta_learned[deck_name].toInt());
                    item_last_learned = new QTableWidgetItem(timestamp.toString("dd. MMM yy"));
                }
                
                item_last_learned->setFlags(Qt::ItemIsEnabled);
                
                this->table->setCellWidget(i, 0, button_view_deck);
                this->table->setItem(i, 1, item_name);
                this->table->setCellWidget(i, 2, button_dirty_dozen);
                this->table->setCellWidget(i, 3, button_inv_dirty_dozen);
                this->table->setCellWidget(i, 4, combo_status);
                this->table->setItem(i, 5, item_last_learned);
                this->table->setCellWidget(i, 6, button_delete_deck);
            }
        }
    }
    
    this->table->setRowCount(i+1);
    
    
    QStringList labels;
    labels << "view" << "deck name" << "learn a" << "learn b" << "tag" << "last learned" << "";
    table->setHorizontalHeaderLabels(labels);
    table->verticalHeader()->hide();
    
    QTableWidgetItem *dd_header_item = this->table->horizontalHeaderItem(2);
    if (dd_header_item)
    {
        dd_header_item->setToolTip("dirty dozen");
    }
    
    QTableWidgetItem *inv_dd_header_item = this->table->horizontalHeaderItem(3);
    if (inv_dd_header_item)
    {
        inv_dd_header_item->setToolTip("inverted dirty dozen");
    }
    
    // using visibility-toggling as workaround to also resize columns to header label
    this->table->setVisible(false);
    this->table->resizeColumnsToContents();
    this->table->setVisible(true);
}

QComboBox *QDecksOverviewWidget::populateComboStatus(QString deck_name)
{
    QComboBox *combo = new QComboBox();
    combo->addItems(COMBO_STATI);
    //combo->addItems(COMBO_STATI_FILTER);
    
    combo->setItemData(0, QColor(Qt::blue ), Qt::BackgroundRole);
    combo->setItemData(1, QColor(0, 180, 0 ), Qt::BackgroundRole);
    combo->setItemData(2, QColor(0, 255, 0 ), Qt::BackgroundRole);
    combo->setItemData(3, QColor(200, 255, 0 ), Qt::BackgroundRole);
    combo->setItemData(4, QColor(Qt::yellow ), Qt::BackgroundRole);
    combo->setItemData(5, QColor(Qt::red ), Qt::BackgroundRole);
    combo->setItemData(6, QColor(Qt::white), Qt::BackgroundRole);
    
    combo->setStyleSheet("QFrame { border: 5px solid grey; }\
                                QComboBox { background-color: white; }");
    
    combo->setCurrentIndex(COMBO_STATI.length()-1);
    
    connect(combo, &QComboBox::currentTextChanged, this, [this, deck_name, combo]{ onComboStatusTextChanged(deck_name, combo); });
    
    return combo;
}

void QDecksOverviewWidget::populateComboStatusFilter(QComboBox *combo)
{
    combo->addItems(COMBO_STATI_FILTER);
    
    combo->setItemData(0, QColor(Qt::white), Qt::BackgroundRole);
    combo->setItemData(1, QColor(Qt::blue ), Qt::BackgroundRole);
    combo->setItemData(2, QColor(0, 180, 0 ), Qt::BackgroundRole);
    combo->setItemData(3, QColor(0, 255, 0 ), Qt::BackgroundRole);
    combo->setItemData(4, QColor(200, 255, 0 ), Qt::BackgroundRole);
    combo->setItemData(5, QColor(Qt::yellow ), Qt::BackgroundRole);
    combo->setItemData(6, QColor(Qt::red ), Qt::BackgroundRole);
    combo->setItemData(7, QColor(Qt::white), Qt::BackgroundRole);
    
    combo->setStyleSheet("QFrame { border: 5px solid grey; }\
                                QComboBox { background-color: white; }");
                                
    combo->setCurrentIndex(0);
}

void QDecksOverviewWidget::tableButtonLearnClicked(QString deck_name)
{
    this->database->updateLastLearned(deck_name);
    
    emit deckLearnClicked(deck_name);
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::tableButtonDirtyDozenClicked(QString deck_name)
{
    this->database->updateLastLearned(deck_name);
    
    emit deckDirtyDozenClicked(deck_name);
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::tableButtonViewDeckClicked(QString deck_name)
{
    emit deckViewClicked(deck_name);
}

void QDecksOverviewWidget::tableButtonDeleteDeckClicked(QString deck_name)
{
    QString message = "delete deck \"" + deck_name + "\"?";
    
    int reply = QMessageBox::question(this, "Delete", message, QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        emit deleteDeck(deck_name);
    }
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::onComboNameFilterTextChanged(QString text)
{
    this->config->setLastLanguageFilter(text);
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::onComboStatusFilterTextChanged(QString text)
{
    comboFilterColorAdjust(this->combo_status_filter);
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::onComboStatusTextChanged(QString deck_name, QComboBox *combo_status)
{
    this->database->updateDeckStatus(deck_name, combo_status->currentText());
    comboColorAdjust(combo_status);
}

void QDecksOverviewWidget::comboColorAdjust(QComboBox *combo)
{
    QString text = combo->currentText();
    if (text == COMBO_STATI[0])
    {
        combo->setStyleSheet("QComboBox { background-color: blue; }");
    }
    else if (text == COMBO_STATI[1])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(0, 180, 0); }");
    }
    else if (text == COMBO_STATI[2])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(0, 255, 0); }");
    }
    else if (text == COMBO_STATI[3])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(200, 255, 0); }");
    }
    else if (text == COMBO_STATI[4])
    {
        combo->setStyleSheet("QComboBox { background-color: yellow; }");
    }
    else if (text == COMBO_STATI[5])
    {
        combo->setStyleSheet("QComboBox { background-color: red; }");
    }
    else if (text == COMBO_STATI[6])
    {
        combo->setStyleSheet("QComboBox { background-color: lightgrey; }");
    }
}

void QDecksOverviewWidget::comboFilterColorAdjust(QComboBox *combo)
{
    QString text = combo->currentText();
    if (text == COMBO_STATI_FILTER[0])
    {
        combo->setStyleSheet("QComboBox { color: black; background-color: white; }");
    }
    else if (text == COMBO_STATI_FILTER[1])
    {
        combo->setStyleSheet("QComboBox { background-color: blue; }");
    }
    else if (text == COMBO_STATI_FILTER[2])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(0, 180, 0); }");
    }
    else if (text == COMBO_STATI_FILTER[3])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(0, 255, 0); }");
    }
    else if (text == COMBO_STATI_FILTER[4])
    {
        combo->setStyleSheet("QComboBox { background-color: rgb(200, 255, 0); }");
    }
    else if (text == COMBO_STATI_FILTER[5])
    {
        combo->setStyleSheet("QComboBox { background-color: yellow; }");
    }
    else if (text == COMBO_STATI_FILTER[6])
    {
        combo->setStyleSheet("QComboBox { background-color: red; }");
    }
    else if (text == COMBO_STATI_FILTER[7])
    {
        combo->setStyleSheet("QComboBox { background-color: lightgrey; }");
    }
}
