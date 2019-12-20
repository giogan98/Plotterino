#ifndef GETCOMPLSUGGESTIONS_H
#define GETCOMPLSUGGESTIONS_H

#include <QCompleter>
#include <QLineEdit>

class GetComplSuggestions
{
public:
    GetComplSuggestions();
    void setStrList(QStringList list);
    void getCompleted(QLineEdit *lineEd);

private:
    QStringList strList;
};
extern GetComplSuggestions complSuggestions;
#endif // GETCOMPLSUGGESTIONS_H
