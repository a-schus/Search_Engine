#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "searchengineexeptions.h"
#include "searchserver.h"

#include <QFileDialog>


QStandardItemModel* MainWindow::getAnswers()
{
    // Удаляем старые данные из векторов
    answers.clear();

    // Формируем ответы
    SearchServer search(index);
    answers = search.search(requests, configData->getResponsesLimit());
    size_t i;
//    // Формируем источник для модели отображения ответов в QTreeView
//    size_t i = 1;
//    size_t requestIndex = 0;
//    AnswersModelSourse item;
//    for (auto& request : qAsConst(answers)) {
//        item.id = i;
//        item.text = requests[requestIndex];
//        item.fileRank = 0;
//        item.parentId = 0;
//        answersModelSourse.push_back(item);
//        ++i;
//        for (auto& file : request) {
//            item.id = i;
//            item.text = docNames[file.first];
//            item.fileRank = file.second;
//            item.parentId = requestIndex;
//            answersModelSourse.push_back(item);
//        }
//        ++requestIndex;
//    }

    // Заполняем ответами модель
    QStandardItemModel* ansModel = new QStandardItemModel(this);
    QStandardItem* rootItem = ansModel->invisibleRootItem();
    rootItem->setColumnCount(2);

    i = 0;
    for (auto& request : qAsConst(answers)) {
        // Заполняем родительский элемент
        QList<QStandardItem*> rows = {new QStandardItem(requests[i])};
        rootItem->appendRow(rows);

        // Заполняем вложенные элементы дерева
        for (auto& file : request) {
            QList<QStandardItem*> nextRow;
            QStandardItem* rowItem;
            rowItem = new QStandardItem(docNames.at(file.first));
            nextRow.append(rowItem);
            rowItem = new QStandardItem(' ' + QString::number(file.second));
            nextRow.append(rowItem);
            rows.first()->appendRow(nextRow);
        }
        ++i;
    }
    return ansModel;
}

void MainWindow::updateAnswers()
{
    //Получаем ответы и отображаем их в ui->answersTreeView
    answersModel = getAnswers();
    ui->answersTreeView->setModel(answersModel);
    ui->answersTreeView->expandAll();
}

void MainWindow::updateDocumentsBase()
{
    // Загружаем данные из файлов для поиска
    docs = ConverterJSON::getTextDocuments(docNames);

    // Проводим индексацию
    index.setDocs(*docs);
    index.UpdateDocumentBase();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Читаем и проверяем на корректность файл config.json, инициализируем configData
    try {
        configData = new ConverterJSON (JSONConsts::configFilePath);
    } catch (const ConfigFileError& ex) {
        ex.showError(this);
        configData = nullptr;
        QMetaObject::invokeMethod(this,"close",Qt::QueuedConnection); // прерываем выполнение программы
        // Программа завершается не сразу, а после выполнения конструктора, поэтому чтобы деструктор
        // отработал корректно, все удаляемые в нем указатели изначально инициализированы значением nullptr
    }

    // Если файл config.json открылся и содержит корректные данные, продолжаем выполнение конструктора.
    if (configData) {
        // Получаем данные из файла requests.json, инициализируем requestsData
        try {
            requestsData = new ConverterJSON(JSONConsts::requestsFilePath);
            requests = requestsData->getRequests();

        } catch (const RequestsFileError& ex) {
            ex.showError(this);
        }
        requestsModel = new ListModel(&requests, this);
        ui->requestsTableView->setModel(requestsModel);

        // устанавливаем начальное значение пути к файлам для поиска
        currentDir = QDir().currentPath();

        // Инициализируем docs данными из файлов для поиска
        try {
            configData->getFileNames(docNames);
            docs = new QVector<QString>();
            docs = configData->getTextDocuments();
        } catch (const DocumentsListNotFound& ex) {
            ex.showError(this);
        }
        docNamesModel = new FileListModel(&docNames, this);
        ui->fileNamesTableView->setModel(docNamesModel);

        // Проводим индексацию
        index.setDocs(*docs);
        index.UpdateDocumentBase();

        // Устанавливаем нужное значение спинбокса responsesLimitSpinBox
        ui->responsesLimitSpinBox->setValue(configData->getResponsesLimit());

        //Получаем ответы и отображаем их в ui->answersTreeView
        answersModel = getAnswers();
        ui->answersTreeView->setModel(answersModel);
        ui->answersTreeView->expandAll();
        ui->answersTreeView->setColumnWidth(0, 350);

        ui->answersTreeView->header()->setSectionResizeMode(0, QHeaderView::Fixed);

        // настраиваем сплитеры
        ui->splitter->setStyleSheet("QSplitter::handle "
                                    "{image: url(:/images/HorSplitter.png);}"
                                    "QSplitter::handle:horizontal "
                                    "{width: 3px;}"
                                    "QSplitter::handle:vertical "
                                    "{height: 20px;}");
        ui->splitter_2->setStyleSheet("QSplitter::handle "
                                    "{image: url(:/images/VerSplitter.png);}"
                                    "QSplitter::handle:horizontal "
                                    "{width: 20px;}"
                                    "QSplitter::handle:vertical "
                                    "{height: 3px;}");

    }
    return;
}

MainWindow::~MainWindow()
{
    delete configData;
    delete requestsData;
    delete requestsModel;
    delete docNamesModel;
    delete answersModel;
    delete docs;

    delete ui;
}


void MainWindow::on_addRequestButton_clicked()
{
    QString value = ui->requestLineEdit->text().toLower();
    if (value.isEmpty())
        return;
    ui->requestLineEdit->clear();

    int row = requestsModel->rowCount(QModelIndex());
    requestsModel->insertRow(row);
    QModelIndex index = requestsModel->index(row, 0);
    requestsModel->setData(index, value, Qt::EditRole);
    updateAnswers();
}


void MainWindow::on_removeRequestButton_clicked()
{
    int row = ui->requestsTableView->currentIndex().row();
    requestsModel->removeRow(row);
    updateAnswers();
}



void MainWindow::on_clearRequestsButton_clicked()
{
    requestsModel->clear();
    updateAnswers();
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_addFileButton_clicked()
{
    QFileDialog fileDialog(this, "Select files to add");
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setNameFilter(tr("Text (*.txt)"));
    fileDialog.setDirectory(currentDir);
    QStringList files;
    if(fileDialog.exec()) {
        files = fileDialog.selectedFiles();
        currentDir = fileDialog.directory().absolutePath();
    }

    docNamesModel->addRows(files);

    updateDocumentsBase();
    updateAnswers();
}


void MainWindow::on_deleteButton_clicked()
{
    int row = ui->fileNamesTableView->currentIndex().row();
    docNamesModel->removeRow(row);

    updateDocumentsBase();
    updateAnswers();
}


void MainWindow::on_clearButton_clicked()
{
    docNamesModel->clear();
    updateDocumentsBase();
    updateAnswers();
}


void MainWindow::on_responsesLimitSpinBox_valueChanged(int arg1)
{
    configData->setResponsesLimit(arg1);
    updateAnswers();
}


void MainWindow::on_requestLineEdit_editingFinished()
{
    emit ui->addRequestButton->clicked();
}


void MainWindow::on_actionAbout_triggered()
{
    QDialog win(this);
    win.setWindowTitle("About");
    win.setFixedSize(QSize(200, 200));

    QVBoxLayout *layout = new QVBoxLayout(&win);
    win.setLayout(layout);

    QLabel* logo = new QLabel(&win);
    logo->setAlignment(Qt::AlignCenter);
    QPixmap image(":/images/icon.svg");
    logo->setPixmap(image);

    QLabel* appName = new QLabel(&win);
    appName->setAlignment(Qt::AlignCenter);
    appName->setText("\nSearch engine for Skillbox");

    QLabel* authorName = new QLabel(&win);
    authorName->setAlignment(Qt::AlignCenter);
    authorName->setText("by Alexander Schus");

    QLabel* ver = new QLabel(&win);
    ver->setAlignment(Qt::AlignCenter);
    ver->setText("\n\nversion 1.0");

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);
    layout->addWidget(appName);
    layout->addWidget(authorName);
    layout->addWidget(ver);

    win.exec();
}

