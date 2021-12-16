#include <iostream>
#include <fstream>
#include <utility>
#include <vector>


// Определяем, насколько глубоко может пойти кроулер
// При значении 15 и больше он зависает надолго, лучше ставить где-то в районе 10

// Я почти уверен, что алгоритм можно улучшить, но мне кажется, что цепочки длиной больше 10
// и так никто не требует, поэтому пойдет.
#define MAX_DEPTH_LEVEL 10


// Объявляю свои типы, чтобы было проще
using stringArray = std::vector<std::string>;
using stringDoubleArray = std::vector<std::vector<std::string>>;
//class NoWordFoundException: public std::exception {};


stringDoubleArray finalTrails;

// Проверяем, проходили ли уже это слово
bool isInExcluded(const std::string& needle, const stringArray& excluded) {
    for (const auto& word: excluded) {
        if (needle == word) {
            return true;
        }
    }

    return false;
}

void findNeighbors(const stringArray& dict, stringArray excluded, const std::string& word, const std::string& destWord, stringArray trail) {

    stringArray tmp;
    stringDoubleArray result;

    if (trail.size() >= MAX_DEPTH_LEVEL) {
        return;
    }

    // Проходимся по словарю
    for (const auto& dictWord: dict) {

        // Считаем количество отличий в двух словах
        int diffCount = 0;
        for (int i = 0; i < word.length(); ++i) {
            if (word[i] != dictWord[i]) {
                ++diffCount;
            }
        }

        // Если отличий ровно одно, значит наш клиент
        if (diffCount == 1) {

            // Проверяем, не попадали мы уже на это слово
            if (!isInExcluded(dictWord, excluded)) {
                tmp.push_back(dictWord);

                // Заодно добавляем в excluded, чтобы не попасть в рекурсию
                excluded.push_back(dictWord);
            }
        }
    }


    for (const auto& foundWord: tmp) {

        if (foundWord == destWord) {
            trail.push_back(foundWord);
            finalTrails.push_back(trail);
        }

//        for (int i = 0; i < trail.size(); ++i) {
//            std::cout << "-";
//        }
//        std::cout << foundWord << std::endl;

        auto copyTrail = trail;
        copyTrail.push_back(foundWord);

        findNeighbors(dict, excluded, foundWord, destWord, copyTrail);

    }

}

int main() {

    std::string ch = "y";


    while(ch == "y" || ch == "Y") {

        stringArray dict, result;

        std::string path, sourceWord, destWord;

        std::cout << "Word morph" << std::endl;
        std::cout << "Input a dictionary filename:" << std::endl;
        std::getline(std::cin, path);

        std::cout << "Input a source word:" << std::endl;
        std::getline(std::cin, sourceWord);

        std::cout << "Input a destination word:" << std::endl;
        std::getline(std::cin, destWord);

        std::ifstream file;
        file.open(path);

        if(file.is_open()) {

            std::string line;
            while(std::getline(file, line)) {
                if (line.length() == sourceWord.length()) {
                    dict.push_back(line);
                }
            }

            // Запускаем процесс поиска
            findNeighbors(dict, stringArray(), sourceWord, destWord, stringArray{sourceWord});

            stringArray final;

            // Путь длиной выше границы просто не может быть, поэтому мы определяем границу
            // как MAX_DEPTH_LEVEL + 1
            auto min = MAX_DEPTH_LEVEL + 1;
            for (const auto& trail: finalTrails) {
                if (trail.size() < min) {
                    final = trail;
                    min = trail.size();
                }
            }

            if (final.empty()) {
                std::cout << "Word chain could not be built" << std::endl;
            }else {
                for (int i = 0; i < final.size() - 1; ++i) {
                    std::cout << final[i] << " -> ";
                }
                std::cout << final[final.size() - 1] << std::endl;
            }

        }else {
            std::cout << "Could not open source file" << std::endl;
        }

        std::cout << std::endl << "Do you wish to proceed? [Y/N]:" << std::endl;
        std::getline(std::cin, ch);
        if (ch == "Y" || ch == "y") {
            finalTrails = stringDoubleArray();
        }
    }
}
