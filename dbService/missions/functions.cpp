#include "./missions.h"

//take sentence from user for example "this is a senetence" break it down into array of words that combine the sentence - "this", "is" "a" "sentence"
void getInputFromUser(vector<string>& words, string message)
{
    cout << message << endl;
    string line;
    getline(cin, line);

    istringstream iss(line);
    string code;
    while (iss >> code)
        words.push_back(code);
}