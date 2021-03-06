#include "menubar.h"

MenuBar::MenuBar(QDir *decks_paths, QMenuBar *parent)
{
    this->decks_paths = decks_paths;
    
    addFileMenu();
    addSearchMenu();
    addToolsMenu();
    addHelpMenu();
}

void MenuBar::addFileMenu()
{
    QAction *newDecksOverviewAction = new QAction(QIcon::fromTheme("utilities-terminal"), "&New Decks Overview Tab");
    newDecksOverviewAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
    newDecksOverviewAction->setStatusTip("new Decks Overview Tab");
    connect(newDecksOverviewAction, &QAction::triggered, this, &MenuBar::emitNewDecksOverviewTab);
    
    QAction *importAction = new QAction(QIcon::fromTheme("emblem-downloads"), "&Import Deck");
    importAction->setShortcut(QKeySequence::fromString("Ctrl+I"));
    importAction->setStatusTip("Import Modules");
    connect(importAction, &QAction::triggered, this, &MenuBar::onImportAction);
    
    QImage export_img = QIcon::fromTheme("emblem-downloads").pixmap(100).toImage();
    QTransform trans;
    trans.rotate(180);
    QIcon export_icon = QIcon(QPixmap::fromImage(export_img.transformed(trans)));
    QAction *exportAction = new QAction(export_icon, "&Export Deck");
    exportAction->setStatusTip("Export Modules");
    connect(exportAction, &QAction::triggered, this, &MenuBar::onExportAction);
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    fileMenu->addAction(newDecksOverviewAction);
    fileMenu->addSeparator();
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
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

void MenuBar::addToolsMenu()
{
    QAction *statsAction = new QAction(QIcon::fromTheme("dialog-question"), "Stats");
    connect(statsAction, &QAction::triggered, this, &MenuBar::emitStatsTab);
    
    QAction *settingsAction = new QAction(QIcon::fromTheme("applications-system"), "&Settings");
    settingsAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    connect(settingsAction, &QAction::triggered, this, &MenuBar::emitSettingsTab);
    
    QMenu *toolsMenu = addMenu("&Tools");
    toolsMenu->addAction(statsAction);
    toolsMenu->addAction(settingsAction);
}

void MenuBar::addHelpMenu()
{
    QAction *helpMarkersAction = new QAction(QIcon::fromTheme("help-faq"), "Markers");
    connect(helpMarkersAction, &QAction::triggered, this, &MenuBar::emitHelpMarkersTab);
    
    QAction *aboutAction = new QAction(QIcon(":logo"), "About Kulanday");
    connect(aboutAction, &QAction::triggered, this, &MenuBar::emitAboutTab);
    
    QMenu *menu = addMenu("&Help");
    menu->addAction(helpMarkersAction);
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

void MenuBar::emitSettingsTab()
{
    emit newSettingsTab();
}

void MenuBar::emitStatsTab()
{
    emit newStatsTab();
}

void MenuBar::emitHelpMarkersTab()
{
    emit newHelpMarkersTab();
}

void MenuBar::emitAboutTab()
{
    emit newAboutTab();
}

void MenuBar::onImportAction()
{
    QString in_path = QFileDialog::getOpenFileName(this, tr("Import Deck"), QDir::homePath(), tr("*.kdeck"));
    
    QString dirname = QDir(in_path).dirName().split(".").at(0);
    QString out_path = this->decks_paths->absolutePath() + "/" + dirname;
    
    if (! QDir(out_path).exists())
    {
        CompressFolder *cmp = new CompressFolder();
        bool res = cmp->decompressFolder(in_path, out_path);
        
        if (res)
        {
            QMessageBox msg_box;
            msg_box.setText("Module \"" + dirname + "\" successfully imported");
            msg_box.exec();
            
            emit deckImported();
        }
    }
    else if (in_path != "")
    {
        QMessageBox msg_box;
        msg_box.setText("A Module named \"" + dirname + "\" already exists.\nIf you want to import it again, please delete it first.");
        msg_box.exec();
    }
}

void MenuBar::onExportAction()
{
    ExportDecksDialog *dialog = new ExportDecksDialog(this->decks_paths);
    dialog->exec();
}
