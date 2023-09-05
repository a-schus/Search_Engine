# Search_Engine
## Описание
Локальный поисковый движок по файлам. 

### Принципы работы поискового движка следующие:
1. В конфигурационном файле перед запуском приложения задаются названия
файлов, по которым движок будет осуществлять поиск.
2. Поисковый движок самостоятельно обходит все файлы и
индексирует их так, чтобы потом по любому поисковому запросу находить наиболее
релевантные документы.
3. Пользователь задаёт запрос через JSON-файл requests.json. Запрос — это
набор слов, по которым нужно найти документы.
4. Запрос трансформируется в список слов.
5. В индексе ищутся те документы, в которых встречаются все эти слова.
6. Результаты поиска ранжируются, сортируются и отдаются пользователю,
максимальное количество возможных документов в ответе задаётся в
конфигурационном файле.
7. В конце программа формирует файл answers.json, в который записывает
результаты поиска.

## Графический интерфейс
В дополнение к основным возможностям поискового движка добавлен графический интерфейс, позволяющий удобно изменять список файлов, пользовательские запросы и в реальном времени просматривать результаты поиска.

![Alt text](https://github.com/a-schus/Search_Engine/blob/master/images/UI.png)

Графический интерфейс создан на базе фреймворка QT.

## Инструменты

Изначально было решено, что приложение должно иметь графический интерфейс, для реализации которого был выбран фреймворк QT. 

После некоторого ознакомления с его возможностями стало очевидно, что и сам движок должен быть реализован максимально используя данные библиотеки.

В приложении вместо контейнеров стандартной библиотеки используются такие контейнеры, как QString, QVector, QMap, QPair и т.д. Многопоточность индексации реализована с помощью классов QtConcurrent, QFuture, QThread. Для отображения и редактирования данных используется концепция QT модель/представление, обеспечивающая удобное манипулирование данными и отображение их в окне интерфейса.

Для написания кода использовалась среда разработки QT Creator. Для сборки и компиляции проекта - CMake и MinGW.

## Подробно

### Устойчивость приложения

Приложение должно быть дружелюбным к пользователю, а значит должно быть устойчивым к некорректным действиям. Например, если файлы config.json и requests.json содержат неправильные запросы, или пути к файлам, не красиво было бы прерывать его выполнение. По этой причине были разработаны обработчики исключений, которые в описаных выше случаях предоставляют пользователю возможность исправить ситуацию. Если какой-либо файл для поиска не найден, он подсвечивается на экране красным цветом, и пользователь может его удалить из списка (а может и не удалять, на результаты выборки это не повлияет). Пользователь может вручную очищать списки файлов и запросов, и вводить новые данные. Для выбора новых файлов используется вызов стандартного диалога открытия файла. 

### Парсинг

Для парсинга файлов и поисковых запросов используется метод QString::split(). В роли разделителя для строк используется регулярное выражение QRegularExpression("[^0-9A-Za-zА-Яа-яЁё]"). За счет этого приложение работает как с латинскими, так и с русскими символами. 

### Многопоточность

Многопоточность индексации реализована с помощью классов QtConcurrent, QFuture, QThread. 

При разработке изначально возникла проблема: пытался записывать информацию из всех потоков в один словарь. Из-за этого скорость получилась даже меньше, чем в однопоточном варианте, так как каждый поток блокировал мьютексом этот словарь, и никакой многопоточности на практике не получалось. 

Было решено сначала генерировать отдельные словарь для всех файлов в разных потоках, затем в основном потоке сливать все словари в один. Это принесло многократное увеличение скорости индексации.

### Тестирование

Для модульного тестирования использовался фреймворк QtTest. В QT Creator был создан отдельный комплект сборки и запускался при необходимости.

## Сборка и запуск

Для сборки проекта нужно создать отдельную папку и перейти в нее в командной строке.

Затем для сборки основного приложения и модульных тестов ввести команду:

cmake -DIS_TESTS=true -G "MinGW Makefiles" "your_dir"

(Если вам необходимо собрать только основное приложение, воспользуйтесь командой
 -DIS_TESTS=false -G "MinGW Makefiles" "your_dir")

где "your_dir" - путь к папке с исходным кодом проекта.

Затем, для генерации исполняемых файлов введите:

mingw32-make

После этого в папке src\ будет находиться исполняемый файл основного приложения, а в папке test\src\ исполняемый файл модульных тестов. 

Для корректной работы приложения в папку с исполняемым файлом основного приложения необходимо скопировать папку config\ из папки с исходным кодом.

Все готово, можно запускать приложение и тесты!

### Результаты модульных тестов

![Alt text](https://github.com/a-schus/Search_Engine/blob/master/images/Untitled-1.png)