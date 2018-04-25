
#include "q_kulanday_main_window.h"

QKulandayMainWindow::QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
    , menu_bar (new QKulandayMenuBar)
{
    resize(800, 700);
    
    this->deckpath = new QDir(QDir::homePath() + "/.tambi/decks/");
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    setMenuBar(menu_bar);
    connect(menu_bar, &QKulandayMenuBar::newDecksOverviewTab, this, &QKulandayMainWindow::showDecksOverviewTab);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QKulandayMainWindow::closeTab);
    
    QTabBar *tab_bar = tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QKulandayMainWindow::onTabMoved);
    
    showDecksOverviewTab();
    
    deactivateDecksOverviewCloseButton();
}

void QKulandayMainWindow::showDecksOverviewTab()
{
    QDecksOverviewWidget *decks = new QDecksOverviewWidget(this->deckpath);
    tab_widget->addTab(decks, "decks");
    connect(decks, &QDecksOverviewWidget::deckDirtyDozenClicked, this, &QKulandayMainWindow::showDirtyDozenWidget);
    connect(decks, &QDecksOverviewWidget::deckViewClicked, this, &QKulandayMainWindow::showDeckWidget);
    connect(decks, &QDecksOverviewWidget::createNewDeck, this, &QKulandayMainWindow::createNewDeck);
    
    activateNewTab();
}

void QKulandayMainWindow::showDirtyDozenWidget(QString deck_name)
{
    QDirtyDozenWidget *dd = new QDirtyDozenWidget(this->deckpath, deck_name);
    tab_widget->addTab(dd, "dirty dozen: " + deck_name);
    
    activateNewTab();
}

void QKulandayMainWindow::showDeckWidget(QString deck_name)
{   
    if (this->deck_item_widgets.contains(deck_name))
    {
        int index = this->deck_item_widgets[deck_name];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckOverviewWidget *deck = new QDeckOverviewWidget(this->deckpath, deck_name);
        connect(deck, &QDeckOverviewWidget::newDeckItemRequested, this, &QKulandayMainWindow::createNewDeckItem);
        connect(deck, &QDeckOverviewWidget::showDeckItemRequested, this, &QKulandayMainWindow::showDeckItem);
        
        tab_widget->addTab(deck, deck_name);
        activateNewTab();
        
        this->deck_item_widgets[deck_name] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::createNewDeck(QUrl deck_url)
{
    if (! deck_url.isEmpty())
    {
        showDeckWidget(deck_url.fileName());
    }
}

void QKulandayMainWindow::createNewDeckItem(QString deck_name)
{
    QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name);
    connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
    tab_widget->addTab(deck_item, "item: " + deck_name);
    
    activateNewTab();
    
    int rowid = deck_item->rowid;
    this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
}

void QKulandayMainWindow::showDeckItem(QString deck_name, int rowid)
{
    if (this->deck_item_widgets.contains(deck_name + "_" + QString::number(rowid)))
    {
        int index = this->deck_item_widgets[deck_name + "_" + QString::number(rowid)];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name, rowid);
        connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
        tab_widget->addTab(deck_item, "item: " + deck_name);
        
        activateNewTab();
        
        this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::onDeckItemContentsUpdated(QString deck_name)
{
    int deck_id = this->deck_item_widgets[deck_name];
    QDeckOverviewWidget *deck = (QDeckOverviewWidget*) tab_widget->widget(deck_id);
    deck->refresh();
    
}

void QKulandayMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QKulandayMainWindow::closeTab(int tab_id)
{
    tab_widget->removeTab(tab_id);
    
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        // remove key->value pair from the map
        if (value == tab_id)
        {
            this->deck_item_widgets.remove(k);
        }
        // adjust the map to match the new tab ids
        else if (value > tab_id)
        {
            this->deck_item_widgets[k] = value -1;
        }
    }
}

void QKulandayMainWindow::onTabMoved(int from, int to)
{
    // adjust the map to match the new tab ids
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        
        if (value == from)
        {
            this->deck_item_widgets[k] = to;
        }
        if (value == to)
        {
            this->deck_item_widgets[k] = from;
        }
        
    }
}

void QKulandayMainWindow::deactivateDecksOverviewCloseButton()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
}
