#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

#define zero_code "_0"
#define ones_code "_1"
#define eps numeric_limits<double>::epsilon()

using namespace std;

double param_prob(int all_words, const string& word, unordered_map<string, int> *dict);
double condition_prob(const string& label_code, const string& word, unordered_map<string, int> *dict, int all_words_param);
double label_prob(int label, int all_labels);
void output(double label0, double label1);


int main(){
    int train, test;
    cin >> train;
    cin >> test;
    cin.get();

    int all_words = 0;
    int all_words_0 = 0;
    int all_words_1 = 0;
    int zero_label = 0;
    int one_label = 0;

    unordered_map<string, int> words_count;
    int tmp_label;
    string tmp_string;
    string word;

    // Тренировка
    for (int i = 0; i < train; i++){
        // Считывание лейбла
        cin >> tmp_label;
        cin.ignore();
        if (tmp_label == 0){
            zero_label++;
        }
        else{
            one_label++;
        }

        // Считывание строки
        getline(cin, tmp_string);
        istringstream stream(tmp_string);
        while(stream >> word){

            all_words++;

            // Преобразования слов
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word.back() == '.'){word.pop_back();}

            // Заполнение информации
            if (tmp_label == 0){
                word += zero_code;
                all_words_0++;
            }
            else{
                word += ones_code;
                all_words_1++;
            }

            if (words_count.find(word) != words_count.end()) {words_count[word]++;}
            else {words_count[word] = 1;}
        }
    }

    // Валидация
    vector<int> predict;
    for (int i = 0; i < test; i++){
        getline(cin, tmp_string);
        istringstream stream(tmp_string);

        double label_0 = 0;
        double label_1 = 0;
        while(stream >> word){
            // Преобразования строки
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word.back() == '.'){word.pop_back();}

            label_0 += log(condition_prob(zero_code, word, &words_count, all_words_0)) - log(param_prob(all_words, word, &words_count));
            label_1 += log(condition_prob(ones_code, word, &words_count, all_words_1)) - log(param_prob(all_words, word, &words_count));
        }
        label_0 += log(label_prob(zero_label, one_label + zero_label));
        label_1 += log(label_prob(one_label, one_label + zero_label));

        output(label_0, label_1);
    }
}

double param_prob(int all_words, const string& word, unordered_map<string, int> *dict){
    int count_word = 0;

    if ((*dict).find(word + zero_code) != (*dict).end()){count_word += (*dict)[word + zero_code];}
    if ((*dict).find(word + ones_code) != (*dict).end()){count_word += (*dict)[word + ones_code];}

    return ((double)count_word / all_words + eps) + eps;
}

double condition_prob(const string& label_code, const string& word, unordered_map<string, int> *dict, int all_words_param){

    int count_word = 0;

    string find = word + label_code;
    if (dict->find(find) != dict->end()){
        count_word += (*dict)[find];
    }

    return ((double)count_word / all_words_param + eps) + eps;
}

double label_prob(int label, int all_labels){
    return ((double)label / all_labels + eps) + eps;
}

void output(double label0, double label1){
    if (label0 > label1){
        cout << 0 << endl;
    }
    else{
        cout << 1 << endl;
    }
}
