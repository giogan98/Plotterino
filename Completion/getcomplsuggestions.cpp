#include "getcomplsuggestions.h"

#include<QDebug>

GetComplSuggestions::GetComplSuggestions()
{

}
//---------------------------------------------------------------------------------
void GetComplSuggestions::getCompleted(QLineEdit *lineEd, QStringList list)
{
    if ( list.length() != 0 )
    {
        QCompleter *completer = new QCompleter(list);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        //completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        lineEd->setCompleter(completer);
    }
}
//---------------------------------------------------------------------------------
