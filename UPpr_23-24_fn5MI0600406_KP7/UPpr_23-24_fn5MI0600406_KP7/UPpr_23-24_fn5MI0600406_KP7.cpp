/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Hristo Hristov
* @idnumber 5MI0600406* @compiler VC
*
* <File with the code for the game Scrabble>
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

using namespace std;

int menu(void);
void startGame(const int, const int, const vector<string> words);
void save(const char*, const vector<string>&);
void addWord(vector<string>&);
vector<string> load(const char*);
vector<string> extractValidWords(const vector<string>&, const int&);
void shuffleWord(string&);
bool exists(const string&, const vector<string>&);
void printLetters(const string&);
bool isValid(const string&, const string&);
void tolower(string&);
int submenu(void);

int main(void) {
    system("chcp 1251>0"); // -> Display Cyrillic text
    int maxLetters = 10;
    int maxRounds = 10;

    vector<string> words = load("words.txt");

    int selection = menu();
    while (selection != 4) {
        switch (selection) {
        case 1:
            startGame(maxLetters, maxRounds,
                extractValidWords(words, maxLetters));
            break;

        case 2:
            selection = submenu();
            switch (selection) {
            case 1:
                cout << "Enter the number of characters: ";
                cin >> maxLetters;
                break;
            case 2:
                cout << "Enter the number of rounds: ";
                cin >> maxRounds;
                cout << maxRounds << endl;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
            }
            break;

        case 3:
            addWord(words);
            break;

        case 4:
            return 0;

        default:
            cout << "Invalid option." << endl;
            break;
        }
        selection = menu();
    }

    save("words.txt", words);
    return 0;
}

void save(const char* filename, const vector<string>& words) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& word : words) {
            outFile << word << endl;
        }
        outFile.close();
        cout << "The words have been successfully written to the file." << endl;
    }
    else {
        cout << "The file could not be opened for writing." << endl;
    }
}

vector<string> load(const char* filename) {
    vector<string> words;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            words.push_back(line);
        }
        inFile.close();
        cout << "The words were successfully read from the file." << endl;
    }
    else {
        cerr << "The file cannot be opened for reading." << endl;
    }
    return words;
}

vector<string> extractValidWords(const vector<string>& words, const int& maxLetters) {
    vector<string> filtered;
    for (auto word : words) {
        if (word.size() <= maxLetters) {
            filtered.push_back(word);
        }
    }
    return filtered;
}

void startGame(const int letters, const int rounds, const vector<string> words) {

    int points = 0;
    int round = 0;

    srand(time(nullptr));

    while (round < rounds) {

        string word = words.at(rand() % words.size());
        int missing = letters - word.size();

        string answer;
        bool guessed;

        for (int i = 0; i < missing; i++) {
            word += 'a' + rand() % 26;
        }

        shuffleWord(word);

        cout << "Round " << round << ". Available letters: ";
        printLetters(word);

        guessed = false;
        while (!guessed) {
            cin >> answer;
            guessed = exists(answer, words) && isValid(answer, word);
            if (!guessed) {
                cout << "Invalid word. Try again with: ";
                printLetters(word);
            }
        }

        points += answer.size();
        cout << "Your poinsts so far are: " << points << endl;
        round++;

    }

    cout << "Your total points are: " << points << endl;
    cout << "Returning to menu." << endl;

}

bool isValid(const string& word, const string& original) {

    int alphabet[26] = { 0 };
    for (const char& letter : original) alphabet[letter - 'a']++;
    for (const char& letter : word)     alphabet[letter - 'a']--;

    for (const int value : alphabet) {
        if (value < 0) {
            return false;
        }
    }

    return true;
}

bool exists(const string& search, const vector<string>& words) {
    bool guessed = false;
    for (const string& word : words) {
        if (word == search) {
            guessed = true;
            break;
        }
    }
    return guessed;
}

void printLetters(const string& word) {
    for (const char& letter : word) {
        cout << letter << " ";
    }
    cout << endl;
}

void addWord(vector<string>& words) {

    bool isUnique;
    string newWord;

    cout << "Нова дума: ";
    cin >> newWord;

    tolower(newWord);

    isUnique = true;
    for (string word : words) {
        if (word == newWord) {
            isUnique = false;
            break;
        }
    }

    if (isUnique) {
        words.push_back(newWord);
        cout << "Думата е добавена успешно." << endl;
    }
    else {
        cout << "Думата е вече в речника." << endl;
    }

}

void tolower(string& word) {
    for (char& letter : word) {
        if (letter >= 'A' && letter <= 'Z') {
            letter = letter - 'A' + 'a';
        }
    }
}

void shuffleWord(string& word) {

    int length = word.size();
    srand(time(0));

    for (int i = 0; i < length - 1; i++) {
        int randomIndex = i + rand() % (length - i);
        swap(word[i], word[randomIndex]);
    }

}

int menu(void) {

    int selection;

    cout << "Меню:" << endl;
    cout << "1. Стартиране на нова игра" << endl;
    cout << "2. Настройки" << endl;
    cout << "3. Въвеждне на нова дума" << endl;
    cout << "4. Изход" << endl;

    cin >> selection;
    return selection;

}

int submenu(void) {

    int selection;

    cout << "1. Променяне на броя на подаваните букви" << endl;
    cout << "2. Променяне на броя рундове" << endl;

    cin >> selection;
    return selection;

}