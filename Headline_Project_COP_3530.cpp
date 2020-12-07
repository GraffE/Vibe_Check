#include <fstream>
#include <ctime>
#include "HashMap.h"
#include "AVLTree.h"
#include "Headline.h"
using namespace std;

// loadWords takes in a reference to an empty unordered map and fills it with words from the subjectivity database
// Each word has a subjectivity "score". Words with positive connotations will have a score of 1. Negative words will have a score of -1. Neutral: 0
// A words score will double if the database records that word as having strong subjectivity. Weak words will remain the same.
void loadWords(unordered_map<string, int>& words) {
    ifstream wordFile;
    wordFile.open("WordData.txt");

    string line;
    string word;
    int multiplier = 1;
    if (wordFile.is_open()) {
        while (getline(wordFile, line)) {

            // The following sets up a multiplier based on the word's subjectivity strength.
            if (line.substr(0, 8) == "type=str") {
                multiplier = 2;
            }
            else {
                multiplier = 1;
            }


            line = line.substr(line.find("word1=") + 6, line.length());
            word = line.substr(0, line.find(" "));
            line = line.substr(line.find("polarity=") + 9, line.length());
            int score;
            if (line == "positive") {
                score = 1;
            }
            else if (line == "negative") {
                score = -1;
            }
            else {
                score = 0;
            }
            words[word] = score * multiplier;
        }
        wordFile.close();
    }
}


// loadheadlines both extracts the headline data from the source file and inserts the headline into two data structures.
// the method takes in references to a HashMap and an AVL Tree for storage, as well as the word data for headline construction.
void loadHeadlines(Hash_Map<Headline>& HM, AVLTree& tree, unordered_map<string, int>& words, int numHeadlines) {
    ifstream headlineFile;
    headlineFile.open("HeadlineData.txt");

    string line, title, description, date;
    int headlineCount = 0;
    if (headlineFile.is_open()) {
        while (getline(headlineFile, line)) {
            headlineCount++;
            line = line.substr(line.find("headline\":") + 11, line.length());
            title = line.substr(0, line.find("\",") + 1);
            line = line.substr(line.find("_description") + 15, line.length());
            description = line.substr(0, line.find("\",") + 1);
            date = line.substr(line.find("date\":") + 8, line.length());
            date = date.substr(0, date.length() - 3);


            Headline x = Headline(title, date, description, words);
            HM.insert(x, date);
            tree.root = tree.insert(tree.root, &x);


            // IMPORTANT: Use this for choosing how many headlines to load it.
            if (headlineCount == numHeadlines) {
                break;
            }
        }
        headlineFile.close();
    }
}



int main() {

    clock_t time;


    cout << "Loading Words... " << endl;
    time = clock();
    unordered_map<string, int> words;
    loadWords(words);
    time = clock() - time;
    cout << "Words Loaded in " << (float)time/CLOCKS_PER_SEC << " seconds." << endl;

    Hash_Map<Headline> HM;
    AVLTree tree;


    int maxHeadlines = -1;
    cout << "\nWe have 200,853 headlines in the database." << endl;
    cout << "How many headlines would you like to load? Enter -1 to load all of them." << endl;
    cin >> maxHeadlines;

    cout << "Loading Headlines..." << endl;
    time = clock();
    loadHeadlines(HM, tree, words, maxHeadlines);
    time = clock() - time;
    cout << "Headlines Loaded in " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;

    cout << endl;
    cout << "*********************" << endl;
    cout << "Welcome to Vibe Check" << endl;
    cout << "*********************" << endl;


    // Note: Program does not run when variables are initialized inside switch function.
    // Make all variables needed out here.
    int input;
    string inputStr;
    int sum;
    int p, n;
    bool mode = true; // Hash Map is true. AVL Tree is false.
    vector<Headline*> headlines;


    do
    {
        headlines.clear();
        Headline headline ("", "", "", words);
        headlines.push_back(&headline); // This puts an empty headline in for comparison needs.

        if (mode) {
            cout << "\nYou are in Hash Map Mode.";
        }
        else {
            cout << "\nYou are in AVL Tree Mode.";
        }

        cout << "\n\n**************************" << endl;
        cout << "What would you like to do?" << endl;
        cout << "**************************\n" << endl;

        cout << "(0) Switch Data Structure." << endl; // Done
        cout << "(1) Find the most Positive Headlines" << endl; // Done
        cout << "(2) Find the most Negative Headlines" << endl; // Done
        cout << "(3) Find the Subjectivity Score of a Date" << endl; // Done
        cout << "(4) Find the most Positive and Negative Headlines from a Date" << endl; // Done
        cout << "(5) Print all the headlines from a given Date." << endl;
        cout << "(6) Quit the Program." << endl; // Done


        cin >> input;

        switch (input) {
            case 0: // Switches the program to use the other data structure.
                mode = !mode;
                break;
            case 1: // Finds all the most positive headlines.
                if (mode) {
                    sum = 0;
                    time = clock();
                    for (int i = 0; i < 4000; i++) {
                        for (int j = 0; j < HM[i].size(); j++) {
                            sum++;
                            if (headlines[0]->getScore() == HM[i][j].getScore()) {
                                headlines.push_back(&HM[i][j]);
                            }
                            if (headlines[0]->getScore() < HM[i][j].getScore()) {
                                headlines.clear();
                                headlines.push_back(&HM[i][j]);
                            }
                        }
                    }
                    time = clock() - time;

                    cout << "The most positive headlines out of a total of " << sum << " in the dataset." << endl;
                    for (int k = 0; k < headlines.size(); k++) {
                        headlines[k]->printAll();
                    }

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                } else {
                    time = clock();
                    queue<Node *> q;
                    sum = 0;

                    if (tree.root != nullptr)
                        q.push(tree.root);

                    while (!q.empty()) {
                        for (int i = 0; i < q.front()->vals.size(); i++) {
                            sum++;
                            if (headlines[0]->getScore() == q.front()->vals[i].getScore()) {
                                headlines.push_back(&q.front()->vals[i]);
                            }
                            if (headlines[0]->getScore() < q.front()->vals[i].getScore()) {
                                headlines.clear();
                                headlines.push_back(&q.front()->vals[i]);
                            }
                        }

                        if (q.front()->right != nullptr) {
                            q.push(q.front()->right);
                        }

                        if (q.front()->left != nullptr) {
                            q.push(q.front()->left);
                        }
                        q.pop();
                    }
                    time = clock() - time;

                    cout << "The most positive headlines out of a total of " << sum << " in the dataset." << endl;
                    for (int k = 0; k < headlines.size(); k++) {
                        headlines[k]->printAll();
                    }

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                break;
            case 2: // Finds all the most negative headlines.
                if (mode) {
                    sum = 0;
                    time = clock();

                    for (int i = 0; i < 4000; i++) {
                        for (int j = 0; j < HM[i].size(); j++) {
                            sum++;
                            if (headlines[0]->getScore() == HM[i][j].getScore()) {
                                headlines.push_back(&HM[i][j]);
                            }
                            if (headlines[0]->getScore() > HM[i][j].getScore()) {
                                headlines.clear();
                                headlines.push_back(&HM[i][j]);
                            }
                        }
                    }
                    time = clock() - time;

                    cout << "The most negative headlines out of a total of " << sum << " in the dataset." << endl;
                    for (int k = 0; k < headlines.size(); k++) {
                        headlines[k]->printAll();
                    }

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                else {
                    time = clock();
                    queue<Node*> q;
                    sum = 0;

                    if(tree.root != nullptr)
                        q.push(tree.root);

                    while(!q.empty()) {
                        for (int i = 0; i < q.front()->vals.size(); i++) {
                            sum++;
                            if (headlines[0]->getScore() == q.front()->vals[i].getScore()) {
                                headlines.push_back(&q.front()->vals[i]);
                            }
                            if (headlines[0]->getScore() > q.front()->vals[i].getScore()) {
                                headlines.clear();
                                headlines.push_back(&q.front()->vals[i]);
                            }
                        }

                        if (q.front()->right != nullptr) {
                            q.push(q.front()->right);
                        }

                        if (q.front()->left != nullptr) {
                            q.push(q.front()->left);
                        }
                        q.pop();
                    }
                    time = clock() - time;

                    cout << "The most negative headlines out of a total of " << sum << " in the dataset." << endl;
                    for (int k = 0; k < headlines.size(); k++) {
                        headlines[k]->printAll();
                    }

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                break;
            case 3: // Calculates the subj score of a particular date.
                if (mode) {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    sum = 0;
                    time = clock();

                    for (int i = 0; i < HM[inputStr].size(); i++) {
                        sum += HM[inputStr][i].getScore();
                    }
                    time = clock() - time;

                    cout << endl << "There were " << HM[inputStr].size() << " headlines on " << inputStr;
                    cout << ", with a total Subjectivity Score of " << sum << "." << endl;

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                else {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    Node* date;
                    time = clock();

                    date = tree.getDay(inputStr);
                    time = clock() - time;

                    if(date != nullptr) {
                        cout << endl << "There were " << date->vals.size() << " headlines on " << inputStr;
                        cout << ", with a total Subjectivity Score of " << date->total << "." << endl;
                        cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                    }
                }
                break;
            case 4: // Displays the most positive and negative headline of a date.
                if (mode) {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    p = 0;
                    n = 0;
                    time = clock();

                    for (int i = 1; i < HM[inputStr].size(); i++) {
                        if (HM[inputStr][i].getScore() > HM[inputStr][p].getScore()) {
                            p = i;
                        }
                        if (HM[inputStr][i].getScore() < HM[inputStr][n].getScore()) {
                            n = i;
                        }
                    }
                    time = clock() - time;

                    cout << "\nThe most positive headline of this day was: " << endl;
                    HM[inputStr][p].printAllData();
                    cout << "\nThe most negative headline of this day was: " << endl;
                    HM[inputStr][n].printAllData();

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                else {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    Node* date;
                    time = clock();

                    date = tree.getDay(inputStr);
                    if(date != nullptr) {
                        int max = -100000;
                        int maxIndex = -1;
                        int min = 100000;
                        int minIndex = -1;
                        for (int i = 0; i < date->vals.size(); i++) {
                            if (date->vals[i].getScore() < min) {
                                min = date->vals[i].getScore();
                                minIndex = i;
                            }
                            if (date->vals[i].getScore() > max) {
                                max = date->vals[i].getScore();
                                maxIndex = i;
                            }
                        }
                        time = clock() - time;

                        cout << "\nThe most positive headline of this day was: " << endl;
                        date->vals[maxIndex].printAllData();
                        cout << "\nThe most negative headline of this day was: " << endl;
                        date->vals[minIndex].printAllData();

                        cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                    }
                }
                break;
            case 5: // Prints all headlines from a date.
                if (mode) {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    time = clock();

                    for (int i = 0; i < HM[inputStr].size(); i++) {
                        HM[inputStr][i].print();
                    }
                    time = clock() - time;

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                else {
                    cout << endl << "Please enter a date.\nUse the format yyyy-mm-dd" << endl;
                    cout << "Available headlines range from 2012-01-28 to 2018-05-26" << endl;
                    cin >> inputStr;
                    time = clock();

                    Node* date = tree.getDay(inputStr);
                    if(date != nullptr) {
                        for (int i = 0; i < date->vals.size(); i++) {
                            date->vals[i].print();
                        }
                    }
                    time = clock() - time;

                    cout << "\nTime taken: " << (float)time / CLOCKS_PER_SEC << " seconds." << endl;
                }
                break;
            case 6:
                cout << "\nThank you come again." << endl;
                break;
            default:
                cout << "\nError: Incorrect input" << endl;
                break;
        }

    } while (input != 6);

    return 0;
}