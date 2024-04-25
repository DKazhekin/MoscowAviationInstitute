#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;

void getZ(string str, int Z[])
{
    size_t n = str.length();
    int L, R, k;

    // [L,R] make a window which matches with prefix of s
    L = R = 0;
    for (int i = 1; i < n; ++i)
    {
        // if i>R nothing matches, so we will calculate.
        // Z[i] using naive way.
        if (i > R)
        {
            L = R = i;

            // R-L = 0 in starting, so it will start
            // checking from 0'th index. For example,
            // for "ababab" and i = 1, the value of R
            // remains 0 and Z[i] becomes 0. For string
            // "aaaaaa" and i = 1, Z[i] and R become 5
            while (R<n && str[R-L] == str[R])
                R++;
            Z[i] = R-L;
            R--;
        }
        else
        {
            // k = i-L so k corresponds to number which
            // matches in [L,R] interval.
            k = i-L;

            // if Z[k] is less than remaining interval
            // then Z[i] will be equal to Z[k].
            // For example, str = "ababab", i = 3, R = 5
            // and L = 2
            if (Z[k] < R-i+1)
                Z[i] = Z[k];

                // For example str = "aaaaaa" and i = 2, R is 5,
                // L is 0
            else
            {
                // else start from R and check manually
                L = i;
                while (R<n && str[R-L] == str[R])
                    R++;
                Z[i] = R-L;
                R--;
            }
        }
    }
}

string trim(string str) {
    // Remove spaces at the end and at the beginning
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](char c) {return !isspace(c);}));
    str.erase(find_if(str.rbegin(), str.rend(), [](char c) {return !isspace(c);}).base(), str.end());

    // Removing spaces between words
    auto new_end = std::unique(str.begin(), str.end(), [](char a, char b){return isspace(a) && std::isspace(b);});
    str.erase(new_end, str.end());

    return str;
}

int main() {
    ifstream fin(INPUT_FILE);
    //istream& fin = cin;

    // Vector of count of words in the string
    vector<int> words_in_str;

    // Initializing pattern
    string pattern;
    getline(fin, pattern);
    transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
    pattern = trim(pattern);

    // Counting words in pattern
    int counter_words_in_pattern = 0;
    for (char i: pattern) {
        if (i == ' ') {
            counter_words_in_pattern++;
        }
    }
    counter_words_in_pattern++;

    // Preprocessing the concat string
    string buffer;

    string concat;
    concat.append(pattern);
    concat.append("$");

    // Cycle for adjusting concat string + making prefix sum vector of words in each string
    while (getline(fin, buffer)) {

        transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
        replace(buffer.begin(), buffer.end(), '\n', ' ');
        buffer = trim(buffer);

        // Counter of words in each string
        int word_counter = 0;

        // If string is not empty then we need to adjust: concat string + words_in_str + word_counter
        if (!buffer.empty()) {
            for (char i: buffer) {
                if (i == ' ') {
                    word_counter++;
                }
            }
            word_counter++;

            // Making format string
            concat.append(buffer + ' ');
        }

        words_in_str.push_back(word_counter);
    }

    // Remove the last space
    concat.pop_back();

    // Make a prefix sum vector
    for (int i = 1; i < words_in_str.size(); i++){
        words_in_str[i] = words_in_str[i] + words_in_str[i-1];
    }

    // Creating Z array
    size_t length = concat.length();
    int Z[length];

    // Getting Z array of newly string
    getZ(concat, Z);

    // Processing final string
    int words_counter_in_concat = 1;

    // Indexes of start and end of the pattern
    size_t f_pos = pattern.length() + 1;
    size_t n_pos = concat.find(' ', f_pos);

    // Adjust the start value for end index of pattern depends on count of words
    for (int i = 1; i < counter_words_in_pattern; i++){
        n_pos = concat.find(' ', n_pos + 1);
    }

    if (n_pos == string::npos){
        n_pos = concat.length();
    }

//    for (auto e : concat){
//        cout << e;
//    }

    // Final processing
    for (size_t i = pattern.length() + 1; i < concat.length(); i++){
        if (concat[i] == ' '){
            words_counter_in_concat++;
            f_pos = i + 1;
            n_pos = concat.find(' ', n_pos + 1);
            if (n_pos == string::npos){
                n_pos = concat.length();
            }

        }
        if (Z[i] == pattern.length() && (n_pos - f_pos) == pattern.length()){
            int j = 0;
            for ( ; j < words_in_str.size(); j++) {
                if (words_counter_in_concat <= words_in_str[j]){
                    break;
                }
            }
            int word;
            if (j == 0){
                word = words_counter_in_concat;
            }
            else{
                word = words_counter_in_concat - words_in_str[j - 1];
            }
            cout << j + 1 << ", " << word << endl;
        }
    }
    return 0;
}
