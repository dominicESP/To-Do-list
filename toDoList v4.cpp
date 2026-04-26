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

string current_date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    // Format: [DD/MM]
    return "[" + to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "] ";
}


void addTask(vector<string> &list, int tasksToAdd)
{
    for (int i = 0; i < tasksToAdd; i++)
    {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        string taskWithDate = current_date() + task;

        list.push_back(taskWithDate);
        cout << "\nTask added: " << task << endl;
    }
}

void removeTask(vector<string> &list, int tasksToRemove)
{
    for (int i = 0; i < tasksToRemove; i++)
    {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        auto isInList = find(list.begin(), list.end(), task);
        if (isInList != list.end())
        {
            cout << "\nTask removed: " << task << endl;
            list.erase(remove(list.begin(), list.end(), task), list.end());
        }
        else
        {
            cout << "\nTask: " << task << " does not exist" << endl;
        }
    }
}

void showTask(vector<string> &list)
{
    if (list.empty())
    {
        cout << "No tasks available" << endl;
    }

    for (int i = 0; i < list.size(); i++)
    {
        cout << i + 1 << ". " << list[i] << endl;
    }
}

void taskCompleted(vector<string> &pendentTasksList, vector<string> &completedTasksList)
{
    string task;

    if (pendentTasksList.empty())
    {
        cout << "There are no pendent tasks" << endl;
    }
    else
    {
        cout << "Which task do you want to set as completed?" << endl;
        showTask(pendentTasksList);
        cout << "Write the task here: ";
        getline(cin, task);
        cout << endl;

        auto it = find(pendentTasksList.begin(), pendentTasksList.end(), task);

        if(it == pendentTasksList.end()) {
            pendentTasksList.erase(it);
            completedTasksList.push_back(*it);
        }
    }
}

void showCompletedTasks(vector<string> &list)
{
    cout << endl;
    if (list.empty())
    {
        cout << "No tasks completed" << endl;
    }

    for (int i = 0; i < list.size(); i++)
    {
        cout << i + 1 << ". " << list[i] << endl;
    }
}

void uploadData(const string fileName,vector<string> &list) {
    ifstream file(fileName);
    string line;

    if(file.is_open()) {
        while(getline(file, line)) {
            if(!line.empty()) {
                list.push_back(line);
            }
        }
        file.close();
    }
}

void saveData(const string fileName, vector<string> &list) {
    ofstream file(fileName);

    if(file.is_open()) {
        for(const string &task : list) {
            file << task << endl;
        }
        file.close();
    }
}

int main()
{

    vector<string> toDoList;
    vector<string> completedList;

    int option;
    int tasksToAdd;
    int tasksToRemove;
    int looping;

    uploadData(pendent, toDoList);
    uploadData(completed, completedList);

    do
    {
        string input;

        cout << "Choose an option: " << endl;
        cout << "1. Add task" << endl;
        cout << "2. Remove task" << endl;
        cout << "3. Show pendent tasks" << endl;
        cout << "4. Add completed task" << endl;
        cout << "5. Show completed tasks" << endl;
        cout << "Write your number: ";

        while (true) {
            try {
                getline(cin, input);
                option = stoi(input);

                if(option < 1 || option > 5) {
                    throw out_of_range("\nInvalid number");
                }
                break;
            } catch(const invalid_argument& e) {
                cout << "\nError: " << e.what() << endl;
                cout << "Write a valid number: ";
            } catch(const out_of_range& e) {
                cout << "\nThe number is too big" << endl;
                cout << "Write a valid number: ";
            }
        }

        switch (option)
        {
        case 1:
            cout << "\nWrite the numbers of tasks you want to add: ";
            getline(cin, input);
            tasksToAdd = stoi(input);

            addTask(toDoList, tasksToAdd);
            saveData(pendent, toDoList);
            break;

        case 2:
            cout << "\nWrite the numbers of tasks you want to remove: ";
            getline(cin, input);
            tasksToRemove = stoi(input);

            removeTask(toDoList, tasksToRemove);
            saveData(pendent, toDoList);
            break;

        case 3:
            cout << "\nPendent tasks:" << endl;

            showTask(toDoList);
            break;

        case 4:
            taskCompleted(toDoList, completedList);
            saveData(completed, toDoList);
            break;

        case 5:
            showCompletedTasks(completedList);
            break;

        default:
            cout << "Choose a valid option" << endl;
            break;
        }

        cout << endl;

        cout << "Do you want to leave?" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cout << "Write here: ";

        while(true) {
            try {
                getline(cin, input);
                looping = stoi(input);

                cout << endl;
                break;
            } catch(const invalid_argument& e) {
                cout << "Error: " << e.what() << endl;
                cout << "Write a valid number: ";
            } catch(const out_of_range& e) {
                cout << "The number is too big" << endl;
                cout << "Write a valid number: ";
            }
        }


    } while (looping == 2);

    return 0;
}
