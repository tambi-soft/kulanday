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

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QDir *deckpath, QMenuBar *parent = nullptr);
    
private:
    QDir *deckpath;
    void addFileMenu();
    void addSearchMenu();
    void addHelpMenu();
    
    void onImportAction();
    void onExportAction();
    
signals:
    void newDecksOverviewTab();
    void newSearchTab();
    void newAboutTab();
    
public slots:
    void quitApplication();
    void emitNewDecksOverviewTab();
    void emitSearchTab();
    void emitAboutTab();
};

#endif // Q_KULANDAY_MENUBAR_H
