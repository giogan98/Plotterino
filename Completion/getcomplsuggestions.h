#ifndef GETCOMPLSUGGESTIONS_H
#define GETCOMPLSUGGESTIONS_H

#include <QCompleter>
#include <QLineEdit>

class GetComplSuggestions
{
public:

private:

public:
    GetComplSuggestions();
    void getCompleted(QLineEdit *lineEd, QStringList list);

private:

};
extern GetComplSuggestions complSuggestions;
#endif // GETCOMPLSUGGESTIONS_H
