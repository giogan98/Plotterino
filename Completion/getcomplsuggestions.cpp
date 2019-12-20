#include "getcomplsuggestions.h"

GetComplSuggestions::GetComplSuggestions()
{

}
//---------------------------------------------------------------------------------
void GetComplSuggestions::setStrList(QStringList list)
{
    for (int ii = 0; ii<list.length(); ii++){
        strList.append(list[ii]);
    }
}
//---------------------------------------------------------------------------------
void GetComplSuggestions::getCompleted(QLineEdit *lineEd)
{
    QCompleter *completer = new QCompleter(strList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    //completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    lineEd->setCompleter(completer);
}
//---------------------------------------------------------------------------------
