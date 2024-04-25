#include "nb_helper.h"

// Функция для парсинга флагов при запуске программы из терминала
void parse_flags(int argc, char *argv[], parse &info) {
    if (strcmp(argv[1], "learn") == 0) {
        info.action = "learn";
        for (size_t i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--input") == 0) {
                info.input_file = argv[i + 1];
            }
            if (strcmp(argv[i], "--output") == 0) {
                info.output_file = argv[i + 1];
            }
        }
    } else if (strcmp(argv[1], "classify") == 0) {
        info.action = "classify";
        for (size_t i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--input") == 0) {
                info.input_file = argv[i + 1];
            }
            if (strcmp(argv[i], "--output") == 0) {
                info.output_file = argv[i + 1];
            }
            if (strcmp(argv[i], "--stats") == 0) {
                info.stats_file = argv[i + 1];
            }
        }
    }
}

// Функция, которая считает статистики при обучении модели на тренировочных данных
void processing_file(unordered_map<string, unordered_map<string, int>> &words_related_2_tag,
                     unordered_map<string, int> &docs_amount_per_tag, ifstream &file) {
    string word, file_string;
    while (getline(file, file_string)) {
        int n = stoi(file_string);

        // Читаем теги
        getline(file, file_string);
        istringstream tags_parse(file_string);
        vector<string> tags;
        while (tags_parse >> word) {
            tags.push_back(word);
            // Считаем количество документов с конкретным тэгом
            if (docs_amount_per_tag.find(word) != docs_amount_per_tag.end()) {
                docs_amount_per_tag[word]++;
            } else {
                docs_amount_per_tag[word] = 1;
            }
        }

        // Читаем заголовок
        getline(file, file_string);
        istringstream header_parse(file_string);
        while (header_parse >> word) {
            // Обработка слова заголовка, если он не является стоп-словом
            for (const auto &tag: tags) {
                if (words_related_2_tag.find(tag) != words_related_2_tag.end()){
                    if (words_related_2_tag[tag].find(word) != words_related_2_tag[tag].end()){
                        words_related_2_tag[tag][word]++;
                    }
                    else{
                        words_related_2_tag[tag][word] = 1;
                    }
                }
                else{
                    words_related_2_tag[tag] = {};
                    words_related_2_tag[tag][word] = 1;
                }
            }

        }

        // Читаем текст вопроса
        for (size_t i = 0; i < n; i++) {
            getline(file, file_string);
            istringstream question_text(file_string);
            while (question_text >> word) {
                for (const auto &tag: tags) {
                    if (words_related_2_tag.find(tag) != words_related_2_tag.end()){
                        if (words_related_2_tag[tag].find(word) != words_related_2_tag[tag].end()){
                            words_related_2_tag[tag][word]++;
                        }
                        else{
                            words_related_2_tag[tag][word] = 1;
                        }
                    }
                    else{
                        words_related_2_tag[tag] = {};
                        words_related_2_tag[tag][word] = 1;
                    }
                }
            }
        }
    }
}

// Функция, которая сериализует статистики и сохраняет их в файл, указанный при --stats, после обучения
void serialize_send(unordered_map<string, unordered_map<string, int>> &words_related_2_tag,
                    unordered_map<string, int> &docs_amount_per_tag, const parse &info) {
    json json_words_related_2_tag;
    json json_docs_amount_per_tag;

    for (const auto &elem: words_related_2_tag) {
        json_words_related_2_tag[elem.first] = elem.second;
    }

    for (const auto &elem: docs_amount_per_tag) {
        json_docs_amount_per_tag[elem.first] = elem.second;
    }

    json json_all;
    json_all["words_related_2_tag"] = json_words_related_2_tag;
    json_all["docs_amount_per_tag"] = json_docs_amount_per_tag;

    string serialized_json = json_all.dump(4);

    ofstream file(info.output_file);
    file << serialized_json;
    file.close();
}

// Функция, с помощью которой мы десереализуем данные из формата JSON при валидации
void deserialize(unordered_map<string, unordered_map<string, int> > &words_related_2_tag,
                 unordered_map<string, int> &docs_amount_per_tag, const parse &info) {
    json json_all;

    ifstream stats(info.stats_file);
    stats >> json_all;
    stats.close();

    // Десереализуем
    if (json_all.contains("words_related_2_tag") && json_all["words_related_2_tag"].is_object()) {
        for (auto &[key, value]: json_all["words_related_2_tag"].items()) {
            unordered_map<string, int> word_freq_map = value.get<unordered_map<string, int>>();
            words_related_2_tag[key] = word_freq_map;
        }
    }

    if (json_all.contains("docs_amount_per_tag") && json_all["docs_amount_per_tag"].is_object()) {
        docs_amount_per_tag = json_all["docs_amount_per_tag"].get<unordered_map<string, int> >();
    }
}

// Функция, которой я обновляю тэги и соответствующие им вероятности на валидации для каждого документа
void get_tags(vector<pair<string, double>> &tags, unordered_map<string, int> &docs_amount_per_tag) {
    for (const auto &pair_: docs_amount_per_tag) {
        pair<string, double> tmp;
        tmp.first = pair_.first;
        tmp.second = 0;
        tags.push_back(tmp);
    }
}

// Функция считает общее число всех документов
double hashmap_sum(unordered_map<string, int> &docs_amount_per_tag) {
    int totalSum = 0;
    for (const auto &pair: docs_amount_per_tag) {
        totalSum += pair.second;
    }
    return totalSum;
}

// Функция считает уникальное число слов в обучающей выборке (используется в сглаживании Лапласа)
double count_unique_words(const unordered_map<string, unordered_map<string, int>> &words_related_2_tag) {
    unordered_set<string> unique_words;

    for (const auto &pair : words_related_2_tag) {
        for (const auto &pair2 : pair.second) {
            unique_words.insert(pair2.first);
        }
    }

    return (double)unique_words.size();
}

// Нахождение безусловной вероятности (та, что стоит в знаменателе)
double get_uncoditional_prob(unordered_map<string, int> &word_counter, const string& word, double unique_values, int data_power){
    double count1 = 0;
    if (word_counter.find(word) != word_counter.end()){
        count1 = word_counter[word];
    }
    return log((count1 + 1) / ((double)data_power + unique_values));
}

// Выдает количество раз сколько встретилось конкретное слово в документах под конкретным тэгом
double word_count(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, string tag, string word){
    double word_c = 0;
    if (words_related_2_tag[tag].find(word) != words_related_2_tag[tag].end()){
        word_c = words_related_2_tag[tag][word];
    }
    return word_c;
}

// Счетчик всех слов под конкретным тэгом
double all_word_count(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, string tag){
    double count = 0;
    for (const auto &pair: words_related_2_tag[tag]){
        count += pair.second;
    }
    return count;
}

// Функция заполнения счетчика всех слов
double fill_dict(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, unordered_map<string, int> &word_counter){
    int all_words = 0;
    for (const auto &pair1: words_related_2_tag){
        for (const auto &pair2: pair1.second){
            if (word_counter.find(pair2.first) != word_counter.end()){
                word_counter[pair2.first] += pair2.second;
            }
            else{
                word_counter[pair2.first] = pair2.second;
            }
            all_words += pair2.second;
        }
    }
    return (double)all_words;
}

// Валидационная главная функция
void evaluate(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, unordered_map<string, int> &docs_amount_per_tag, ifstream &file, unordered_map<string, int> &word_counter, double data_power, parse &info){

    ofstream output_file_(info.output_file);
    string ans, answer_string;

    // Обрабатываем все файлы и вычисляем вероятность
    string word, file_string;
    double documents_amount = hashmap_sum(docs_amount_per_tag); // All docs count
    double unique_words = count_unique_words(words_related_2_tag);

    while (getline(file, file_string)) {
        // Создаем вектор тэгов и соответствующих им 'вероятностей'
        vector<pair<string, double>> tags;
        get_tags(tags, docs_amount_per_tag);

        int n = stoi(file_string);

        for (size_t i = 0; i < n + 1; i++) {
            getline(file, file_string);
            istringstream stream(file_string);
            while (stream >> word) {
                for (auto &pair: tags) {
                    double word_c = word_count(words_related_2_tag, pair.first, word);
                    double all_words = all_word_count(words_related_2_tag, pair.first);
                    pair.second += log((word_c + 1.0) / (all_words + unique_words)) - get_uncoditional_prob(word_counter, word, unique_words, data_power);
                }

            }
        }

        for (auto &pair: tags) {
            pair.second += log((double) docs_amount_per_tag[pair.first] / documents_amount);
        }

        //Softmax
        for (auto &pair: tags) {
            double sum_ = 0;
            for (auto &pair2: tags){
                sum_ += exp(pair2.second);
            }
            pair.second = exp(pair.second) / (double) sum_;
        }

        // Sort the results
        sorting(tags);

        // Writing predicts
        inference(tags, 0.7, output_file_);
    }
    output_file_.close();


    // Calculating statistics
    double TP = 0, FP = 0, FN = 0;
    calc_statistics(info, TP, FP, FN);
    cout << "Micro-Precision: " << (TP / (TP + FP)) << endl;
    cout << "Micro-Recall: " << (TP / (TP + FN)) << endl;
}

// Сортировка массива тэгов
void sorting(vector<pair<string, double>> &tags){
    sort(tags.begin(), tags.end(), [](const pair<string, double> &a, const pair<string, double> &b) {
        return a.second > b.second;
    });
}

// Отбор нужных тэгов и запись в файл
void inference(vector<pair<string, double>> &tags, double threshold, ofstream &output_file){
    bool flag = 0;
    vector<string> inference;
    for (const auto &pair: tags){
        if (pair.second > threshold){
            inference.push_back(pair.first);
            flag = 1;
        }
    }
    if (!flag){
        inference.push_back(tags[0].first);
    }
    for (size_t i = 0; i < inference.size(); i++){
        output_file << inference[i];
        if (i < inference.size() - 1){
            output_file << " ";
        }
    }
    output_file << "\n";
}

// Подсчет метрик
void calc_statistics(parse &info, double &TP, double &FP, double &FN){
    ifstream predict(info.output_file);
    ifstream actual("/Users/deniskazhekin/DiscreteAnalysis/CourseProject/version2/materials/valid_xxl_ans.txt");
    string predict_word, predict_string;
    string actual_word, actual_string;

    while (getline(predict, predict_string) && getline(actual, actual_string)){

        istringstream predict_stream(predict_string);
        istringstream actual_stream(actual_string);


        unordered_set<string> actual_tags;
        while (actual_stream >> actual_word){
            actual_tags.insert(actual_word);
        }

        unordered_set<string> predicted_tags;
        while (predict_stream >> predict_word){
            if (actual_tags.find(predict_word) != actual_tags.end()){
                TP++;
            }
            else{
                FP++;
            }

        }

        for (const auto &item: actual_tags){
            if (predicted_tags.find(item) == predicted_tags.end()){
                FN++;
            }
        }
    }
}