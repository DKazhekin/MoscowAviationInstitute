#include "nb_helper.h"

int main(int argc, char *argv[]) {
    // Обработка аргументов
    parse info;
    parse_flags(argc, argv, info);

    // Выбор режима
    if (info.action == "learn") {
        // Открываем файл с обучающими данными
        ifstream file(info.input_file);
        if (!file.is_open()) {
            cout << "Error, while opening file" << endl;
            return 1;
        }
        // Обрабатываем файл, заполняем и считаем статистики
        unordered_map<string, unordered_map<string, int>> words_related_2_tag;
        unordered_map<string, int> docs_amount_per_tag;
        processing_file(words_related_2_tag, docs_amount_per_tag, file);

        // Сериализуем статистики и заполняем файл
        serialize_send(words_related_2_tag, docs_amount_per_tag, info);


    } else if (info.action == "classify") {

        // Открываем файл с обучающими данными
        ifstream file(info.input_file);
        ifstream stats(info.stats_file);
        if (!file.is_open() || !stats.is_open()) {
            cout << "Error, while opening file" << endl;
            return 1;
        }

        unordered_map<string, unordered_map<string, int>> words_related_2_tag;
        unordered_map<string, int> docs_amount_per_tag;

        // Десереализуем данные из JSON
        deserialize(words_related_2_tag, docs_amount_per_tag, info);

        // Помощь к поиску безусловной вероятности на словах
        unordered_map<string, int> word_counter;
        double data_power = fill_dict(words_related_2_tag, word_counter); // Все слова

        // Вычисляем вероятности на каждый тэг
        vector<pair<string, double>> tags;
        evaluate(words_related_2_tag, docs_amount_per_tag, file, word_counter, data_power, info);
    }
}