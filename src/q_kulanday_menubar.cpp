#include "q_kulanday_menubar.h"

QKulandayMenuBar::QKulandayMenuBar(QMenuBar *parent)
{
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &QKulandayMenuBar::quitApplication);
    
    
    
    QMenu *fileMenu = addMenu("&File");
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

