#ifndef Q_KULANDAY_MENUBAR_H
#define Q_KULANDAY_MENUBAR_H

#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>

class QKulandayMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit QKulandayMenuBar(QMenuBar *parent = nullptr);
    
signals:
    void newDecksOverviewTab();
    
public slots:
    void quitApplication();
    void emitNewDecksOverviewTab();
};

#endif // Q_KULANDAY_MENUBAR_H
