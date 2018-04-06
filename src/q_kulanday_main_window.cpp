
#include <QWidget>
#include <QGridLayout>

#include <src/q_kulanday_main_window.h>
#include <src/q_deck_overview.h>
#include "q_dirtydozen_widget.h"

QKulandayMainWindow :: QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , grid(new QGridLayout)
{
    resize(600, 600);
    
    //QWidget *central_widget = new QWidget();
    //setCentralWidget(central_widget);
    //central_widget->setLayout(grid);
    
    
    // TODO: stacked widget
    // now for testing
    
    //QDecksOverviewWidget *decks = new QDecksOverviewWidget();
    //setCentralWidget(decks);
    
    //QDeckOverviewWidget *deck = new QDeckOverviewWidget();
    //setCentralWidget(deck);
    
    QDirtyDozenWidget *dirty_dozen = new QDirtyDozenWidget();
    setCentralWidget(dirty_dozen);
}
