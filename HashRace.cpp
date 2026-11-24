#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <cstdint>

#include "DataSetRegister.h"
#include "Participants.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


// Название команды
static string team = "";

// Название текущего набора данных
static string dataset = "";

// Хранение результатов тестирования
static vector<string> logs;


// Макрос для запуска алгоритма
#define RUN(x) Run(#x, x, data)


// Пример алгоритма хэширования FNV-1a
Digest fnv1a_example(const Bytes& data)
{
    uint32_t hash = 2166136261u;

    for (uint8_t byte : data)
    {
        hash ^= byte;
        hash *= 16777619u;
    }

    return Digest32(hash);
}


// Функция тестирования алгоритма
void Run(string method_name, Participant p, const vector<Bytes>& data)
{
    vector<Digest> results;
    results.reserve(data.size());

    // Начинаем измерение времени
    auto start = chrono::steady_clock::now();

    // Хэшируем каждый элемент набора
    for (const Bytes& element : data)
    {
        results.push_back(p(element));
    }

    // Заканчиваем измерение времени
    auto stop = chrono::steady_clock::now();

    // Переводим время в микросекунды
    auto time = chrono::duration_cast<chrono::microseconds>(
        stop - start
    ).count();


    // Проверка результатов
    bool correct = !results.empty();

    // Количество коллизий
    size_t collisions = 0;

    // Запоминаем, какой хеш был получен для каждого входного значения
    map<Bytes, Digest> inputHashes;

    // Запоминаем, какие входные значения дали одинаковый хеш
    map<Digest, Bytes> hashInputs;

    size_t expectedLength = results.empty() ? 0 : results.front().size();

    for (size_t i = 0; i < results.size(); i++)
    {
        // Хеш не должен быть пустым.
        // Длина хеша одного алгоритма должна быть постоянной.
        if (results[i].empty() || results[i].size() != expectedLength)
        {
            correct = false;
        }

        // Проверяем, встречался ли такой вход раньше
        auto previousInput = inputHashes.find(data[i]);

        if (previousInput != inputHashes.end())
        {
            // Одинаковые данные должны давать одинаковый хеш
            if (previousInput->second != results[i])
            {
                correct = false;
            }

            continue;
        }

        inputHashes[data[i]] = results[i];

        // Проверяем, встречался ли такой хеш у другого входного значения
        auto previousHash = hashInputs.find(results[i]);

        if (previousHash != hashInputs.end())
        {
            collisions++;
        }
        else
        {
            hashInputs[results[i]] = data[i];
        }
    }

    // Дополнительная проверка повторного вызова алгоритма
    if (!data.empty() && p(data.front()) != results.front())
    {
        correct = false;
    }


    // Формируем строку с результатами
    ostringstream oss;

    oss << "\"" << dataset << "\"\t"
        << data.size() << "\t"
        << team << "\t"
        << method_name << "\t"
        << (correct ? to_string(time) : "-1") << "\t"
        << collisions << "\t"
        << (correct ? "OK" : "ERROR");

    logs.push_back(oss.str());
}


// Главная функция программы
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    cout << "************** HASH RACE **************" << endl;

    // Получаем тестовые наборы данных
    auto dataSets = GenerateDataSets();

    // Проверяем каждый набор
    while (!dataSets.empty())
    {
        auto& ds = dataSets.top();

        dataset = ds.description;

        auto& data = ds.data;

        cout << "Testing: " << dataset << endl;


        // Пример алгоритма
        team = "Example";
        RUN(fnv1a_example);


        // Сюда будем добавлять алгоритмы участников
        //
        // team = "IzavovTeam";

        // Переходим к следующему набору
        dataSets.pop();
    }


    // Сохраняем результаты в файл
    ofstream fout("release.txt");

    if (!fout.is_open())
    {
        cerr << "Cannot open release.txt" << endl;
        return 1;
    }

    fout << "Dataset\tElements\tTeam\tAlgorithm\tTime_us\tCollisions\tStatus\n";

    for (const string& s : logs)
    {
        fout << s << endl;
    }

    fout.close();

    cout << "************** FINISHED **************" << endl;
    cout << "Results saved to release.txt" << endl;

    return 0;
}