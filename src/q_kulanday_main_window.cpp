
#include "q_kulanday_main_window.h"
#include "q_deck_overview.h"
#include "q_dirtydozen_widget.h"

QKulandayMainWindow :: QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(600, 600);
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    QDirtyDozenWidget *dirty_dozen = new QDirtyDozenWidget();
    tab_widget->addTab(dirty_dozen, "dirty dozen");
    
    QDecksOverviewWidget *decks = new QDecksOverviewWidget();
    tab_widget->addTab(decks, "decks");
    
    QDeckOverviewWidget *deck = new QDeckOverviewWidget();
    tab_widget->addTab(deck, "deck");
}
