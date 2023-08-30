#include "mainwindow.h"
//#include "converterjson.h"
//#include "invertedindex.h"
//#include "searchengineexeptions.h"
//#include "searchserver.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon.svg"));

    MainWindow w;
    w.show();



//    SearchServer search(index);

//    ConverterJSON answers(JSONConsts::answersFilePath);
//    answers.putAnswers(search.search(requests, configData->getResponsesLimit()));

//    delete configData;
    return a.exec();
}
