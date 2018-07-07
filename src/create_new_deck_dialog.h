#ifndef Q_CREATE_NEW_DECK_WIDGET_H
#define Q_CREATE_NEW_DECK_WIDGET_H

#include <QDialog>
#include <QDir>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRadioButton>
#include <QTextEdit>

class QCreateNewDeckDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QCreateNewDeckDialog(QString default_decks_path, QDialog *parent = nullptr);
    
    static QString getExistingDirectory(QString default_decks_path);
    
private:
    QGridLayout *grid;
    QComboBox *combo_prefixes;
    QLineEdit *lineedit_new_deck_name_a;
    QLineEdit *lineedit_new_deck_name_b;
    QLineEdit *lineedit_new_prefix;
    QString default_decks_path;
    QRadioButton *radio_prefix_existing;
    QRadioButton *radio_prefix_new;
    
signals:
    
public slots:
    void cancel();
    void create();
    void onRadioExisting();
    void onRadioNew();
};

#endif // Q_CREATE_NEW_DECK_WIDGET_H
