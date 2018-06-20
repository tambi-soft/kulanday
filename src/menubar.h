#ifndef Q_KULANDAY_MENUBAR_H
#define Q_KULANDAY_MENUBAR_H

#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QDir>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QMenuBar *parent = nullptr);
    
private:
    void addFileMenu();
    void addSearchMenu();
    void addHelpMenu();
    
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
