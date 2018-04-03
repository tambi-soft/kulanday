
#include <QWidget>
#include <QGridLayout>

#include <src/q_kulanday_main_window.h>


QKulandayMainWindow :: QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , grid(new QGridLayout)
{
    resize(600, 400);
    
    //QWidget *central_widget = new QWidget();
    //setCentralWidget(central_widget);
    //central_widget->setLayout(grid);
    
    
    // TODO: stacked widget
    // now for testing
    QDecksOverviewWidget *decks = new QDecksOverviewWidget();
    setCentralWidget(decks);
}
