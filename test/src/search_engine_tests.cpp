#include "../include/search_engine_tests.h"

//void Search_Engine_Tests::fillDocsForBENCHMARKS()
//{
//    QVector<QString> inputDocsNames = {"test_content/1.txt",
//                                       "test_content/2.txt",
//                                       "test_content/3.txt",
//                                       "test_content/4.txt",
//                                       "test_content/5.txt",
//                                       "test_content/6.txt",
//                                       "test_content/7.txt",
//                                       "test_content/8.txt",
//                                       "test_content/9.txt",
//                                       "test_content/10.txt"};
//    for (auto fileName : inputDocsNames) {
//        QFile file;
//        file.setFileName(fileName);
//        file.open(QIODevice::ReadOnly | QFile::Text);

//        docs.push_back(file.readAll());
//        file.close();
//    }
//}

void Search_Engine_Tests::initTestCase()
{
//    fillDocsForBENCHMARKS();
}

//void Search_Engine_Tests::InvertedIndex_IndexingDocumentBENCHMARK()
//{
//    InvertedIndex i;
//    i.setDocs(docs);
//    QBENCHMARK {
//        i.UpdateDocumentBase();
//    }
//    return;
//}

//void Search_Engine_Tests::InvertedIndex_IndexingDocumentSingleThreadBENCHMARK()
//{
//    InvertedIndex i;
//    i.setDocs(docs);
//    QBENCHMARK {
//        i.UpdateDocumentBaseSingleThread();
//    }
//}

void Search_Engine_Tests::TestInvertedIndexFunctionality_1()
{
    InvertedIndex i;

    QVector<QVector<Entry>> result;
    i.setDocs(testFunctionalityDocs_1);
    i.UpdateDocumentBase();
    for(auto& request : testFunctionalityRequests_1) {
        result.push_back(i.getWordCount(request));
    }
    QCOMPARE(result, testFunctionalityExpected_1);
}

void Search_Engine_Tests::TestInvertedIndexFunctionality_2()
{
    InvertedIndex i;

    QVector<QVector<Entry>> result;
    i.setDocs(testFunctionalityDocs_2);
    i.UpdateDocumentBase();
    for(auto& request : testFunctionalityRequests_2) {
        result.push_back(i.getWordCount(request));
    }
    QCOMPARE(result, testFunctionalityExpected_2);
}

void Search_Engine_Tests::TestInvertedIndexFunctionality_3()
{
    InvertedIndex i;

    QVector<QVector<Entry>> result;
    i.setDocs(testFunctionalityDocs_3);
    i.UpdateDocumentBase();
    for(auto& request : testFunctionalityRequests_3) {
        result.push_back(i.getWordCount(request));
    }
    QCOMPARE(result, testFunctionalityExpected_3);
}

void Search_Engine_Tests::TEST_SearchServer_sortWordsByFreq()
{
    InvertedIndex i;
    i.setDocs(testFunctionalityDocs_2);
    i.UpdateDocumentBase();

    SearchServer s(i);
    QStringList request({"milk", "water", "americano"});
    s.sortWordsByFreq(request);

    QCOMPARE(request, QStringList({"americano", "milk", "water"}));
}

void Search_Engine_Tests::TEST_SearchServer_search_Simple()
{
    const QVector<QString> request = {"milk water", "sugar"};
    const QVector<QVector<QPair<int, float>>> expected = {
        {
            {2, 1},
            {0, 0.7},
            {1, 0.3}
        },
        {}
    };

    InvertedIndex i;
    i.setDocs(testFunctionalityDocs_2);
    i.UpdateDocumentBase();

    SearchServer s(i);
    QVector<QVector<QPair<int, float>>> result = s.search(request, 0);

    ConverterJSON j(JSONConsts::answersFilePath);
    j.putAnswers(result);

    QCOMPARE(result, expected);
}

void Search_Engine_Tests::TEST_SearchServer_search_Top5()
{
    const QVector<QString> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland"
    };
    const QVector<QString> request = {"moscow is the capital of russia"};

    const QVector<QVector<QPair<int, float>>> expected = {
        {
            {7, 1},
            {14, 1},
            {0, 0.666666687},
            {1, 0.666666687},
            {2, 0.666666687}
        }
    };

    InvertedIndex i;
    i.setDocs(docs);
    i.UpdateDocumentBase();

    SearchServer s(i);
    QVector<QVector<QPair<int, float>>> result = s.search(request, 5);

    ConverterJSON j(JSONConsts::answersFilePath);
    j.putAnswers(result);

    QCOMPARE(result, expected);
}

QTEST_MAIN(Search_Engine_Tests)
