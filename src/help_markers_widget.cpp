#include "help_markers_widget.h"

HelpMarkersWidget::HelpMarkersWidget(QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , show_text (new QTextEdit)
{
    setLayout(layout);
    layout->addWidget(show_text);
    
    
    QFile file(":markers");
    
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
    
    show_text->setHtml(lines);
    
    show_text->setReadOnly(true);
}
