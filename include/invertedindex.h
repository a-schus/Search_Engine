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
    // Содержимое текстовых документов для поиска
    QVector<QString> docs;

    // Содержит количесто повторений слов в каждом документе для поиска
    QMap<QString, QVector<Entry>> freqDictionary;

public:
    // регулярное выражение для разделения строки QString на отдельные слова
    QRegularExpression splitter = QRegularExpression("[^0-9A-Za-zА-Яа-яЁё]");

    InvertedIndex();

    // Одтельный метод для инициализации docs для более удобного использования
    // чтобы можно было задавать свой список файлтов
    void setDocs(const QVector<QString>& inputDocs);

    // Формируем freqDictionary в многопоточном режиме
    void UpdateDocumentBase();

    // Формируем freqDictionary в однопоточном режиме. Использовалось для сравнения скорости выполнения
    void UpdateDocumentBaseSingleThread();

    // Возвращает количество повторений определенного слова в каждом документе
    QVector<Entry> getWordCount(const QString& word);

    // Дает возможность установить пользовательский разделитель для парсинга документов
    void setSplitter(const QRegularExpression& splitterRegExp);

private:
    void indexingDocumentSingleThread(const size_t docId); // для сравнения времени выполнения многопоточного и однопоточного вариантов

    // Создает freqDictionary для одного конкретного документа и возвращает указатель на него
    QMap<QString, Entry>* indexingDocument(const QString& doc, const size_t docId);

    // Объединяет все freqDictionary, полученные с помощью
    // QMap<QString, Entry>* indexingDocument(const QString& doc, const size_t docId)
    void mergeFreqDictionarys(const QMap<QString, Entry> *docFreqDictionary);
};

#endif // INVERTEDINDEX_H
