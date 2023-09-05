#include "../include/searchserver.h"

QVector<QVector<QPair<int, float>>> SearchServer::search(const QVector<QString> &queries_input, size_t numberOfAnswers)
{
    QVector<QVector<QPair<int, float>>> result; // int - docId, float - rank

    for (auto request : queries_input) {
        QVector<QPair<int, float>> rank = calculateRequestRank(request, numberOfAnswers);
        result.push_back(rank);
    }
    return result;
}

int SearchServer::wordFreq(const QString &word)
{
    QVector<Entry> wordIndex = _index.getWordCount(word);
    int freq = 0;
    for (auto entry : wordIndex) {
        freq += entry.count;
    }
    return freq;
}

void SearchServer::sortWordsByFreq(QStringList &words)
{
    // страшно не нравится, но учитывая ничтожно малый объем слов в запросе
    // решено пока оставить так, будем считать это техническим долгом
    for (int i = 0; i < words.size() - 1; ++i){
        for (int j = 0; j < words.size() - 1; ++j) {
            if (wordFreq(words[j]) > wordFreq(words[j + 1])) {
                std::swap(words[j], words[j + 1]);
            }
        }
    }
}

QVector<QPair<int, float> > SearchServer::calculateRequestRank(QString &request, size_t numberOfAnswers)
{
    QStringList words = request.split(_index.splitter, Qt::SkipEmptyParts);
    sortWordsByFreq(words);

    // Создадим map: key - docId, value - вектор, содержаший количества повторений
    // каждого слова из данного запроса (индекс в векторе соответствует индексу слова
    // в QStringList words
    int size = words.size();
    QVector<size_t> value(size); // начальное значение для инициализации элемента в map wordsFreqByDocs
    QMap<size_t, QVector<size_t>> wordsFreqByDocs; // key - docId, value - vector of word freq.
    for (int i = 0; i < words.size(); ++i) {
        QVector<Entry> wordCount = _index.getWordCount(words[i]);
        for (auto val : wordCount){
            if (!wordsFreqByDocs.contains(val.docId)) {
                wordsFreqByDocs.insert(val.docId, value);
            }
            wordsFreqByDocs[val.docId][i] += val.count;
        }
    }

    // Вычисляем абсолютную релевантность для каждого документа
    QVector<QPair<size_t, size_t>> ranks;
    size_t maxRank = 0;
    for (auto i = wordsFreqByDocs.begin(); i != wordsFreqByDocs.end(); ++i) {
        size_t docId = i.key();
        size_t docRank = 0;
        for (auto count : qAsConst(i.value())) {
            docRank += count;
        }
        if (docRank > maxRank) maxRank = docRank;
        ranks.push_back(QPair(docId, docRank));
    }

    // Вычисляем относительную релевантность для каждого документа
    // помещаем в результирующий вектор
    QVector<QPair<int, float> > result;
    for (auto i = 0; i < ranks.size(); ++i) {
        result.push_back(qMakePair(ranks[i].first, (float)ranks[i].second / maxRank));
    }

    return getTopAnswers(result, numberOfAnswers);
}

QVector<QPair<int, float> > SearchServer::getTopAnswers(const QVector<QPair<int, float> > &answers, size_t count)
{
    if (count > answers.size() || count == 0) {
        count = answers.size();
    }

    // Из входящего вектора вытащим только count самых релевантных ответов,
    // остальные отбрасываем
    QVector<QPair<int, float>> result(count);
    for (auto answer : answers) {
        if (answer.second > result[count - 1].second){
            result[count - 1] = answer;
            sortTopAnswers(result);
        }
    }
    return result;
}

void SearchServer::sortTopAnswers(QVector<QPair<int, float>> &topAnswers)
{
    // Перемещаем последний добавленый ответ на свое место в итоговом векторе (по убыванию)
    for (int i = topAnswers.size() - 1; i > 0; --i) {
        if (topAnswers[i].second > topAnswers[i - 1].second) {
            std::swap(topAnswers[i], topAnswers[i - 1]);
        }
        else {
            break;
        }
    }
}
