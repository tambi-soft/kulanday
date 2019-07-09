#include "text_edit_markers.h"

TextEditMarkers::TextEditMarkers(QWidget *parent)
    : QWidget(parent)
    , markers (new MarkersTextEdit)
    , payload (new QPlainTextEdit)
    , layout (new QHBoxLayout)
{
    layout->addWidget(markers);
    layout->addWidget(payload);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    
    markers->setMaximumWidth(200);
    
    connect(payload, &QPlainTextEdit::textChanged, this, &TextEditMarkers::onPayloadTextChanged);
}

void TextEditMarkers::onPayloadTextChanged()
{
    /* detect if a marker was typed in */
    QString current_text = payload->toPlainText();
    QStringList current_text_lines = current_text.split("\n");
    int payload_lines_count = current_text_lines.count();
    
    int i = -1;
    foreach (QString line, current_text_lines) {
        i++;
        
        if (line.startsWith("\\"))
        {
            if (line.contains(" "))
            {
                /* saves the current QTextCursor for restoring the cursor line position afterwards */
                QTextCursor cursor = this->payload->textCursor();
                
                /* take action: move the marker in the markers QPlainTextEdit */
                QString marker = line.split(" ")[0];
                this->setMarker(marker, i, payload_lines_count);
                
                /* remove the marker part of the line */
                current_text_lines[i] = line.replace(marker+" ", "");
                
                QString new_payload = "";
                foreach (QString line, current_text_lines)
                {
                    new_payload += line + "\n";
                }
                
                /* the chopped is for removing the last \n */
                payload->setPlainText(new_payload.chopped(1));
                
                /* restore the previously saved text cursor */
                this->payload->setTextCursor(cursor);
                
                //qDebug() << cursor.
            }
        }
    }
    
    /* for saving the text on input in the parent class */
    emit textChanged();
}

void TextEditMarkers::setMarker(QString marker, int position, int payload_lines_count)
{
    /* make shure MarkersTextEdit has at least the same amount of lines than PayloadTextEdit */
    QString current_text = this->markers->toPlainText();
    QStringList current_text_lines = current_text.split("\n");
    QString new_text = "";
    int i = -1;
    while (i < payload_lines_count)
    {
        i++;
        
        if (i == position)
        /* write the new marker in the correct line */
        {
            new_text += marker + "\n";
        }
        else if (i < current_text_lines.count())
        {
            new_text += current_text_lines[i] + "\n";
        }
        else
        {
            new_text += "\n";
        }
    }
    
    /* chopped for removing last \n */
    markers->setPlainText(new_text.chopped(1));
}


MarkersTextEdit::MarkersTextEdit(QWidget *parent) : QTextEdit(parent)
{
    setReadOnly(true);
}


PayloadTextEdit::PayloadTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

