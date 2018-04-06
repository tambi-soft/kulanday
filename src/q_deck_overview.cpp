#include "q_deck_overview.h"

QDeckOverviewWidget::QDeckOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
{
    setLayout(layout);
    
    QPushButton *deck_select_button = new QPushButton("<<<");
    QPushButton *new_item_button = new QPushButton("new item");
    
    connect (deck_select_button, &QPushButton::clicked, this, &QDeckOverviewWidget::selectDeckButtonClicked);
    connect (new_item_button, &QPushButton::clicked, this, &QDeckOverviewWidget::newItemButtonClicked);
    
    layout->addWidget(deck_select_button);
    layout->addWidget(table);
    layout->addWidget(new_item_button);
    
    initTableWidget();
}



void QDeckOverviewWidget::initTableWidget()
{
    DbAdapter *db_adapter = new DbAdapter();
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckItems();
    
    table->setColumnCount(data.at(0).count());
    table->setRowCount(data.length());
    
    int max_audio_count = db_adapter->getMaxAudioCount();
    
    for (int i = 0; i < data.length(); ++i)
    {
        QPushButton *edit_button = new QPushButton();
        edit_button->setIcon(QIcon::fromTheme("document-properties"));
        
        QPushButton *delete_button = new QPushButton();
        delete_button->setIcon(QIcon::fromTheme("edit-delete"));
        
        int rowid = data.at(i)["rowid"].toInt(); // needed for SELECTing audio files
        QString order_index = data.at(i)["order_index"].toString();
        QString name = data.at(i)["name"].toString();
        QString word = data.at(i)["word"].toString();
        QString phonetical = data.at(i)["phonetical"].toString();
        QString translation = data.at(i)["translation"].toString();
        
        QString image_filename = data.at(i)["image"].toString();
        
        QLabel *image_widget = new QLabel(this);
        if (image_filename != "")
        {
            QPixmap pixmap("/home/samuel/.tambi/decks/arab_landschaft/" + image_filename);
            pixmap = pixmap.scaled(QSize(60, 30), Qt::KeepAspectRatio);
            image_widget->setPixmap(pixmap);
        }
        
        table->setCellWidget(i, 0, edit_button);
        table->setCellWidget(i, 1, delete_button);
        
        table->setItem(i, 2, new QTableWidgetItem(order_index));
        table->setItem(i, 3, new QTableWidgetItem(name));
        table->setItem(i, 4, new QTableWidgetItem(word));
        table->setItem(i, 5, new QTableWidgetItem(phonetical));
        table->setItem(i, 6, new QTableWidgetItem(translation));
        
        table->setCellWidget(i, 8, image_widget);
        
        QList<QMap<QString,QVariant>> audio_filenames = db_adapter->audioFilenamesForDeckRowID(rowid);
        appendPlayButtons(i, audio_filenames);
    }
    
    table->resizeColumnsToContents();
}

void QDeckOverviewWidget::appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames)
{
    
}

void QDeckOverviewWidget::selectDeckButtonClicked()
{
    
}

void QDeckOverviewWidget::newItemButtonClicked()
{
    
}

void QDeckOverviewWidget::editRowButtonClicked(int rowid)
{
    
}

void QDeckOverviewWidget::deleteRowButtonClicked(int rowid)
{
    
}
