#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// function to separate the text by sentences that end with a dot
vector<string> sentenceSeparator(string textLine) {
    vector<string> sentences;
    string sentence;
    int separator, size = textLine.size();

    while(textLine.size() > 0) {
        separator = textLine.find(". ");                // looking for a sentence

        if(separator == -1) {                             // the last sentence remains
            sentence = textLine;
            sentences.push_back(sentence);
            break;
        }
        else
            sentence = textLine.substr(0, separator+1);

        sentences.push_back(sentence);                     // pushing the found sentence to the vector
        textLine = textLine.substr(separator+2, size);
    }

    return sentences;
}

// function to uppercase all entries of a query word in a sentence
string toUppercase(string sentence, vector<int> indices, int wordSize) {
    for(auto index : indices) {                                     // for each entry of the query word
        for(int i = index; i < index+wordSize; i++)
            if(97 <= sentence[i] && sentence[i] <= 122)             // if lowercase
                sentence[i] = sentence[i] - 32;                     // make it uppercase (ASCII table)
    }
    return sentence;
}

// function to check if a found query word entry is a part of a bigger word
bool checkIfFragment(string sentence, int entry, int wordSize) {
    char before, after;
    int beforeInd = entry - 1, afterInd = entry + wordSize, size = sentence.size();
    if(beforeInd == 0) beforeInd++;                     // preventing the out of the bounds access
    if(afterInd > size) afterInd = size - 1;

    before = sentence[beforeInd];                       // a char before the word
    after = sentence[afterInd];                         // a char after the word

    if(65 <= before && before <= 90 || 97 <= before && before <= 122 ||
       65 <= after && after <= 90 || 97 <= after && after <= 122)           // if one of these characters is a letter
        return true;                                                        // then the query word is a fragment of a bigger word

    return false;                                                           // otherwise the function has found the query word
}                                                                           // that is not a fragment

// function to find a query word in a sentence
// it returns the sentence with the UPPERCASE query word
string findWord(string sentence, string word) {
    string originalSentence = sentence;
    vector<int> indices;
    int entry, wordSize = word.size();
    bool isFragment;

    // find entries of query word and get the indices
    entry = -wordSize;
    while(true) {
        entry = sentence.find(word, entry + wordSize);
        if(entry == -1) break;                                      // there are no entries left
        isFragment = checkIfFragment(sentence, entry, wordSize);
        if(!isFragment) indices.push_back(entry);                  // if the function have found a word but not a fragment,
    }                                                              // then push its entry index to the vector of indices

    // if at least one entry is found, uppercase the word in the original sentence and return
    if(indices.size() > 0) {
        originalSentence = toUppercase(originalSentence, indices, wordSize);
        return originalSentence;
    }
    else                                                            // otherwise return an empty string
        return "";
}

// function to find the entries of the query word in the set of sentences
vector<vector<string>> wordsMatches(vector<string> sentences, vector<string> queryWords) {
    vector<vector<string>> matches;
    string match;

    for(auto word : queryWords) {                       // for each query word
        vector<string> matchVector;                     // declaration of a nested vector<string>

        for (auto sentence : sentences) {               // for each sentence
            match = findWord(sentence, word);           // find the word in the sentence
            if (match == "") continue;
            matchVector.push_back(match);               // if match is found, push it to the vector of matches
        }
        matches.push_back(matchVector);
    }

    return matches;
}

int main() {

    // READING AND PROCESSING OF THE INPUT DATA
    string textLine, line;
    int wordsNumber;
    vector<string> queryWords, sentences;
    vector<vector<string>> matches;

    ifstream in("in.txt");

    if(in.is_open()) getline(in, textLine);     // reading the text
    if(in.is_open()) getline(in, line);         // reading the number of query words
    wordsNumber = stoi(line);                         // converting number of query words to int

    if(in.is_open())
        while (getline(in, line))
            queryWords.push_back(line);
    in.close();

    // FUNCTION CALLS
    sentences = sentenceSeparator(textLine);
    matches = wordsMatches(sentences, queryWords);

    // OUTPUT
    ofstream out;
    out.open("out.txt");

    if (out.is_open())
        for(auto match : matches) {
            out << match.size() << endl;
            for (auto sentence : match)
                out << sentence << endl;
        }
    out.close();

    return 0;
}
