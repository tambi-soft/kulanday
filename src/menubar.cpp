#include "menubar.h"

QKulandayMenuBar::QKulandayMenuBar(QMenuBar *parent)
{
    addFileMenu();
    addSearchMenu();
    addHelpMenu();
}

void QKulandayMenuBar::addFileMenu()
{
    QAction *newDecksOverviewAction = new QAction(QIcon::fromTheme("utilities-terminal"), "&New Decks Overview Tab");
    newDecksOverviewAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
    newDecksOverviewAction->setStatusTip("new Decks Overview Tab");
    connect(newDecksOverviewAction, &QAction::triggered, this, &QKulandayMenuBar::emitNewDecksOverviewTab);
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &QKulandayMenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    fileMenu->addAction(newDecksOverviewAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void QKulandayMenuBar::addSearchMenu()
{
    QAction *searchAction = new QAction(QIcon::fromTheme("system-search"), "&Search");
    searchAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    searchAction->setStatusTip("search for an item");
    connect(searchAction, &QAction::triggered, this, &QKulandayMenuBar::emitSearchTab);
    
    QMenu *searchMenu = addMenu("&Search");
    searchMenu->addAction(searchAction);
}

void QKulandayMenuBar::addHelpMenu()
{
    QAction *aboutAction = new QAction(QIcon(":logo"), "About Kulanday");
    connect(aboutAction, &QAction::triggered, this, &QKulandayMenuBar::emitAboutTab);
    
    QMenu *menu = addMenu("&Help");
    menu->addAction(aboutAction);
}

void QKulandayMenuBar::quitApplication()
{
    QApplication::quit();
}

void QKulandayMenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}

void QKulandayMenuBar::emitSearchTab()
{
    emit newSearchTab();
}

void QKulandayMenuBar::emitAboutTab()
{
    emit newAboutTab();
}
