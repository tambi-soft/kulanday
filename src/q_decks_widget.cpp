
#include <src/q_decks_widget.h>

QDecksOverviewWidget :: QDecksOverviewWidget(QDir *decks_path, QWidget *parent)
    : layout (new QVBoxLayout)
    , combo (new QComboBox)
    , table (new QTableWidget)
    , new_deck_button (new QPushButton)
{
    resize(600, 400);
    setLayout(layout);
    
    this->decks_path = decks_path;
    
    connect(combo, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboTextChanged);
    
    layout->addWidget(combo);
    layout->addWidget(table);
    layout->addWidget(new_deck_button);
    
    new_deck_button->setText("create new deck");
    
    connect(new_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewDeckClicked);
    
    //decks_path = new QDir(QDir::homePath() + "/.tambi/decks");
    
    table->horizontalHeader()->hide();
    //table->verticalHeader()->hide();
    
    populateComboBox();
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewDeckClicked()
{
    //QCreateNewDeckDialog *new_deck_dialog = new QCreateNewDeckDialog(decks_path);
    //new_deck_dialog->exec();
    QString default_deck_path = QDir::homePath();
    //QUrl url = QFileDialog::getExistingDirectoryUrl(this, "Select Directory for the new Deck", default_deck_path, QFileDialog::ShowDirsOnly);
    QString url_path = QFileDialog::getExistingDirectory(this, tr("Create New Deck"),
                                                    default_deck_path + "/.tambi/decks",
                                                    QFileDialog::ShowDirsOnly);
    
    emit createNewDeck(QUrl(url_path));
    
    table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::populateComboBox()
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
    this->combo->blockSignals(true);
    this->combo->addItems(items);
    this->combo->blockSignals(false);
}

void QDecksOverviewWidget::populateDecksOverview()
{
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    this->table->setColumnCount(4);
    this->table->setRowCount(decks_names.length());
    
    int i = -1;
    foreach (QString deck_name, decks_names)
    {
        if (this->combo->currentText() == "[all]" || deck_name.startsWith(this->combo->currentText()))
        {
            i++;
            
            QTableWidgetItem *item = new QTableWidgetItem(deck_name);
            this->table->setItem(i, 0, item);
            item->setFlags(Qt::ItemIsEnabled);
            
            QPushButton *button_learn_deck = new QPushButton("learn");
            QPushButton *button_dirty_dozen = new QPushButton("dirty dozen");
            QPushButton *button_view_deck = new QPushButton("view deck");
            
            this->table->setCellWidget(i, 1, button_learn_deck);
            this->table->setCellWidget(i, 2, button_dirty_dozen);
            this->table->setCellWidget(i, 3, button_view_deck);
            
            //connect(button_dirty_dozen, &QPushButton::clicked, this, &QDecksOverviewWidget::tableButtonDirtyDozenClicked);
            connect(button_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{  tableButtonDirtyDozenClicked(deck_name); });
            
            //connect(button_view_deck, &QPushButton::clicked, this, &QDecksOverviewWidget::tableButtonViewDeckClicked);
            connect(button_view_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonViewDeckClicked(deck_name); });
        }
    }
    
    this->table->setRowCount(i+1);
    this->table->resizeColumnsToContents();
}

void QDecksOverviewWidget::tableButtonDirtyDozenClicked(QString deck_name)
{
    emit deckDirtyDozenClicked(deck_name);
}

void QDecksOverviewWidget::tableButtonViewDeckClicked(QString deck_name)
{
    emit deckViewClicked(deck_name);
}

void QDecksOverviewWidget::onComboTextChanged(QString text)
{
    this->table->clear();
    populateDecksOverview();
}
