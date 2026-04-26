#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <ctime>

using namespace std;

const string pendent = "pendent.txt";
const string completed = "completed.txt";
const string dates = "dates.txt";
const string completedDates = "completedDates.txt";

string current_date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return "[" + to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "] ";
}

void addTask(vector<string> &dateList, vector<string> &list, int tasksToAdd) {
    for (int i = 0; i < tasksToAdd; i++) {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        dateList.push_back(current_date());
        list.push_back(task);
        cout << "\nTask added: " << task << endl;
    }
}

void removeTask(vector<string> &dateList, vector<string> &list, int tasksToRemove) {
    for (int i = 0; i < tasksToRemove; i++) {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        auto it = find(list.begin(), list.end(), task);
        if (it != list.end()) {
            int index = distance(list.begin(), it);
            cout << "\nTask removed: " << task << endl;
            list.erase(it);

            if (index < (int)dateList.size()) {
                dateList.erase(dateList.begin() + index);
            }
        } else {
            cout << "\nTask: " << task << " does not exist" << endl;
        }
    }
}

void showTask(vector<string> &dateList, vector<string> &list) {
    if (list.empty()) {
        cout << "No tasks available" << endl;
        return;
    }

    for (int i = 0; i < (int)list.size(); i++) {
        string date = (i < (int)dateList.size()) ? dateList[i] : "";
        cout << i + 1 << ". " << date << list[i] << endl;
    }
}

void taskCompleted(vector<string> &dateList, vector<string> &completedDateList, vector<string> &pendentList, vector<string> &completedList) {
    if (pendentList.empty()) {
        cout << "There are no pendent tasks" << endl;
        return;
    }

    cout << "Which task do you want to set as completed?" << endl;
    showTask(dateList, pendentList);
    cout << "Write the task here: ";
    string task;
    getline(cin, task);
    cout << endl;

    auto it = find(pendentList.begin(), pendentList.end(), task);

    if (it != pendentList.end()) {
        int index = distance(pendentList.begin(), it);
        completedList.push_back(*it);
        completedDateList.push_back(current_date());
        pendentList.erase(it);
        if (index < (int)dateList.size()) {
            dateList.erase(dateList.begin() + index);
        }
        cout << "Task marked as completed: " << task << endl;
    } else {
        cout << "Task not found: " << task << endl;
    }
}

void showCompletedTasks(vector<string> &dateList, vector<string> &list) {
    if (list.empty()) {
        cout << "No tasks completed" << endl;
        return;
    }

    for (int i = 0; i < (int)list.size(); i++) {
        string date = (i < (int)dateList.size()) ? dateList[i] : "";
        cout << i + 1 << ". " << date << list[i] << endl;
    }
}


void removeCompletedTasks(vector<string> &dateList, vector<string> &list, int tasksToRemove) {
    for (int i = 0; i < tasksToRemove; i++) {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        auto it = find(list.begin(), list.end(), task);
        if (it != list.end()) {
            int index = distance(list.begin(), it);
            cout << "\nTask removed: " << task << endl;
            list.erase(it);

            if (index < (int)dateList.size()) {
                dateList.erase(dateList.begin() + index);
            }
        } else {
            cout << "\nTask: " << task << " does not exist" << endl;
        }
    }
}

void uploadData(const string fileName, vector<string> &list) {
    ifstream file(fileName);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                list.push_back(line);
            }
        }
        file.close();
    }
}

void saveData(const string fileName, vector<string> &list) {
    ofstream file(fileName);

    if (file.is_open()) {
        for (const string &task : list) {
            file << task << endl;
        }
        file.close();
    }
}

int main() {
    vector<string> toDoList;
    vector<string> completedList;
    vector<string> dateList;
    vector<string> completedDatesList;

    int option, tasksToAdd, tasksToRemove, looping;

    uploadData(pendent, toDoList);
    uploadData(completed, completedList);
    uploadData(dates, dateList);
    uploadData(completedDates, completedDatesList);

    do {
        string input;

        cout << "Choose an option: " << endl;
        cout << "1. Add task" << endl;
        cout << "2. Remove task" << endl;
        cout << "3. Show pendent tasks" << endl;
        cout << "4. Add completed task" << endl;
        cout << "5. Show completed tasks" << endl;
        cout << "6. Remove completed tasks" << endl;
        cout << "Write your number: ";

        while (true) {
            try {
                getline(cin, input);
                option = stoi(input);
                if (option < 1 || option > 6) {
                    throw out_of_range("Invalid number");
                }
                break;
            } catch (const invalid_argument &e) {
                cout << "\nError: write a valid number: ";
            } catch (const out_of_range &e) {
                cout << "\nChoose a number between 1 and 6: ";
            }
        }

        switch (option) {
        case 1:
            cout << "\nHow many tasks do you want to add? ";
            getline(cin, input);
            tasksToAdd = stoi(input);
            addTask(dateList, toDoList, tasksToAdd);
            saveData(pendent, toDoList);
            saveData(dates, dateList);
            break;

        case 2:
            cout << "\nHow many tasks do you want to remove? ";
            getline(cin, input);
            tasksToRemove = stoi(input);
            removeTask(dateList, toDoList, tasksToRemove);
            saveData(pendent, toDoList);
            saveData(dates, dateList);
            break;

        case 3:
            cout << "\nPendent tasks:" << endl;
            showTask(dateList, toDoList);
            break;

        case 4:
            taskCompleted(dateList, completedDatesList, toDoList, completedList);
            saveData(pendent, toDoList);
            saveData(completed, completedList);
            saveData(dates, dateList);
            saveData(completedDates, completedDatesList);
            break;

        case 5:
            cout << "\nCompleted tasks:" << endl;
            showCompletedTasks(completedDatesList, completedList);
            break;

        case 6:
            cout << "\nHow many tasks do you want to remove? ";
            getline(cin, input);
            tasksToRemove = stoi(input);
            removeCompletedTasks(completedDatesList, completedList, tasksToRemove);
            saveData(completed, completedList);
            saveData(completedDates, completedDatesList);
            break;
        }

        cout << endl;
        cout << "Do you want to leave?" << endl;
        cout << "1. Yes / 2. No: ";

        while (true) {
            try {
                getline(cin, input);
                looping = stoi(input);
                if(looping < 1 || looping > 2) {
                    throw out_of_range("\n");
                }
                cout << endl;
                break;
            } catch (const invalid_argument &e) {
                cout << "Write a valid number: ";
            } catch (const out_of_range &e) {
                cout << "Choose 1 or 2: ";
            }
        }

    }
    while (looping == 2);

    return 0;
}