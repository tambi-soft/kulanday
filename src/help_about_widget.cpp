#include "help_about_widget.h"

HelpAboutWidget::HelpAboutWidget(QWidget *parent)
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

void HelpAboutWidget::showAbout()
{
    QTextEdit *text = new QTextEdit();
    text->setText("Kulanday - Elearning / Flashcard / Dictionary Application\n\n\
    (c) 2018-2019 The Kulanday Author(s)\n\n\
    http://tambi-soft.github.io");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "About");
}

void HelpAboutWidget::showLibraries()
{
    QTextEdit *text = new QTextEdit();
    text->setText("Qt (5.11.0)\n\
ffmpeg\n\
ogg vorbis codec");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Libraries");
}

void HelpAboutWidget::showAuthors()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Authors");
}

void HelpAboutWidget::showThanks()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Thanks To");
}

void HelpAboutWidget::showLicense()
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
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "License");
}
