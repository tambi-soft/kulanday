#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent)
{
    addFileMenu();
    addSearchMenu();
    addHelpMenu();
}

void MenuBar::addFileMenu()
{
    QAction *newDecksOverviewAction = new QAction(QIcon::fromTheme("utilities-terminal"), "&New Decks Overview Tab");
    newDecksOverviewAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
    newDecksOverviewAction->setStatusTip("new Decks Overview Tab");
    connect(newDecksOverviewAction, &QAction::triggered, this, &MenuBar::emitNewDecksOverviewTab);
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    fileMenu->addAction(newDecksOverviewAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void MenuBar::addSearchMenu()
{
    QAction *searchAction = new QAction(QIcon::fromTheme("system-search"), "&Search");
    searchAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    searchAction->setStatusTip("search for an item");
    connect(searchAction, &QAction::triggered, this, &MenuBar::emitSearchTab);
    
    QMenu *searchMenu = addMenu("&Search");
    searchMenu->addAction(searchAction);
}

void MenuBar::addHelpMenu()
{
    QAction *aboutAction = new QAction(QIcon(":logo"), "About Kulanday");
    connect(aboutAction, &QAction::triggered, this, &MenuBar::emitAboutTab);
    
    QMenu *menu = addMenu("&Help");
    menu->addAction(aboutAction);
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}

void MenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}

void MenuBar::emitSearchTab()
{
    emit newSearchTab();
}

void MenuBar::emitAboutTab()
{
    emit newAboutTab();
}
