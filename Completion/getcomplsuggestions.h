#ifndef GETCOMPLSUGGESTIONS_H
#define GETCOMPLSUGGESTIONS_H
#include <QCompleter>
#include <QLineEdit>


class getComplSuggestions
{

public:
    getComplSuggestions();
    void setStrList(QStringList list);
    void getCompleted(QLineEdit *lineEd);

private:
    QStringList strList;


};

#endif // GETCOMPLSUGGESTIONS_H
