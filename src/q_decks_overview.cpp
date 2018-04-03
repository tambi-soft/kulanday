
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
    
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewDeck()
{
    
}

void QDecksOverviewWidget::populateDecksOverview()
{
    QDir *decks = new QDir(QDir::homePath() + "/.tambi/decks");
    QStringList decks_names = decks->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    this->table->setColumnCount(4);
    this->table->setRowCount(decks_names.length());
    
    int i = -1;
    foreach (QString deck, decks_names)
    {
        i++;
        
        QTableWidgetItem *item = new QTableWidgetItem(deck);
        this->table->setItem(i, 0, item);
        item->setFlags(Qt::ItemIsEnabled);
        
        QPushButton *button_learn_deck = new QPushButton("learn");
        QPushButton *button_dirty_dozen = new QPushButton("dirty dozen");
        QPushButton *button_view_deck = new QPushButton("view deck");
        
        this->table->setCellWidget(i, 1, button_learn_deck);
        this->table->setCellWidget(i, 2, button_dirty_dozen);
        this->table->setCellWidget(i, 3, button_view_deck);
        
        connect(button_dirty_dozen, &QPushButton::clicked, this, &QDecksOverviewWidget::tableButtonDirtyDozenClicked);
        
        connect(button_view_deck, &QPushButton::clicked, this, &QDecksOverviewWidget::tableButtonViewDeckClicked);
        
    }
    
    this->table->resizeColumnsToContents();
}

void QDecksOverviewWidget::tableButtonDirtyDozenClicked()
{
    
}

void QDecksOverviewWidget::tableButtonViewDeckClicked()
{
    
}
