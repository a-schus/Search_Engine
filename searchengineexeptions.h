#ifndef SEARCHENGINEEXEPTIONS_H
#define SEARCHENGINEEXEPTIONS_H

#include <exception>
#include <QMessageBox>


class ConfigFileError : public std::exception
{
    const QString message;

public:
    ConfigFileError(const QString& inMessage) : message(inMessage) {};
    void showError(QWidget* parent = nullptr) const {
        QMessageBox::critical(parent, "Ошибка", message);
    };
};

class RequestsFileError : public std::exception
{
    const QString message;

public:
    RequestsFileError(const QString& inMessage) : message(inMessage) {};
    void showError(QWidget* parent = nullptr) const {
        QMessageBox::warning(parent, "Ошибка", message);
    };
};

class DocumentsListNotFound : public std::exception
{
    const QString message;

public:
    DocumentsListNotFound(const QString& inMessage) : message(inMessage) {};
    void showError(QWidget* parent = nullptr) const {
        QMessageBox::warning(parent, "Ошибка", message);
    };
};

class FileNotFound : public std::exception
{
    const QString message;

public:
    FileNotFound(const QString& inMessage) : message(inMessage) {};
    void showError(QWidget* parent = nullptr) const {
        QMessageBox::warning(parent, "Ошибка", message);
    };
};



#endif // SEARCHENGINEEXEPTIONS_H
