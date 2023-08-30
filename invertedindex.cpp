#include "invertedindex.h"
#include <assert.h>
#include <iostream>

InvertedIndex::InvertedIndex()
{
}

void InvertedIndex::setDocs(const QVector<QString> &inputDocs)
{
    docs = inputDocs;
}

void InvertedIndex::UpdateDocumentBase()
{
    freqDictionary.clear();

    QVector<QFuture<QMap<QString, Entry>*>> futures;
    for (size_t i = 0; i < docs.size(); ++i){
        QThreadPool pool;
        futures.append(QtConcurrent::run(this,
                                         &InvertedIndex::indexingDocument,
                                         docs[i], i));
    }

    for (auto &future : futures) {
        future.waitForFinished();
    }

    for (auto &future : futures) {
        mergeFreqDictionarys(future.result());
        delete future.result(); // освобождаем память, выделенную в InvertedIndex::indexingDocument()
    }
}

void InvertedIndex::UpdateDocumentBaseSingleThread()
{
    freqDictionary.clear();
    for (int i = 0; i < docs.size(); ++i) {
        indexingDocumentSingleThread(i);
    }
}

QVector<Entry> InvertedIndex::getWordCount(const QString &word)
{
    return freqDictionary.value(word, QVector<Entry>());
}

void InvertedIndex::setSplitter(const QRegularExpression &splitterRegExp)
{
    splitter = splitterRegExp;
}

void InvertedIndex::indexingDocumentSingleThread(const size_t docId)
{
    QString doc(docs[docId].data());
    QStringList words = doc.split(splitter, Qt::SkipEmptyParts);

    for (auto nextWord : words) {
        nextWord = nextWord.toLower();
        if (freqDictionary.contains(nextWord)){
            size_t i = 0;
            for (; i < freqDictionary[nextWord].size(); ++i){
                assert(i < freqDictionary[nextWord].size());
                if (freqDictionary[nextWord][i].docId == docId){
                    ++freqDictionary[nextWord][i].count;
                    break;
                }
            }
            if (i == freqDictionary[nextWord].size()) {
                freqDictionary[nextWord].push_back({docId, 1});
            }
        } else {
            freqDictionary.insert(nextWord, {{docId, 1}});
        }
    }
}

QMap<QString, Entry>* InvertedIndex::indexingDocument(const QString& doc, const size_t docId)
{
    QMap<QString, Entry>* docFreqDictionary = new QMap<QString, Entry>;
    QStringList words = doc.split(splitter, Qt::SkipEmptyParts);
    for (auto nextWord : words) {
        nextWord = nextWord.toLower();
        if (docFreqDictionary->contains(nextWord)) {
            ++docFreqDictionary->find(nextWord)->count;
        } else {
            docFreqDictionary->insert(nextWord, {docId, 1});
        }
    }
    return docFreqDictionary;
}

void InvertedIndex::mergeFreqDictionarys(const QMap<QString, Entry>* docFreqDictionary)
{
    QMap<QString, Entry>::ConstIterator it = docFreqDictionary->begin();
    for (auto word : *docFreqDictionary) {
        freqDictionary[it.key()].push_back(word);
        ++it;
    }
}



bool Entry::operator==(const Entry &other) const
{
    return (docId == other.docId && count == other.count);
}
