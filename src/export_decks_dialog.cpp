#include "export_decks_dialog.h"

ExportDecksDialog::ExportDecksDialog(QDir *decks_path, QDialog *parent)
    : QDialog(parent)
    , scroll_layout (new QVBoxLayout)
    , export_path_edit (new QLineEdit)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    this->decks_path = decks_path;
    
    setWindowTitle("kulanday - export decks");
    
    QGridLayout *top_layout = new QGridLayout;
    
    QWidget *scroll_widget = new QWidget;
    QScrollArea *scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(scroll_widget);
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    foreach (QString deck, decks_names)
    {
        QCheckBox *check = new QCheckBox(deck);
        this->scroll_layout->addWidget(check);
    }
    
    scroll_widget->setLayout(scroll_layout);
    
    top_layout->addWidget(scroll_area, 0, 0, 1, 2);
    
    export_path_edit->setText(QDir::homePath());
    QPushButton *change_path_button = new QPushButton("change path");
    top_layout->addWidget(export_path_edit, 1, 0);
    top_layout->addWidget(change_path_button, 1, 1);
    
    QPushButton *export_button = new QPushButton("export");
    QPushButton *cancel_button = new QPushButton("cancel");
    top_layout->addWidget(export_button, 2, 0);
    top_layout->addWidget(cancel_button, 2, 1);
    
    connect(export_button, &QPushButton::clicked, this, &ExportDecksDialog::onExportButton);
    connect(cancel_button, &QPushButton::clicked, this, &ExportDecksDialog::onCancelButton);
    connect(change_path_button, &QPushButton::clicked, this, &ExportDecksDialog::onChangePathButton);
    
    setLayout(top_layout);
}

void ExportDecksDialog::onExportButton()
{
    // get checked boxes status
    QStringList selected_decks_names;
    for (int i = 0; i < this->scroll_layout->count(); ++i)
    {
        QWidget *widget = this->scroll_layout->itemAt(i)->widget();
        QString class_name = widget->metaObject()->className();
        if (class_name == "QCheckBox")
        {
            QCheckBox *check = (QCheckBox*)widget;
            if (check->isChecked())
            {
                selected_decks_names.append(check->text());
            }
        }
    }
    
    AudioConverter *conv = new AudioConverter(this->decks_path);
    foreach (QString deck_name, selected_decks_names)
    {
        conv->convertAudioToProperOggVorbis(deck_name);
    }
    
    // export the selected stuff
    CompressFolder *cmp = new CompressFolder;
    foreach (QString module_name, selected_decks_names)
    {
        QString in_path = this->decks_path->absolutePath() + "/" + module_name;
        QString zip_path = this->export_path_edit->text() + "/" + module_name + ".kdeck";
        
        bool res = cmp->compressFolder(in_path, zip_path);
    }
    
    QMessageBox msg_box;
    msg_box.setText("Module(s) successfully exported");
    msg_box.exec();
    
    close();
}

void ExportDecksDialog::onCancelButton()
{
    close();
}

void ExportDecksDialog::onChangePathButton()
{
    QString export_path = QFileDialog::getExistingDirectory(this, tr("Select Folder for exporting Decks"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    
    this->export_path_edit->setText(export_path);
}
