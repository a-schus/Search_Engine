#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <QVector>
#include <QString>
#include <QJsonDocument>

#include <exception>



class ConverterJSON
{
    // responsesLimit по умолчанию, если данный параметр не указан в config.json
    const int defaultmaxResponses = 5;

    // Максимальное количество выводимых ответов
    int responsesLimit;

    // Имя файла с которым будет работать экземпляр класса ConverterJSON
    QString fileName;

    // Содержимое файла fileName + ".json"
    QJsonDocument doc;

public:
    ConverterJSON(QString jsonFileName);
    ~ConverterJSON(){}

    /* Методы QVector<QString>* getTextDocuments() и
     * static QVector<QString>* getTextDocuments(QVector<QString>& docNames) формируют вектор,
     * содержащий данные из текстовых файлов, по которым будет производиться поиск. Каждый
     * элемент вектора хранит содержимое очередного файла.
     *
     * Метод QVector<QString>* getTextDocuments() берет имена файлов и файла config.json.
     * Если же требуется передать другие имена, нужно использовать перегруженный метод
     * static QVector<QString>* getTextDocuments(QVector<QString>& docNames), параметром которого
     * будет вектор, содержащий имена файлов.
     *
     * Оба метода возвращабт указатель чтобы избежать лишнего копирования данных */
    QVector<QString>* getTextDocuments();
    static QVector<QString>* getTextDocuments(QVector<QString>& docNames);

    // Возвращает значение responsesLimit
    int getResponsesLimit();

    // Устанавливает значение responsesLimit
    void setResponsesLimit(int val);

    // Возвращает вектор, содержащий текстовые запросы из файла requests.json
    QVector<QString> getRequests();

    // Записывает результаты поиска в файл
    void putAnswers(QVector<QVector<QPair<int, float>>> answers);

    // В данный метод нужно передать (по ссылке) QVector<QString>& fileNames,
    // в который будут добавлены имена файлов для поиска, содержащиеся в файле config.json
    void getFileNames(QVector<QString>& fileNames);


private:
    // Формирует ключи запросов для файла answers.json (например request001, request002 и т.д.)
    QString getRequestKey(size_t requestIndex);

    // Возвращает содержимое текстового файла для поиска
    static QString getTextFromFile (const QString& name); // static для использования в статическом методе getTextDocuments(...)
};



// Пространство имен JSONConsts создано для удобства,
// если вдруг возникнет необходимость переименовать поля в файлах *.json
namespace JSONConsts {
    const QString configFilePath = "config/config.json";
    const QString requestsFilePath = "config/requests.json";
    const QString answersFilePath = "answers.json";

    const QString config_config = "config";
    const QString config_config_name = "name";
    const QString config_config_version = "version";
    const QString config_config_maxResponses = "max_responses";
    const QString config_files = "files";

    const QString requests_requests = "requests";

    const QString answers_answers = "answers";
    const QString answers_request = "request";
    const QString answers_result = "result";
    const QString answers_relevance = "relevance";
    const QString answers_docid = "docid";
    const QString answers_rank = "rank";
}

#endif // CONVERTERJSON_H
