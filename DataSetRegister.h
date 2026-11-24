#pragma once

#include "HashTypes.h"

#include <stack>
#include <string>
#include <vector>

// Функция для создания тестовых наборов данных
inline std::stack<DataSet> GenerateDataSets()
{
    std::stack<DataSet> dataSets;

    // Набор №1: короткие строки
    DataSet shortStrings;
    shortStrings.description = "Короткие строки (100 элементов)";

    for (int i = 0; i < 100; i++)
    {
        std::string text = "key_" + std::to_string(i);
        shortStrings.data.push_back(TextBytes(text));
    }

    dataSets.push(shortStrings);


    // Набор №2: длинные строки
    DataSet longStrings;
    longStrings.description = "Длинные строки (1000 элементов)";

    for (int i = 0; i < 1000; i++)
    {
        std::string text = std::string(100, 'a') + std::to_string(i);
        longStrings.data.push_back(TextBytes(text));
    }

    dataSets.push(longStrings);


    // Набор №3: повторяющиеся строки
    DataSet repeatedStrings;
    repeatedStrings.description = "Повторяющиеся строки (10000 элементов)";

    for (int i = 0; i < 10000; i++)
    {
        std::string text = "value_" + std::to_string(i % 100);
        repeatedStrings.data.push_back(TextBytes(text));
    }

    dataSets.push(repeatedStrings);

    return dataSets;
}
