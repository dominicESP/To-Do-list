#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <limits>

using namespace std;

void addTask(vector<string> &list, int tasksToAdd)
{
    for (int i = 0; i < tasksToAdd; i++)
    {
        string task;
        cout << "Write your task: ";
        getline(cin, task);

        list.push_back(task);
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

        pendentTasksList.erase(remove(pendentTasksList.begin(), pendentTasksList.end(), task), pendentTasksList.end());
        completedTasksList.push_back(task);
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

int main()
{

    vector<string> toDoList;
    vector<string> completedList;

    int option;
    int tasksToAdd;
    int tasksToRemove;
    int looping;

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

        getline(cin, input);
        option = stoi(input);

        switch (option)
        {
        case 1:
            cout << "\nWrite the numbers of tasks you want to add: ";
            getline(cin, input);
            tasksToAdd = stoi(input);

            addTask(toDoList, tasksToAdd);
            break;

        case 2:
            cout << "\nWrite the numbers of tasks you want to remove: ";
            getline(cin, input);
            tasksToRemove = stoi(input);

            removeTask(toDoList, tasksToRemove);
            break;

        case 3:
            cout << "\nPendent tasks:" << endl;

            showTask(toDoList);
            break;

        case 4:
            taskCompleted(toDoList, completedList);
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

        getline(cin, input);
        looping = stoi(input);

        cout << endl;

    } while (looping == 2);

    return 0;
}
