#include "create_new_deck_dialog.h"

QCreateNewDeckDialog::QCreateNewDeckDialog(QString default_decks_path, QDialog *parent)
    : QDialog(parent)
    , grid (new QGridLayout)
    , combo_prefixes (new QComboBox)
    , lineedit_new_deck_name_a (new QLineEdit)
    , lineedit_new_deck_name_b (new QLineEdit)
    , lineedit_new_prefix (new QLineEdit)
{
    setLayout(grid);
    setWindowTitle("create new deck");
    
    this->default_decks_path = default_decks_path;
    
    QStringList prefixes;
    QStringList decks_names = QDir(default_decks_path).entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    foreach (QString deck_name, decks_names)
    {
        QString current_prefix = deck_name.split("_")[0];
        
        if (! prefixes.contains(current_prefix))
        {
            prefixes.append(current_prefix);
        }
    }
    this->combo_prefixes->addItems(prefixes);
    
    this->radio_prefix_existing = new QRadioButton("choose an existing prefix:");
    this->radio_prefix_new = new QRadioButton("introduce a new prefix:");
    
    connect(radio_prefix_existing, &QRadioButton::clicked, this, &QCreateNewDeckDialog::onRadioExisting);
    connect(radio_prefix_new, &QRadioButton::clicked, this, &QCreateNewDeckDialog::onRadioNew);
    
    QPushButton *button_cancel = new QPushButton("cancel");
    QPushButton *button_create = new QPushButton("create deck");
    
    connect(button_cancel, &QPushButton::clicked, this, &QCreateNewDeckDialog::cancel);
    connect(button_create, &QPushButton::clicked, this, &QCreateNewDeckDialog::create);
    
    QTextEdit *help = new QTextEdit();
    help->setReadOnly(true);
    help->setText("<h5>Concerning Prefixes</h5>The Prefix is meant for grouping / filtering decks. For example you can take a language code as a prefix (e.g. \"de\" or \"farsi\").");
    
    this->lineedit_new_deck_name_a->setPlaceholderText("type here the name of the new deck");
    this->lineedit_new_deck_name_b->setPlaceholderText("type here the name of the new deck");
    this->lineedit_new_deck_name_a->setMinimumWidth(300);
    this->lineedit_new_prefix->setPlaceholderText("type here the prefix");
    this->lineedit_new_prefix->setMinimumWidth(140);
    
    this->grid->addWidget(radio_prefix_existing, 0, 0);
    this->grid->addWidget(this->combo_prefixes, 0, 1);
    this->grid->addWidget(new QLabel("_"), 0, 2);
    this->grid->addWidget(this->lineedit_new_deck_name_a, 0, 3);
    
    this->grid->addWidget(radio_prefix_new, 1, 0);
    this->grid->addWidget(lineedit_new_prefix, 1, 1);
    this->grid->addWidget(new QLabel("_"), 1, 2);
    this->grid->addWidget(lineedit_new_deck_name_b, 1, 3);
    
    this->grid->addWidget(button_create, 2, 0);
    this->grid->addWidget(button_cancel, 2, 3);
    
    this->grid->addWidget(help, 3, 0, 1, 4);
    
    if (prefixes.length() <= 0)
    {
        radio_prefix_new->setChecked(true);
        onRadioNew();
    }
    else
    {
        radio_prefix_existing->setChecked(true);
        onRadioExisting();
    }
}

void QCreateNewDeckDialog::cancel()
{
    this->close();
}

void QCreateNewDeckDialog::create()
{
    QString new_deck_path = "";
    if (radio_prefix_existing->isChecked())
    {
        new_deck_path = this->default_decks_path + "/" + this->combo_prefixes->currentText() + "_" + this->lineedit_new_deck_name_a->text();
    }
    else if (radio_prefix_new->isChecked())
    {
        new_deck_path = this->default_decks_path + "/" + this->lineedit_new_prefix->text() + "_" + this->lineedit_new_deck_name_b->text();
    }
    
    if (QDir(new_deck_path).exists())
    {
        QMessageBox message;
        message.setText("A deck with the name \"" + this->combo_prefixes->currentText() + "_" + this->lineedit_new_deck_name_a->text() + "\" already exists. Please choose another name.");
        message.exec();
    }
    else if (this->lineedit_new_deck_name_a->text() == "" && this->lineedit_new_deck_name_b->text() == "")
    {
        QMessageBox message;
        message.setText("The deck name must not be empty");
        message.exec();
    }
    else if (this->radio_prefix_new->isChecked() && this->lineedit_new_prefix->text() == "")
    {
        QMessageBox message;
        message.setText("The prefix must not be empty");
        message.exec();
    }
    else if (this->lineedit_new_prefix->text().contains("_"))
    {
        QMessageBox message;
        message.setText("the prefix must not contain an underscore \"_\"");
        message.exec();
    }
    else
    {
        QDir *dir = new QDir();
        dir->mkdir(new_deck_path);
        
        QDialog::accept();
    }
}

QString QCreateNewDeckDialog::getExistingDirectory(QString default_decks_path)
{
    QCreateNewDeckDialog dialog(default_decks_path);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (dialog.radio_prefix_existing->isChecked())
        {
            return default_decks_path + "/" + dialog.combo_prefixes->currentText() + "_" + dialog.lineedit_new_deck_name_a->text();
        }
        else if (dialog.radio_prefix_new->isChecked())
        {
            return default_decks_path + "/" + dialog.lineedit_new_prefix->text() + "_" + dialog.lineedit_new_deck_name_b->text();
        }
    }
    
    return QString();
}

void QCreateNewDeckDialog::onRadioExisting()
{
    this->combo_prefixes->setEnabled(true);
    this->lineedit_new_deck_name_a->setEnabled(true);
    this->lineedit_new_deck_name_b->setEnabled(false);
    this->lineedit_new_prefix->setEnabled(false);
}

void QCreateNewDeckDialog::onRadioNew()
{
    this->combo_prefixes->setEnabled(false);
    this->lineedit_new_deck_name_a->setEnabled(false);
    this->lineedit_new_deck_name_b->setEnabled(true);
    this->lineedit_new_prefix->setEnabled(true);
}
