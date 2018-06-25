
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "menubar.h"
#include "decks_widget.h"
#include "deck_widget.h"
#include "inv_dirty_dozen_widget.h"
#include "dirtydozen_widget.h"
#include "deck_item_widget.h"
#include "db_adapter.h"
#include "config.h"
#include "search_widget.h"
#include "about_widget.h"

class QKulandayMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QKulandayMainWindow(QWidget *parent = 0);
    
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QTabWidget *tab_widget;
    MenuBar *menu_bar;
    
    QDir *deckpath;
    QString last_image_import_path = QDir::homePath();
    QString last_audio_import_path = QDir::homePath();
    
    int SELECT_DECK_INDEX = 0;
    int DECK_OVERVIEW_INDEX = 1;
    int NEW_DECK_INDEX = 2;
    int DECK_LEARN_INDEX = 3;
    int DECK_DIRTY_DOZEN_INDEX = 4;
    
    void deactivateDecksOverviewCloseButton();
    QMap<QString,int> deck_item_widgets; // {deck_name} OR {deck_name}_{item_id} -> tab widget id
    
    int tooltip_last_index;
    
private slots:
    void showDecksOverviewTab();
    void showLearnWidget(QString deck_name);
    void showDirtyDozenWidget(QString deck_name);
    void showDeckWidget(QString deck_name);
    void createNewDeck(QUrl deck_url);
    void createNewDeckItem(QString deck_name);
    void deleteDeck(QString deck_name);
    void showDeckItem(QString deck_name, int rowid);
    void showSearchWidget();
    void showAboutWidget();
    
    void onDeckItemContentsUpdated(QString deck_name);
    
    void activateNewTab();
    void closeTab(int tab_id);
    void onTabMoved(int from, int to);
    void onLastImageImportPathUpdated(QString last_image_import_path);
    void onLastAudioImportPathUpdated(QString last_audio_import_path);
};
