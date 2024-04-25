#ifndef DA_NB_HELPER_H
#define DA_NB_HELPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include "nlohmann/json.hpp"
#include <unordered_set>

using namespace std;
using namespace nlohmann;

typedef struct parse {
    string action;
    string input_file;
    string output_file;
    string stats_file;
} parse;

void parse_flags(int argc, char *argv[], parse &info);
void processing_file(unordered_map<string, unordered_map<string, int>> &words_related_2_tag,
                     unordered_map<string, int> &docs_amount_per_tag, ifstream &file);
void serialize_send(unordered_map<string, unordered_map<string, int>> &words_related_2_tag,
                    unordered_map<string, int> &docs_amount_per_tag, const parse &info);
void deserialize(unordered_map<string, unordered_map<string, int> > &words_related_2_tag,
                 unordered_map<string, int> &docs_amount_per_tag, const parse &info);
void get_tags(vector<pair<string, double>> &tags, unordered_map<string, int> &docs_amount_per_tag);
double hashmap_sum(unordered_map<string, int> &docs_amount_per_tag);
double count_unique_words(const unordered_map<string, unordered_map<string, int>> &words_related_2_tag);
double get_uncoditional_prob(unordered_map<string, int> &word_counter, const string& word, double unique_values, int data_power);
double word_count(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, string tag, string word);
double all_word_count(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, string tag);
double fill_dict(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, unordered_map<string, int> &word_counter);
void evaluate(unordered_map<string, unordered_map<string, int>> &words_related_2_tag, unordered_map<string, int> &docs_amount_per_tag, ifstream &file, unordered_map<string, int> &word_counter, double data_power, parse &info);
void sorting(vector<pair<string, double>> &tags);
void inference(vector<pair<string, double>> &tags, double threshold, ofstream &output_file);
void calc_statistics(parse &info, double &TP, double &FP, double &FN);

#endif
