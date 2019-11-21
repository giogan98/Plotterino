#include "getcomplsuggestions.h"

getComplSuggestions::getComplSuggestions()
{

}

void getComplSuggestions::setStrList(QStringList list)
{
    for (int ii = 0; ii<list.length(); ii++){
        strList.append(list[ii]);
    }
}

void getComplSuggestions::getCompleted(QLineEdit *lineEd)
{
    QCompleter *completer = new QCompleter(strList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    //completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    lineEd->setCompleter(completer);
}

