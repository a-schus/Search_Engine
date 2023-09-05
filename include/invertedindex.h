#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QThread>

struct Entry {
    size_t docId;
    size_t count;

    bool operator==(const Entry& other) const;
};


class InvertedIndex
{
    QVector<QString> docs;
    QMap<QString, QVector<Entry>> freqDictionary;

public:
    QRegularExpression splitter = QRegularExpression("[^0-9A-Za-zА-Яа-яЁё]"); // регулярное выражение для разделения строки QString на отдельные слова

    InvertedIndex();

    void setDocs(const QVector<QString>& inputDocs);// одтельный метод для инициализации docs
                                                    // для более удобного использования (чтобы можно было
                                                    // изменять список файлтов в процессе выполнения приложения
    void UpdateDocumentBase();
    void UpdateDocumentBaseSingleThread();
    QVector<Entry> getWordCount(const QString& word);
    void setSplitter(const QRegularExpression& splitterRegExp);

private:
    void indexingDocumentSingleThread(const size_t docId); // для сравнения времени выполнения многопоточного и однопоточного вариантов

    QMap<QString, Entry>* indexingDocument(const QString& doc, const size_t docId); // возвращаем указатель, чтобы избавиться от копирования больших объемов данных
    void mergeFreqDictionarys(const QMap<QString, Entry> *docFreqDictionary);
};

#endif // INVERTEDINDEX_H
