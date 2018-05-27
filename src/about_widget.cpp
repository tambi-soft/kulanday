#include "about_widget.h"

AboutWidget::AboutWidget(QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , tab_widget (new QTabWidget)
{
    setLayout(layout);
    layout->addWidget(tab_widget);
    
    showAbout();
    showLibraries();
    //showAuthors();
    //showThanks();
    showLicense();
}

void AboutWidget::showAbout()
{
    QTextEdit *text = new QTextEdit();
    text->setText("Kulanday - Elearning / Flashcard / Dictionary Application\n\n\
    (c) 2018-2018 The Kulanday Author(s)\n\n\
    http://tambi-soft.github.io");
    
    this->tab_widget->addTab(text, "About");
}

void AboutWidget::showLibraries()
{
    QTextEdit *text = new QTextEdit();
    text->setText("Qt (5.11.0)");
    
    this->tab_widget->addTab(text, "Libraries");
}

void AboutWidget::showAuthors()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    this->tab_widget->addTab(text, "Authors");
}

void AboutWidget::showThanks()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    this->tab_widget->addTab(text, "Thanks To");
}

void AboutWidget::showLicense()
{
    QFile file(":gpl");
    
    QString lines;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            lines.append(stream.readLine() + "\n");
        }
    }
    file.close();
    
    QTextEdit *text = new QTextEdit();
    text->setText(lines);
    
    this->tab_widget->addTab(text, "License");
}
