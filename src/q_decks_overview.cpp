
#include <src/q_decks_overview.h>

QDecksOverviewWidget :: QDecksOverviewWidget(QWidget *parent)
    : layout (new QVBoxLayout)
    , table (new QTableWidget)
    , new_deck_button (new QPushButton)
{
    resize(600, 400);
    setLayout(layout);
    
    layout->addWidget(table);
    layout->addWidget(new_deck_button);
    
    new_deck_button->setText("create new deck");
    
    connect(new_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewDeck);
    
    decks_path = new QDir(QDir::homePath() + "/.tambi/decks");
    
    table->horizontalHeader()->hide();
    //table->verticalHeader()->hide();
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewDeck()
{
    QCreateNewDeckDialog *new_deck_dialog = new QCreateNewDeckDialog(decks_path);
    //QDir new_deck_dir = new_deck_dialog->
}

void QDecksOverviewWidget::populateDecksOverview()
{
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    this->table->setColumnCount(4);
    this->table->setRowCount(decks_names.length());
    
    int i = -1;
    foreach (QString deck_name, decks_names)
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
