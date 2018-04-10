
#include "q_kulanday_main_window.h"

QKulandayMainWindow::QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
    , menu_bar (new QKulandayMenuBar)
{
    resize(600, 600);
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    setMenuBar(menu_bar);
    connect(menu_bar, &QKulandayMenuBar::newDecksOverviewTab, this, &QKulandayMainWindow::showDecksOverviewTab);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QKulandayMainWindow::closeTab);
    
    showDecksOverviewTab();
}

void QKulandayMainWindow::showDecksOverviewTab()
{
    QDecksOverviewWidget *decks = new QDecksOverviewWidget();
    tab_widget->addTab(decks, "decks");
    connect(decks, &QDecksOverviewWidget::deckDirtyDozenClicked, this, &QKulandayMainWindow::showDirtyDozenWidget);
    connect(decks, &QDecksOverviewWidget::deckViewClicked, this, &QKulandayMainWindow::showNewDeckWidget);
    connect(decks, &QDecksOverviewWidget::createNewDeck, this, &QKulandayMainWindow::createNewDeck);
    
    activateNewTab();
}

void QKulandayMainWindow::showDirtyDozenWidget(QString deck_name)
{
    QDirtyDozenWidget *dd = new QDirtyDozenWidget(deck_name);
    tab_widget->addTab(dd, "dirty dozen: " + deck_name);
    
    activateNewTab();
}

void QKulandayMainWindow::showNewDeckWidget(QString deck_name)
{
    QDeckOverviewWidget *deck = new QDeckOverviewWidget(deck_name);
    tab_widget->addTab(deck, deck_name);
    
    activateNewTab();
}

void QKulandayMainWindow::createNewDeck(QUrl deck_url)
{
    if (! deck_url.isEmpty())
    {
        QDeckOverviewWidget *deck = new QDeckOverviewWidget(deck_url.path());
        tab_widget->addTab(deck, deck_url.path());
        
        activateNewTab();
    }
}

void QKulandayMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QKulandayMainWindow::closeTab(int tab_id)
{
    tab_widget->removeTab(tab_id);
}
