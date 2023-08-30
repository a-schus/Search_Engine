#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include <QVector>
#include <QString>

#include "invertedindex.h"



class Search_Engine_Tests;

class SearchServer
{
    friend class Search_Engine_Tests;

    InvertedIndex _index;

public:
    SearchServer(InvertedIndex& idx) : _index(idx) {}

    QVector<QVector<QPair<int, float>>> search(const QVector<QString>& queries_input,
                                               size_t numberOfAnswers = 0); // если numberOfAnswers == 0 то вернет все ответы

private:
    int wordFreq(const QString& word);
    void sortWordsByFreq(QStringList& words);
    QVector<QPair<int, float>> calculateRequestRank(QString& request, size_t numberOfAnswers);
    QVector<QPair<int, float>> getTopAnswers(const QVector<QPair<int, float>>& answers,
                                             size_t count);
    void sortTopAnswers(QVector<QPair<int, float>> &topAnswers);
};

#endif // SEARCHSERVER_H
