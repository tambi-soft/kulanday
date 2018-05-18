
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>

#include "q_kulanday_menubar.h"
#include "q_decks_widget.h"
#include "q_deck_widget.h"
#include "q_inv_dirty_dozen_widget.h"
#include "q_dirtydozen_widget.h"
#include "q_deck_item_widget.h"
#include "db_adapter.h"
#include "config.h"

class QKulandayMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QKulandayMainWindow(QWidget *parent = 0);
    
private:
    QTabWidget *tab_widget;
    QKulandayMenuBar *menu_bar;
    
    QDir *deckpath;
    
    int SELECT_DECK_INDEX = 0;
    int DECK_OVERVIEW_INDEX = 1;
    int NEW_DECK_INDEX = 2;
    int DECK_LEARN_INDEX = 3;
    int DECK_DIRTY_DOZEN_INDEX = 4;
    
    void deactivateDecksOverviewCloseButton();
    QMap<QString,int> deck_item_widgets; // {deck_name} OR {deck_name}_{item_id} -> tab widget id
    
private slots:
    void showDecksOverviewTab();
    void showLearnWidget(QString deck_name);
    void showDirtyDozenWidget(QString deck_name);
    void showDeckWidget(QString deck_name);
    void createNewDeck(QUrl deck_url);
    void createNewDeckItem(QString deck_name);
    void deleteDeck(QString deck_name);
    void showDeckItem(QString deck_name, int rowid);
    void onDeckItemContentsUpdated(QString deck_name);
    
    void activateNewTab();
    void closeTab(int tab_id);
    void onTabMoved(int from, int to);
};
