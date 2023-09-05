#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <QVector>
#include <QString>
#include <QJsonDocument>

#include <exception>



class ConverterJSON
{
    const int defaultmaxResponses = 5;
    int responsesLimit;

    QString fileName;
    QJsonDocument doc; // содержит данные из файла .json

public:
    ConverterJSON(QString jsonFileName);
    ~ConverterJSON(){}

    // 2 следующие функции возвращабт указатель чтобы избежать лишнего копирования данных
    QVector<QString>* getTextDocuments();
    static QVector<QString>* getTextDocuments(QVector<QString>& docNames); // перегруженная функция, дает возможность загружать имена файлов для поиска не из файла config.json

    int getResponsesLimit();
    void setResponsesLimit(int val);
    QVector<QString> getRequests();
    void putAnswers(QVector<QVector<QPair<int, float>>> answers);
    void getFileNames(QVector<QString>& fileNames);


private:
    QString getRequestKey(size_t requestIndex);
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
