#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include <QVector>
#include <QString>

#include "invertedindex.h"



class Search_Engine_Tests;

class SearchServer
{
    // Даем тестам доступ к приватным полям и методам класса
    friend class Search_Engine_Tests;

    // Содержит экземпляр класса InvertedIndex, в котором будет производиться поиск
    InvertedIndex _index;

public:
    SearchServer(InvertedIndex& idx) : _index(idx) {}

    // Возвращает вектор с результатами поиска по каждому запросу
    QVector<QVector<QPair<int, float>>> search(const QVector<QString>& queries_input,
                                               size_t numberOfAnswers = 0); // если numberOfAnswers == 0 то вернет все ответы

private:
    // Возвращает количество повторений слова по всем документам (для дальнейшей сортировки слов)
    int wordFreq(const QString& word);

    // Сортирует слова в запросе по неубыванию количества повторений в документах
    void sortWordsByFreq(QStringList& words);

    // Рассчитывает релевантность документов для конкретного запроса
    QVector<QPair<int, float>> calculateRequestRank(QString& request, size_t numberOfAnswers);

    // Выбирает count самых релевантных ответов для каждого запроса
    QVector<QPair<int, float>> getTopAnswers(const QVector<QPair<int, float>>& answers,
                                             size_t count);

    // Используется в QVector<QPair<int, float>> getTopAnswers(const QVector<QPair<int, float>>& answers, size_t count)
    // для упорядочивания выбранных ответов
    void sortTopAnswers(QVector<QPair<int, float>> &topAnswers);
};

#endif // SEARCHSERVER_H
