
#include <src/q_decks_widget.h>

QDecksOverviewWidget :: QDecksOverviewWidget(QDir *decks_path, QWidget *parent)
    : layout (new QGridLayout)
    , combo_name_filter (new QComboBox)
    , combo_status_filter (new QComboBox)
    , table (new QTableWidget)
{
    resize(600, 400);
    setLayout(layout);
    
    this->decks_path = decks_path;
    
    COMBO_STATI << "ToDo" << "perfect" << "good" << "weak" << "poor" << "none";
    COMBO_STATI_FILTER << "[all]" << "ToDo" << "perfect" << "good" << "weak" << "poor" << "none";
    
    connect(combo_name_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboNameFilterTextChanged);
    
    populateComboStatusFilter(combo_status_filter);
    connect(combo_status_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboStatusFilterTextChanged);
    
    QPushButton *refresh_button = new QPushButton("refresh");
    connect(refresh_button, &QPushButton::clicked, this, &QDecksOverviewWidget::refreshTable);
    
    QPushButton *new_deck_button = new QPushButton("create new deck");
    connect(new_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewDeckClicked);
    
    layout->addWidget(combo_name_filter, 0, 0);
    layout->addWidget(combo_status_filter, 0, 1);
    layout->addWidget(table, 1, 0, 1, 2);
    layout->addWidget(refresh_button, 2, 0);
    layout->addWidget(new_deck_button, 2, 1);
    
    table->horizontalHeader()->hide();
    //table->verticalHeader()->hide();
    
    populateComboNameFilterBox();
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewDeckClicked()
{
    //QCreateNewDeckDialog *new_deck_dialog = new QCreateNewDeckDialog(decks_path);
    //new_deck_dialog->exec();
    QString default_deck_path = this->decks_path->absolutePath();
    //QUrl url = QFileDialog::getExistingDirectoryUrl(this, "Select Directory for the new Deck", default_deck_path, QFileDialog::ShowDirsOnly);
    QString url_path = QFileDialog::getExistingDirectory(this, tr("Create New Deck"),
                                                    default_deck_path,
                                                    QFileDialog::ShowDirsOnly);
    
    emit createNewDeck(QUrl(url_path));
    
    table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::refreshTable()
{
    table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::populateComboNameFilterBox()
{
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    QStringList items;
    items << "[all]";
    foreach (QString decks_name, decks_names) {
        QString item = decks_name.split("_")[0];
        if (! items.contains(item + "_") && decks_name.contains("_"))
        {
            items.append(item + "_");
        }
    }
    this->combo_name_filter->blockSignals(true);
    this->combo_name_filter->addItems(items);
    this->combo_name_filter->blockSignals(false);
}

void QDecksOverviewWidget::populateComboStatusFilterBox()
{
    
}

void QDecksOverviewWidget::populateDecksOverview()
{
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
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
                    || current_filter_text == "none")
            {
                i++;
                
                QTableWidgetItem *item_name = new QTableWidgetItem(deck_name);
                item_name->setFlags(Qt::ItemIsEnabled);
                
                QPushButton *button_dirty_dozen = new QPushButton("dirty dozen");
                connect(button_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{  tableButtonDirtyDozenClicked(deck_name); });
                
                QPushButton *button_inv_dirty_dozen = new QPushButton("inv. dirty dozen");
                connect(button_inv_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{ tableButtonLearnClicked(deck_name); });
                
                QPushButton *button_view_deck = new QPushButton();
                button_view_deck->setIcon(QIcon::fromTheme("folder-open"));
                connect(button_view_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonViewDeckClicked(deck_name); });
                
                QPushButton *button_delete_deck = new QPushButton();
                button_delete_deck->setIcon(QIcon::fromTheme("edit-delete"));
                connect(button_delete_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonDeleteDeckClicked(deck_name); });
                
                QComboBox *combo_status = populateComboStatus(deck_name);
                
                QTableWidgetItem *item_last_learned = new QTableWidgetItem("never");
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
    this->table->resizeColumnsToContents();
}

QComboBox *QDecksOverviewWidget::populateComboStatus(QString deck_name)
{
    QComboBox *combo_status = new QComboBox();
    combo_status->addItems(COMBO_STATI);
    
    combo_status->setItemData( 0, QColor( Qt::blue ), Qt::BackgroundRole );
    combo_status->setItemData( 1, QColor( 0, 180, 0 ), Qt::BackgroundRole );
    combo_status->setItemData( 2, QColor( 0, 255, 0 ), Qt::BackgroundRole );
    combo_status->setItemData( 3, QColor( Qt::yellow ), Qt::BackgroundRole );
    combo_status->setItemData( 4, QColor( Qt::red ), Qt::BackgroundRole );
    
    combo_status->setStyleSheet("QFrame { border: 4px solid black; }\
                                QComboBox { background-color: lightgrey; }");
                                
    combo_status->setCurrentIndex(COMBO_STATI.length()-1);
    
    connect(combo_status, &QComboBox::currentTextChanged, this, [this, deck_name, combo_status]{ onComboStatusTextChanged(deck_name, combo_status); });
    
    return combo_status;
}

void QDecksOverviewWidget::populateComboStatusFilter(QComboBox *combo)
{
    combo->addItems(COMBO_STATI_FILTER);
    
    combo->setItemData(0, QColor(Qt::white), Qt::BackgroundRole);
    combo->setItemData(1, QColor( Qt::blue ), Qt::BackgroundRole );
    combo->setItemData(2, QColor( 0, 180, 0 ), Qt::BackgroundRole );
    combo->setItemData(3, QColor( 0, 255, 0 ), Qt::BackgroundRole );
    combo->setItemData(4, QColor( Qt::yellow ), Qt::BackgroundRole );
    combo->setItemData(5, QColor( Qt::red ), Qt::BackgroundRole );
    
    combo->setStyleSheet("QFrame { border: 4px solid black; }\
                                QComboBox { background-color: white; }");
                                
    combo->setCurrentIndex(0);
}

void QDecksOverviewWidget::tableButtonLearnClicked(QString deck_name)
{
    emit deckLearnClicked(deck_name);
}

void QDecksOverviewWidget::tableButtonDirtyDozenClicked(QString deck_name)
{
    emit deckDirtyDozenClicked(deck_name);
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
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::onComboStatusFilterTextChanged(QString text)
{
    comboColorAdjust(this->combo_status_filter);
    
    this->table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::onComboStatusTextChanged(QString deck_name, QComboBox *combo_status)
{
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
        combo->setStyleSheet("QComboBox { background-color: yellow; }");
    }
    else if (text == COMBO_STATI[4])
    {
        combo->setStyleSheet("QComboBox { background-color: red; }");
    }
    else if (text == COMBO_STATI[5])
    {
        combo->setStyleSheet("QComboBox { background-color: lightgrey; }");
    }
}
