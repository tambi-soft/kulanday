#ifndef Q_KULANDAY_MENUBAR_H
#define Q_KULANDAY_MENUBAR_H

#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTransform>
#include <QImage>
#include <QMessageBox>
#include <QDebug>

#include "compress_folder.h"
#include "export_decks_dialog.h"

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QDir *decks_paths, QMenuBar *parent = nullptr);
    
private:
    QDir *decks_paths;
    void addFileMenu();
    void addSearchMenu();
    void addToolsMenu();
    void addHelpMenu();
    
    void onImportAction();
    void onExportAction();
    
signals:
    void newDecksOverviewTab();
    void newSearchTab();
    void newSettingsTab();
    void newStatsTab();
    void newHelpMarkersTab();
    void newAboutTab();
    void deckImported();
    
public slots:
    void quitApplication();
    void emitNewDecksOverviewTab();
    void emitSearchTab();
    void emitSettingsTab();
    void emitStatsTab();
    void emitHelpMarkersTab();
    void emitAboutTab();
};

#endif // Q_KULANDAY_MENUBAR_H
