#ifndef DECK_ITEM_MARKERS_WIDGET_H
#define DECK_ITEM_MARKERS_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QDir>
#include <QSize>
#include <QSysInfo>
#include <QSvgWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFont>

#include "db_adapter.h"
#include "audio_list_table.h"
#include "text_edit_markers.h"
#include "unicode_fonts.h"

class QDeckItemMarkersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckItemMarkersWidget(QDir *decks_path, QString deck_name, QString last_image_import_path, QString last_audio_import_path, QWidget *parent = nullptr);
    explicit QDeckItemMarkersWidget(QDir *decks_path, QString deck_name, int rowid, QString last_image_import_path, QString last_audio_import_path, QWidget *parent = nullptr);
    
    int rowid;
    
private:
    QDir *decks_path;
    //QString last_image_import_path = QDir::homePath();
    QString last_image_import_path;
    QString last_audio_import_path;
    QString deck_name;
    QString image_path;
    
    QGridLayout *grid;
    QLabel *image_view;
    TextEditMarkers *text_edit_markers;
    QAudioListTable *audio_list_widget;
    DbAdapter *database = nullptr;
    QPushButton *import_image_button;
    QPushButton *delete_image_button;
    
    bool ignore_item_changes;
    bool item_changed = false;
    
    QUrl default_import_path = QDir::homePath();
    
    QSize IMAGE_SIZE = QSize(300, 150); // 600 300
    
    void populateGui(QDir *decks_path, QString deck_name, int rowid);
    void initializeGui(QString deck_name, int rowid);
    
    QString randomword(int length);
    
    UnicodeFonts *unicodeFonts;
    
protected:
    void hideEvent(QHideEvent *event);
    
signals:
    void contentsUpdated(QString deck_name);
    void imageImportPathUpdated(QString last_image_import_path);
    void audioImportPathUpdated(QString last_audio_import_path);
    
public slots:
    void importImageClicked();
    void deleteImageClicked();
    
    void newAudioButtonClicked();
    void onItemChanged();
    
private slots:
    void onAudioListChanged();
};

#endif // DECK_ITEM_MARKERS_WIDGET_H
