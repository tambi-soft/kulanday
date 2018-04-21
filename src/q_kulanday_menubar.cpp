#include "q_kulanday_menubar.h"

QKulandayMenuBar::QKulandayMenuBar(QMenuBar *parent)
{
    /*
    QAction *newDecksOverviewAction = new QAction(QIcon::fromTheme("utilities-terminal"), "&New Decks Overview Tab");
    newDecksOverviewAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
    newDecksOverviewAction->setStatusTip("new Decks Overview Tab");
    connect(newDecksOverviewAction, &QAction::triggered, this, &QKulandayMenuBar::emitNewDecksOverviewTab);
    */
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &QKulandayMenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    //fileMenu->addAction(newDecksOverviewAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void QKulandayMenuBar::quitApplication()
{
    QApplication::quit();
}

void QKulandayMenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}

