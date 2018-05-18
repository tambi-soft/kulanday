
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
    
    connect(combo_name_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboNameFilterTextChanged);
    
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
        if (this->combo_name_filter->currentText() == "[all]" || deck_name.startsWith(this->combo_name_filter->currentText()))
        {
            i++;
            
            QTableWidgetItem *item_name = new QTableWidgetItem(deck_name);
            item_name->setFlags(Qt::ItemIsEnabled);
            
            QPushButton *button_dirty_dozen = new QPushButton("dirty dozen");
            QPushButton *button_inv_dirty_dozen = new QPushButton("inv. dirty dozen");
            QPushButton *button_view_deck = new QPushButton();
            button_view_deck->setIcon(QIcon::fromTheme("document-properties"));
            
            QPushButton *button_delete_deck = new QPushButton();
            button_delete_deck->setIcon(QIcon::fromTheme("edit-delete"));
            
            QComboBox *combo_status = new QComboBox();
            
            QTableWidgetItem *item_last_learned = new QTableWidgetItem("never");
            item_last_learned->setFlags(Qt::ItemIsEnabled);
            
            this->table->setCellWidget(i, 0, button_view_deck);
            this->table->setCellWidget(i, 1, button_delete_deck);
            this->table->setItem(i, 2, item_name);
            this->table->setCellWidget(i, 3, button_dirty_dozen);
            this->table->setCellWidget(i, 4, button_inv_dirty_dozen);
            this->table->setCellWidget(i, 5, combo_status);
            this->table->setItem(i, 6, item_last_learned);
            
            connect(button_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{  tableButtonDirtyDozenClicked(deck_name); });
            
            connect(button_inv_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{ tableButtonLearnClicked(deck_name); });
            
            connect(button_view_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonViewDeckClicked(deck_name); });
        }
    }
    
    this->table->setRowCount(i+1);
    this->table->resizeColumnsToContents();
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

void QDecksOverviewWidget::onComboNameFilterTextChanged(QString text)
{
    this->table->clear();
    populateDecksOverview();
}
