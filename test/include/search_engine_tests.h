#ifndef SEARCH_ENGINE_TESTS_H
#define SEARCH_ENGINE_TESTS_H

#include <QObject>
#include <QtTest/QTest>

#include "../include/converterjson.h"
#include "../include/invertedindex.h"
#include "../include/searchserver.h"

class Search_Engine_Tests : public QObject
{
    Q_OBJECT

private:
    // ----- For indexingDocumentBENCHMARK -----//
//    QVector<QString> docs;
//    void fillDocsForBENCHMARKS();
    // ----- END ----- //

    // ----- For TestInvertedIndexFunctionality -----//
    const QVector<QString> testFunctionalityDocs_1 = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const QVector<QString> testFunctionalityDocs_2 = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"
    };
    const QVector<QString> testFunctionalityDocs_3 = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const QVector<QString> testFunctionalityRequests_1 = {"london", "the"};
    const QVector<QString> testFunctionalityRequests_2 = {"milk", "water", "cappuchino"};
    const QVector<QString> testFunctionalityRequests_3 = {"m", "statement"};
    const QVector<QVector<Entry>> testFunctionalityExpected_1 = {
        { {0, 1} },
        { {0, 1}, {1, 3} }
    };
    const QVector<QVector<Entry>> testFunctionalityExpected_2 = {
        { {0, 4}, {1, 1}, {2, 5} },
        { {0, 3}, {1, 2}, {2, 5} },
        { {3, 1} }
    };
    const QVector<QVector<Entry>> testFunctionalityExpected_3 = {
        {},
        { {1, 1} }
    };
    // ----- END ----- //

private slots:
    void initTestCase();

    // ----- InvertedIndex TESTS ----- //
//    void InvertedIndex_IndexingDocumentBENCHMARK();
//    void InvertedIndex_IndexingDocumentSingleThreadBENCHMARK();
    void TestInvertedIndexFunctionality_1();
    void TestInvertedIndexFunctionality_2();
    void TestInvertedIndexFunctionality_3();
    // ----- END ----- //

    // ----- SearchServer TESTS ----- //
    void TEST_SearchServer_sortWordsByFreq();
    void TEST_SearchServer_search_Simple();
    void TEST_SearchServer_search_Top5();
    // ----- END ----- //
};

#endif // SEARCH_ENGINE_TESTS_H
