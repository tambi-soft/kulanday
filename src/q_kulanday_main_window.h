
#include <QMainWindow>
#include <QTabWidget>

#include "q_kulanday_menubar.h"
#include "q_decks_overview.h"
#include "q_deck_overview.h"
#include "q_dirtydozen_widget.h"

class QKulandayMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QKulandayMainWindow(QWidget *parent = 0);
    
private:
    QTabWidget *tab_widget;
    QKulandayMenuBar *menu_bar;
    
    int SELECT_DECK_INDEX = 0;
    int DECK_OVERVIEW_INDEX = 1;
    int NEW_DECK_INDEX = 2;
    int DECK_LEARN_INDEX = 3;
    int DECK_DIRTY_DOZEN_INDEX = 4;
    
private slots:
    void showDecksOverviewTab();
    void showDirtyDozenWidget(QString deck_name);
    void showNewDeckWidget(QString deck_name);
    void activateNewTab();
    void closeTab(int tab_id);
};
