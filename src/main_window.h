
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
#include "deck_item_simple_widget.h"
#include "deck_item_markers_widget.h"
#include "db_adapter.h"
#include "config.h"
#include "search_widget.h"
#include "help_about_widget.h"
#include "help_markers_widget.h"
#include "settings_widget.h"
#include "stats_widget.h"

class QKulandayMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QKulandayMainWindow(QWidget *parent = nullptr);
    
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Config *config;
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
    
    // We want to have a "deck item widget", "deck widget" and "settings widget" just open once for every deck to avoid synchronization issues. So we store them in this map to keep track of them
    QMap<QString,int> open_deck_item_widgets; // {deck_name} OR {deck_name}_{item_id} OR "settings" -> tab widget id
    
    int tooltip_last_index;
    
private slots:
    void showDecksOverviewTab();
    void showLearnWidget(QString deck_name);
    void showDirtyDozenWidget(QString deck_name);
    void showDeckWidget(QString deck_name);
    void createNewDeck(QUrl deck_url);
    void createNewDeckItem(QString deck_name);
    void deleteDeck(QString deck_name);
    void showSimpleDeckItem(QString deck_name, int rowid);
    void showMarkersDeckItem(QString deck_name, int rowid);
    void showSearchWidget();
    void showSettingsWidget();
    void showStatsWidget();
    void showHelpMarkersWidget();
    void showHelpAboutWidget();
    
    void onDeckItemContentsUpdated(QString deck_name);
    
    void activateNewTab();
    void closeTab(int tab_id);
    void onTabMoved(int from, int to);
    void onLastImageImportPathUpdated(QString last_image_import_path);
    void onLastAudioImportPathUpdated(QString last_audio_import_path);
    void synchronizeDecksOverviews();
};
