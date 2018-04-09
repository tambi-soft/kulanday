
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
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QKulandayMainWindow::closeTab);
    
    QDecksOverviewWidget *decks = new QDecksOverviewWidget();
    tab_widget->addTab(decks, "decks");
    connect(decks, &QDecksOverviewWidget::deckDirtyDozenClicked, this, &QKulandayMainWindow::showDirtyDozenWidget);
    connect(decks, &QDecksOverviewWidget::deckViewClicked, this, &QKulandayMainWindow::showNewDeckWidget);
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

void QKulandayMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QKulandayMainWindow::closeTab(int tab_id)
{
    tab_widget->removeTab(tab_id);
}
