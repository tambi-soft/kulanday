
#include <src/decks_widget.h>

QDecksOverviewWidget :: QDecksOverviewWidget(Config *config, QDir *decks_path, QWidget */*parent*/)
    : combo_status_filter (new QComboBox)
{
    resize(600, 400);
    setLayout(layout);
    
    this->config = config;
    this->decks_path = decks_path;
    
    this->scroll_area->setWidgetResizable(true);
    
    this->combo_name_filter = new FilterLanguageCombo(decks_path, config);
    
    COMBO_STATI_LABELS << "[all]" << "ToDo" << "perfect" << "good" << "mediocre" << "weak" << "poor" << "none";
    
    COMBO_STATI_COLORS << QColor(Qt::white) << QColor(Qt::blue) << QColor(0, 180, 0 ) << QColor(0, 255, 0 ) << QColor(200, 255, 0 ) << QColor(Qt::yellow ) << QColor(Qt::red ) << QColor(Qt::white);
    
    this->database = new DbAdapterMeta(decks_path);
    
    connect(combo_name_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboNameFilterTextChanged);
    
    populateComboStatusFilter(combo_status_filter);
    connect(combo_status_filter, &QComboBox::currentTextChanged, this, &QDecksOverviewWidget::onComboStatusFilterTextChanged);
    
    combo_name_filter->setToolTip("filter for decks with the selected prefix");
    combo_status_filter->setToolTip("filter for decks with the selected tag");
    
    QPushButton *refresh_button = new QPushButton("refresh");
    connect(refresh_button, &QPushButton::clicked, this, &QDecksOverviewWidget::refreshTable);
    
    QPushButton *new_markers_deck_button = new QPushButton("create new markers deck");
    connect(new_markers_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewMarkersDeckClicked);
    
    QPushButton *new_simple_deck_button = new QPushButton("create new deck");
    connect(new_simple_deck_button, &QPushButton::clicked, this, &QDecksOverviewWidget::createNewSimpleDeckClicked);
    
    layout->addWidget(new QLabel("filter:"), 0, 0);
    layout->addWidget(combo_name_filter, 0, 1);
    layout->addWidget(combo_status_filter, 0, 2);
    layout->addWidget(this->scroll_area, 1, 0, 1, 3);
    //layout->addWidget(table, 1, 0, 1, 3);
    layout->addWidget(refresh_button, 2, 0);
    //layout->addWidget(new_markers_deck_button, 2, 1);
    layout->addWidget(new_simple_deck_button, 2, 2);
    layout->setRowStretch(1, 100);
    
    populateDecksOverview();
}

void QDecksOverviewWidget::createNewSimpleDeckClicked()
{
    QString default_deck_path = this->decks_path->absolutePath();
    
    QString url_path = QCreateNewDeckDialog::getExistingDirectory(default_deck_path);
    
    emit createNewDeck(QUrl(url_path));
    
    refreshTable();
}

void QDecksOverviewWidget::createNewMarkersDeckClicked()
{
    
}

void QDecksOverviewWidget::refreshTable()
{
    //table->clear();
    populateDecksOverview();
}

void QDecksOverviewWidget::populateDecksOverview()
{
    this->grid = new QGridLayout;
    this->scroll_widget = new QWidget;
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(scroll_widget);
    
    
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    this->database->populateDecksTable(decks_names);
    QMap<QString,QString> decks_meta = this->database->selectDecksStati();
    QMap<QString,QString> decks_meta_learned = this->database->selectDecksLearned();
    
    
    int i = -1;
    foreach (QString deck_name, decks_names)
    {
        if (this->combo_name_filter->currentText() == "[all]"
                || deck_name.startsWith(this->combo_name_filter->currentText()))
        {
            QString current_filter_text = this->combo_status_filter->currentText();
            
            if (current_filter_text == "[all]"
                    || current_filter_text == decks_meta[deck_name])
            {
                i++;
                
                
                QPushButton *button_dirty_dozen = new QPushButton("hear and response");
                button_dirty_dozen->setIcon(QIcon::fromTheme("image-loading"));
                connect(button_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{  tableButtonDirtyDozenClicked(deck_name); });
                button_dirty_dozen->setToolTip("dirty dozen");
                
                QPushButton *button_inv_dirty_dozen = new QPushButton("think and hear");
                button_inv_dirty_dozen->setIcon(QIcon::fromTheme("image-x-generic"));
                connect(button_inv_dirty_dozen, &QPushButton::clicked, this, [this, deck_name]{ tableButtonLearnClicked(deck_name); });
                button_inv_dirty_dozen->setToolTip("inverted dirty dozen");
                
                QPushButton *button_dirtydozen_write = new QPushButton("write");
                //TODO: add some fancy icon here
                connect(button_dirtydozen_write, &QPushButton::clicked, this, [this, deck_name]{ tableButtonDirtydozenWriteClicked(deck_name); });
                button_dirtydozen_write->setToolTip("train writing");
                
                QPushButton *button_view_deck = new QPushButton();
                button_view_deck->setIcon(QIcon::fromTheme("folder-open"));
                connect(button_view_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonViewDeckClicked(deck_name); });
                button_view_deck->setToolTip("view / edit deck");
                button_view_deck->setMaximumWidth(25);
                
                QPushButton *button_delete_deck = new QPushButton();
                button_delete_deck->setIcon(QIcon::fromTheme("edit-delete"));
                connect(button_delete_deck, &QPushButton::clicked, this, [this, deck_name]{ tableButtonDeleteDeckClicked(deck_name); });
                button_delete_deck->setToolTip("delete deck");
                button_delete_deck->setMaximumWidth(25);
                
                QComboBox *combo_status = populateComboStatus(deck_name);
                if (decks_meta[deck_name] != "")
                {
                    combo_status->setCurrentText(decks_meta[deck_name]);
                }
                
                QLabel *item_last_learned;
                if (decks_meta_learned[deck_name] == "")
                {
                    item_last_learned = new QLabel("never");
                }
                else
                {
                    QDateTime timestamp;
                    timestamp.setTime_t(decks_meta_learned[deck_name].toInt());
                    item_last_learned = new QLabel(timestamp.toString("dd. MMM yy"));
                }
                
                this->grid->addWidget(button_view_deck, i, 0);
                this->grid->addWidget(new QLabel(deck_name), i, 1);
                this->grid->addWidget(button_dirty_dozen, i, 2);
                this->grid->addWidget(button_inv_dirty_dozen, i, 3);
                this->grid->addWidget(button_dirtydozen_write, i, 4);
                this->grid->addWidget(combo_status, i, 5);
                this->grid->addWidget(item_last_learned, i, 6);
                this->grid->addWidget(button_delete_deck, i, 7);
            }
        }
    }
}

QComboBox *QDecksOverviewWidget::populateComboStatus(QString deck_name)
{
    QComboBox *combo = new QComboBox();
    combo->addItems(COMBO_STATI_LABELS.mid(1, -1));
    
    // starting at 1 because we do not want to have the first entry for "showing all" here
    for (int i=1; i < COMBO_STATI_COLORS.length(); i++)
    {
        combo->setItemData(i-1, COMBO_STATI_COLORS.at(i), Qt::BackgroundRole);
    }
    
    //combo->setStyleSheet("QFrame { border: 5px solid grey; } QComboBox { background-color: white; }");
    
    combo->setCurrentIndex(COMBO_STATI_LABELS.length()-2);
    
    connect(combo, &QComboBox::currentTextChanged, this, [this, deck_name, combo]{ onComboStatusTextChanged(deck_name, combo); });
    
    return combo;
}

void QDecksOverviewWidget::populateComboStatusFilter(QComboBox *combo)
{
    combo->addItems(COMBO_STATI_LABELS);
    
    for (int i=0; i < COMBO_STATI_COLORS.length(); i++)
    {
        combo->setItemData(i, COMBO_STATI_COLORS.at(i), Qt::BackgroundRole);
    }
    
    //combo->setStyleSheet("QFrame { border: 5px solid grey; } QComboBox { background-color: white; }");
                                
    combo->setCurrentIndex(0);
}

void QDecksOverviewWidget::tableButtonLearnClicked(QString deck_name)
{
    this->database->updateLastLearned(deck_name);
    
    emit deckLearnClicked(deck_name);
    
    refreshTable();
}

void QDecksOverviewWidget::tableButtonDirtyDozenClicked(QString deck_name)
{
    this->database->updateLastLearned(deck_name);
    
    emit deckDirtyDozenClicked(deck_name);
    
    refreshTable();
}

void QDecksOverviewWidget::tableButtonDirtydozenWriteClicked(QString deck_name)
{
    this->database->updateLastLearned(deck_name);
    
    emit deckDirtydozenWriteClicked(deck_name);
    
    refreshTable();
}

void QDecksOverviewWidget::tableButtonViewDeckClicked(QString deck_name)
{
    emit deckViewClicked(deck_name);
}

void QDecksOverviewWidget::tableButtonDeleteDeckClicked(QString deck_name)
{
    QString message = "delete deck \"" + deck_name + "\"?";
    
    int reply = QMessageBox::question(this, "Delete", message, QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        emit deleteDeck(deck_name);
    }
    
    refreshTable();
}

void QDecksOverviewWidget::onComboNameFilterTextChanged(QString text)
{
    this->config->setLastLanguageFilter(text);
    
    refreshTable();
}

void QDecksOverviewWidget::onComboStatusFilterTextChanged(QString text)
{
    comboColorAdjust(this->combo_status_filter);
    
    refreshTable();
}

void QDecksOverviewWidget::onComboStatusTextChanged(QString deck_name, QComboBox *combo_status)
{
    this->database->updateDeckStatus(deck_name, combo_status->currentText());
    comboColorAdjust(combo_status);
}

/* 
 * Adjusts the color for the selected item to match with the color
 * defined in COMBO_STATI_COLORS 
 */
void QDecksOverviewWidget::comboColorAdjust(QComboBox *combo)
{
    // make shure the list with the stylesheets for each item is built
    if (this->style_list.length() != COMBO_STATI_COLORS.length())
    {
        comboColorAdjustBuildStylelist();
    }
    
    // select the stylesheet-entry relevant for the current item
    QString text = combo->currentText();
    for (int i=0; i < COMBO_STATI_LABELS.length(); i++)
    {
        if (text == COMBO_STATI_LABELS.at(i))
        {
            combo->setStyleSheet(this->style_list.at(i));
            break;
        }
    }
}

void QDecksOverviewWidget::comboColorAdjustBuildStylelist()
{
    for (int i=0; i < COMBO_STATI_COLORS.length(); i++)
    {
        QString r = QString::number(COMBO_STATI_COLORS.at(i).red());
        QString g = QString::number(COMBO_STATI_COLORS.at(i).green());
        QString b = QString::number(COMBO_STATI_COLORS.at(i).blue());
        
        QString style = "QFrame {"
                        "border: 5px solid grey;"
                        "}"
                        "QComboBox {"
                        "background-color: rgb("+r+","+g+","+b+");"
                        "}";
        
        this->style_list.append(style);
    }
}
