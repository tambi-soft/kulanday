#include "resizing_text_edit.h"

ResizingTextEdit::ResizingTextEdit(QTextEdit *parent) : QTextEdit(parent)
{
    QSizePolicy size_policy = this->sizePolicy();
    size_policy.setHeightForWidth(true);
    //size_policy.setVerticalPolicy(QSizePolicy::Preferred);
    this->setSizePolicy(size_policy);
    
    connect(this, &ResizingTextEdit::textChanged, this, &ResizingTextEdit::updateGeometry);
}

/*
 * sets minimum widget height to a value corresponding to specified number of lines in the default font
 */
/*
void ResizingTextEdit::setMinimumLines(int num_lines)
{
    //this->setMinimumSize(this->minimumSize().width(), this->lineCountToWidgetHeight(num_lines));
    
    
}

bool ResizingTextEdit::hasHeightForWidth()
{
    return true;
}

int ResizingTextEdit::heightForWidth(int width)
{
    QMargins margins = this->contentsMargins();
    
    int document_width;
    if (width >= margins.left() + margins.right())
    {
        document_width = width - margins.left() - margins.right();
    }
    else
    {
        document_width = 0;
    }
    
    QTextDocument *doc = this->document()->clone();
    doc->setTextWidth(document_width);
    
    return margins.top() + doc->size().height() + margins.bottom();
}

QSize ResizingTextEdit::sizeHint()
{
    QSize original_hint = QTextEdit::sizeHint();
    QSize result(original_hint.width(), this->heightForWidth(original_hint.width()));
    
    //return result;
}
*/
/*
 * Returns the Number of pixels corresponding to the height of specified number of lines in the default font
 */
/*
int ResizingTextEdit::lineCountToWidgetHeight(int num_lines)
{
    QMargins widget_margins = this->contentsMargins();
    qreal document_margin = this->document()->documentMargin();
    QFontMetrics font_metrics = QFontMetrics(this->document()->defaultFont());
    
    qDebug() << font_metrics.height();
    
    int result = widget_margins.top() +
            document_margin +
            std::max(num_lines, 1) * font_metrics.height() +
            this->document()->documentMargin() +
            widget_margins.bottom();
    
    //return result;
}

void ResizingTextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);
}
*/
