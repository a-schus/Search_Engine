#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "converterjson.h"
#include "invertedindex.h"
#include "listmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

//    struct AnswersModelSourse {
//        size_t id;
//        QString text;
//        float fileRank;
//        size_t parentId;
//    };

    QStandardItemModel *getAnswers();
    void updateAnswers();
    void updateDocumentsBase();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_addRequestButton_clicked();

    void on_removeRequestButton_clicked();

    void on_clearRequestsButton_clicked();

    void on_actionExit_triggered();

    void on_addFileButton_clicked();

    void on_deleteButton_clicked();

    void on_clearButton_clicked();

    void on_responsesLimitSpinBox_valueChanged(int arg1);

    void on_requestLineEdit_editingFinished();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    QString currentDir;

    ConverterJSON* configData = nullptr; // содержимое файла config.json
    ConverterJSON* requestsData = nullptr; // содержимое файла requests.json

    QVector<QString>* docs = nullptr; // содержимое файлов, по которым будем искать
    QVector<QString> docNames; // имена файлов (индекс в векторе соответствует docId)
    FileListModel* docNamesModel = nullptr;
    QVector<QString> requests; // поисковые запросы
    ListModel* requestsModel = nullptr;
    QStandardItemModel *answersModel = nullptr;
    InvertedIndex index;
    QVector<QVector<QPair<int, float>>> answers;

//    QVector<AnswersModelSourse> answersModelSourse;
};
#endif // MAINWINDOW_H
