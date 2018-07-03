#ifndef EXPORT_DECKS_DIALOG_H
#define EXPORT_DECKS_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QDir>
#include <QGridLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

#include "compress_folder.h"

class ExportDecksDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExportDecksDialog(QDir *decks_path, QDialog *parent = nullptr);
    
private:
    QDir *decks_path;
    QVBoxLayout *scroll_layout;
    QLineEdit *export_path_edit;
    
signals:
    
public slots:
    
private slots:
    void onExportButton();
    void onCancelButton();
    void onChangePathButton();
};

#endif // EXPORT_DECKS_DIALOG_H
