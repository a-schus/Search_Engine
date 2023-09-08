#include "../include/converterjson.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../include/searchengineexeptions.h"


ConverterJSON::ConverterJSON(QString jsonFileName) : fileName(jsonFileName)
{
    // Инициализируем член doc в зависимости от имени файла
    if (fileName != JSONConsts::answersFilePath){
        if (!QFile::exists(fileName)) {
            // если нужный файл не найден, выбрасываем соответствующее исключение
            if (fileName == JSONConsts::configFilePath)
                throw ConfigFileError("Файл конфигурации config.json не найден "
                                      "в корневой папке программы.\nПриложение будет закрыто.");
            if (fileName == JSONConsts::requestsFilePath)
                throw RequestsFileError("Файл с поисковыми запросами requests.json не найден "
                                        "в корневой папке программы.\n"
                                        "Добавьте поисковые запросы самостоятельно, "
                                        "нажав кнопку \"Добавить поисковый запрос\""
                                        " в главном окне приложения.");
        }
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly | QFile::Text);
        if (!file.isOpen()) {
            // если файл невозможно открыть, выбрасываем соответствующее исключение
            if (fileName == JSONConsts::configFilePath)
                throw ConfigFileError("Ошибка при открытии файла конфигурации config.json. "
                                      "Возможно он используется другой программой.\n"
                                      "Приложение будет закрыто.");
            if (fileName == JSONConsts::requestsFilePath)
                throw RequestsFileError("Ошибка при открытии файла с поисковыми запросами requests.json. "
                                        "Возможно он используется другой программой.\n"
                                        "Добавьте поисковые запросы самостоятельно, "
                                        "нажав кнопку \"Добавить поисковый запрос\""
                                        " в главном окне приложения.");
        }


        // Получаем из файла данные JSON
        doc = QJsonDocument::fromJson(QByteArray(file.readAll()));
        if (fileName == JSONConsts::configFilePath &&
            !doc.object().contains(JSONConsts::config_config))
            throw ConfigFileError("В файле конфигурации config.json нет данных.\n"
                                  "Приложение будет закрыто.");


        // Если файл "config.json", инициализируем responsesLimit
        if (fileName == JSONConsts::configFilePath){
            QJsonValue val = doc.object().value(JSONConsts::config_config).toObject().
                             value(JSONConsts::config_config_maxResponses);

            // Есле в файле данных JSON отсутствует поле max_responses,
            // установим для этого параметра некое дефолтное значение
            val == QJsonValue::Undefined ?
                responsesLimit = defaultmaxResponses : responsesLimit = val.toInt();
        }
        file.close();
    }
}

QVector<QString>* ConverterJSON::getTextDocuments()
{
    QVector<QString>* resultVec = new QVector<QString>();

    // Если файл не "config.json", возвращаем пустой вектор.
    if (fileName == JSONConsts::configFilePath){
        // Загружаем данные из файлов
        QVector<QString> fileNames;
        getFileNames(fileNames);
        for (const auto& name : qAsConst(fileNames)) {
            QString text;
            try {
                text = getTextFromFile(name);
            } catch (const FileNotFound& ex) {
                ex.showError(nullptr);
                resultVec->push_back(QString()); // чтобы если файл не найден, при выдаче результатов docid соответствовали файлу config.json
                continue;
            }
            resultVec->push_back(text);
        }
    }

    return resultVec;
}

QVector<QString> *ConverterJSON::getTextDocuments(QVector<QString>& docNames)
{
    QVector<QString>* resultVec = new QVector<QString>();
        // Загружаем данные из файлов
        for (const auto& name : docNames) {
            QString text;
            try {
                text = ConverterJSON::getTextFromFile(name);
            } catch (const FileNotFound& ex) {
                ex.showError(nullptr);
                resultVec->push_back(QString()); // чтобы при выдаче результатов docid соответствовали файлу config.json
                continue;
            }
            resultVec->push_back(text);
        }

    return resultVec;
}

int ConverterJSON::getResponsesLimit()
{
    return responsesLimit;
}

void ConverterJSON::setResponsesLimit(int val)
{
    responsesLimit = val;
}

QVector<QString> ConverterJSON::getRequests()
{
    // Если файл не "requests.json", возвращаем пустой вектор.
    QVector<QString> resultVec;
    if (fileName == JSONConsts::requestsFilePath){
        QJsonArray requestsArray = doc.object().value(JSONConsts::requests_requests).toArray();

        for (auto val : requestsArray){
            resultVec.push_back(val.toString());
        }
    }

    return resultVec;
}

void ConverterJSON::putAnswers(QVector<QVector<QPair<int, float>>> answers)
{
    if (fileName == JSONConsts::answersFilePath) {

        // формируем QJsonObject answersVal из результатов всех запросов
        QJsonObject answersVal;
        for (size_t i = 0; i < answers.size(); ++i) {
            // формируем QJsonObject request из результата очередного запроса
            QJsonObject request;
            if (answers[i].size() > 0) {
                request.insert(JSONConsts::answers_result, true);
                if (answers[i].size() > 1) {
                    // если результатов больше одного
                    QJsonArray relevances;
                    for (auto index : qAsConst(answers[i])) {
                        QJsonObject relevance;
                        relevance.insert(JSONConsts::answers_docid, index.first);
                        relevance.insert(JSONConsts::answers_rank, index.second);
                        relevances.append(relevance);
                    }
                    request.insert(JSONConsts::answers_relevance, relevances);
                } else {
                    //если результат один
                    request.insert(JSONConsts::answers_docid, answers[i][0].first);
                    request.insert(JSONConsts::answers_rank, answers[i][0].second);
                }
            } else {
                // если результатов нет
                request.insert(JSONConsts::answers_result, false);
            }

            answersVal.insert(getRequestKey(i), request);
        }

        // Создаем основной объект Json с ключом "answers"
        QJsonObject mainObject;
        mainObject.insert(JSONConsts::answers_answers, answersVal);
        doc.setObject(mainObject);

        // Открываем (или создаем) файл и записываем туда основной объект Json
        QFile file;
        file.setFileName(JSONConsts::answersFilePath);
        file.open(QIODevice::WriteOnly | QFile::Text);
        // ----- проверить создался ли файл, если нет, выбросить и обработать исключение ----- //
        file.write(doc.toJson());
        file.close();
    }
}

QString ConverterJSON::getRequestKey(size_t requestIndex)
{
    // формируем ключ "request"
    const int countOfDigits = 3; // количество цифр в ключе "request"
    QString result = QString::number(requestIndex + 1);

    for (int i = 0; i <= countOfDigits - result.length(); ++i) {
        result = '0' + result;
    }

    return (JSONConsts::answers_request + result);
}

void ConverterJSON::getFileNames(QVector<QString>& fileNames)
{
    QJsonArray filesArray = doc.object().value(JSONConsts::config_files).toArray();
    // Если список файлов отсутствует, выбрасываем исключение
    if (filesArray.isEmpty())
        throw DocumentsListNotFound("Не найден список файлов для поиска.\n"
                                    "Добавьте файлы для поиска самостоятельно "
                                    "в главном окне приложения");

    // Формируем из полученного массива вектор с именами файлов для поиска
    for (const auto& val : filesArray){
        fileNames.push_back(val.toString());
    }
}

QString ConverterJSON::getTextFromFile(const QString &name)
{
    QFile file;
    file.setFileName(name);
    file.open(QIODevice::ReadOnly | QFile::Text);

    if (!file.isOpen())
        throw FileNotFound("Файл " + name + " не может быть открыт.\n"
                           "Файл не найден, или занят другим приложением.");

    QString result = file.readAll();
    file.close();
    return result;
}
