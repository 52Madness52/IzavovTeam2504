#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <cctype>

#include "DataSetRegister.h"
#include "Participants.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

string ParseName(string& method_name)
{
    transform(method_name.begin(), method_name.end(), method_name.begin(),
        [](unsigned char c) { return std::tolower(c); });

    auto res = regex_replace(method_name, std::regex("_by_"), "\t");

    if (res.length() == method_name.length())
    {
        res = regex_replace(res, std::regex("by"), "\t");

        if (res.length() == method_name.length())
            res = regex_replace(res, std::regex("_"), "\t");
    }

    return res;
}

#define RUN(x) {                \
    Run(#x, x, data);           \
}

static string team = "";
static string dataset = "";
static vector<string> logs;

// Запуск алгоритма и измерение времени
void Run(string method_name, Participant p, const vector<Bytes>& data)
{
    vector<Digest> res;
    res.reserve(data.size());

    auto start = chrono::steady_clock::now();

    for (const Bytes& element : data)
    {
        res.push_back(p(element));
    }

    auto stop = chrono::steady_clock::now();

    auto time = chrono::duration_cast<chrono::microseconds>(
        stop - start
    ).count();


    ostringstream oss{};

    oss << "\"" << dataset << "\"\t"
        << data.size() << "\t"
        << team << "\t"
        << ParseName(method_name) << "\t"
        << time;

    logs.push_back(oss.str());
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    {
        cout << endl
            << "**************HASH RACE**************"
            << endl << endl;

        auto dataSets = GenerateDataSets();

        while (!dataSets.empty())
        {
            auto& ds = dataSets.top();

            dataset = ds.description;
            auto& data = ds.data;


            //------------- АЛГОРИТМЫ УЧАСТНИКОВ -------------

            // team = "IzavovTeam";
            // RUN(algorithm_by_Ivanov);
            // RUN(algorithm_by_Petrov);
            // RUN(algorithm_by_Sidorov);

            //------------------------------------------------


            dataSets.pop();
        }
    }

    {
        ofstream fout("release.txt");

        for (auto& s : logs)
            fout << s << endl;
    }

    cout << "Testing finished!" << endl;

    return 0;
}