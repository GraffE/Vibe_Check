#ifndef PROJECT3B_HEADLINE_H
#define PROJECT3B_HEADLINE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <algorithm>
using namespace std;

class Headline {
    string title;
    string date;
    string description;
    int score;
public:

    // Data retrieval:
    string getTitle() {
        return this->title;
    }
    string getDate() {
        return this->date;
    }
    string getDescription() {
        return description;
    }
    int getScore() {
        return score;
    }

    // Data displaying:
    void printAllData() {
        cout << "\nScore: " << this->score << endl;
        cout << "Title: " << this->title << endl;
        cout << "Description: " << this->description << endl;
    }

    //Data displaying with dates
    void printAll() {
        cout << "\nScore: " << this->score << endl;
        cout << "Date: " << this->date << endl;
        cout << "Title: " << this->title << endl;
        cout << "Description: " << this->description << endl;
    }

    void print() {
        cout << "\nScore: " << this->score << endl;
        cout << "Title: " << this->title << endl;
        //cout << "Date: " << this->date << endl;
    }

    // Constructor:
    Headline(string T, string D, string DS, unordered_map<string, int>& words) {
        title = T;
        date = D;
        description = DS;
        this->calculateScore(words);
    }

    // Method for calculating subjectivity score of a headline.
    // This method is called in the constructor and adds up the subjectivity score of all the words in the headline.
    // Result is stored in a private variable.
    void calculateScore(unordered_map<string, int>& words) {

        int total = 0;

        // Title must be converted to lower case.
        string lowerTitle = title;
        for_each(lowerTitle.begin(), lowerTitle.end(), [](char& c) {
            c = ::tolower(c);
        });


        // Finds the sum all the scores from the words in the title.
        istringstream iss(lowerTitle);
        do {
            string subs;
            iss >> subs;
            if (subs.empty()) {
                break;
            }
            if (subs.back() == '\'' || subs.back() == ',' || subs.back() == ' ' || subs.back() == '.' || subs.back() == '\"' || subs.back() == ':' || subs.back() == ';') {
                subs = subs.substr(0, subs.length() - 1);
            }
            if (subs[0] == '\'' || subs[0] == ' ' || subs[0] == '\"') {
                subs = subs.substr(1, subs.length());
            }
            total += words[subs];

        } while (iss);

        // Description must be converted to lower case.
        string lowerDesc = description;
        for_each(lowerDesc.begin(), lowerDesc.end(), [](char& c) {
            c = ::tolower(c);
        });


        // Adds score of the description to the total sum.
        istringstream iss2(lowerDesc);
        do {
            string subs;
            iss2 >> subs;
            if (subs.empty()) {
                break;
            }
            if (subs.back() == '\'' || subs.back() == ',' || subs.back() == ' ' || subs.back() == '.' || subs.back() == '\"' || subs.back() == ':' || subs.back() == ';') {
                subs = subs.substr(0, subs.length() - 1);
            }
            if (subs[0] == '\'' || subs[0] == ' ' || subs[0] == '\"') {
                subs = subs.substr(1, subs.length());
            }
            total += words[subs];

        } while (iss2);


        this->score = total;
    }

};


#endif //PROJECT3B_HEADLINE_H
