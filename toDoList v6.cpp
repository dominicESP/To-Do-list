#define _CRT_SECURE_NO_WARNINGS //define added to avoid the C4996 error in current_date because of the usage of localtime instead of localtime_s

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <ctime>

enum class Priority { HIGH = 1, MEDIUM, LOW };

std::string current_date() {
	time_t now = time(nullptr);
	tm* ltm = localtime(&now);
	return "[" + std::to_string(ltm->tm_mday) + "/" + std::to_string(1 + ltm->tm_mon) + "] ";
}

std::string taskPriority(Priority p) {
	std::string userPriority;
	int priority = 0;
	std::string returnPriority;
	std::cout << "Choose a priority (1. High, 2. Medium, 3. Low): ";
	while (true) {
		try {
			std::getline(std::cin, userPriority);
			priority = std::stoi(userPriority);

			if (priority < 1 || priority > 3) {
				throw std::out_of_range("\n");
			}

			p = static_cast<Priority>(priority);
			break;
		}
		catch (...) {
			std::cout << "Write 1, 2 or 3: ";
		}
	}

	switch (p) {
	case Priority::HIGH:
		returnPriority = "[HIGH] ";
		break;

	case Priority::MEDIUM:
		returnPriority = "[MEDIUM] ";
		break;

	case Priority::LOW:
		returnPriority = "[LOW] ";
		break;
	} // default is not needed because the try-catch inside the while loop forces p to be 1, 2 or 3

	return returnPriority;
}

void addTask(std::vector<std::string>& dateList, std::vector<std::string>& priorityList, std::vector<std::string>& list, int tasksToAdd, Priority p) {
	for (int i = 0; i < tasksToAdd; i++) {
		std::string task;
		std::cout << "Write your task: ";
		std::getline(std::cin, task);

		priorityList.push_back(taskPriority(p));
		dateList.push_back(current_date());
		list.push_back(task);
		std::cout << "\nTask added: " << task << std::endl;
	}
}

void showTask(std::vector<std::string>& dateList, std::vector<std::string>& priorityList, std::vector<std::string>& list) {
	if (list.empty()) {
		std::cout << "No pendent tasks" << std::endl;
		return;
	}

	for (int i = 0; i < (int)list.size(); i++) {
		std::string date = (i < (int)dateList.size()) ? dateList[i] : "";
		std::string priority = (i < (int)priorityList.size()) ? priorityList[i] : "";
		std::cout << i + 1 << ". " << date << priority << list[i] << std::endl;
	}
}

void removeTask(std::vector<std::string>& dateList, std::vector<std::string>& priorityList, std::vector<std::string>& list, int tasksToRemove) {
	for (int i = 0; i < tasksToRemove; i++) {
		std::string task;
		std::cout << "Write your task: ";
		std::getline(std::cin, task);

		auto it = find(list.begin(), list.end(), task);
		if (it != list.end()) {
			int index = std::distance(list.begin(), it);
			std::cout << "\nTask removed: " << task << std::endl;
			list.erase(it);

			if (index < (int)dateList.size()) {
				dateList.erase(dateList.begin() + index);
			}

			if (index < (int)priorityList.size()) {
				priorityList.erase(priorityList.begin() + index);
			}
			std::cout << "Updated pendent list:" << std::endl;
			showTask(dateList, priorityList, list);
		}
		else {
			std::cout << "\nTask: " << task << " does not exist" << std::endl;
		}

	}
}

void taskCompleted(std::vector<std::string>& dateList, std::vector<std::string>& priorityList, std::vector<std::string>& completedDateList, std::vector<std::string>& pendentList, std::vector<std::string>& completedList) {
	if (pendentList.empty()) {
		std::cout << "There are no pendent tasks" << std::endl;
		return;
	}

	std::cout << "Which task do you want to set as completed?" << std::endl;
	showTask(dateList, priorityList, pendentList);
	std::cout << "Write the task here: ";
	std::string task;
	std::getline(std::cin, task);
	std::cout << std::endl;

	auto it = find(pendentList.begin(), pendentList.end(), task);

	if (it != pendentList.end()) {
		int index = std::distance(pendentList.begin(), it);
		completedList.push_back(*it);
		completedDateList.push_back(current_date());

		pendentList.erase(it);

		if (index < (int)dateList.size()) {
			dateList.erase(dateList.begin() + index);
		}

		if (index < (int)priorityList.size()) {
			priorityList.erase(priorityList.begin() + index);
		}

		std::cout << "Task marked as completed: " << task << std::endl;
	}
	else {
		std::cout << "Task not found: " << task << std::endl;
	}
}

void showCompletedTasks(std::vector<std::string>& dateList, std::vector<std::string>& list) {
	if (list.empty()) {
		std::cout << "No tasks completed" << std::endl;
		return;
	}

	for (int i = 0; i < (int)list.size(); i++) {
		std::string date = (i < (int)dateList.size()) ? dateList[i] : "";
		std::cout << i + 1 << ". " << date << list[i] << std::endl;
	}
}

void removeCompletedTasks(std::vector<std::string>& dateList, std::vector<std::string>& list, int tasksToRemove) {
	for (int i = 0; i < tasksToRemove; i++) {
		std::string task;
		std::cout << "Write your task: ";
		std::getline(std::cin, task);

		auto it = find(list.begin(), list.end(), task);
		if (it != list.end()) {
			int index = std::distance(list.begin(), it);
			std::cout << "\nTask removed: " << task << std::endl;
			list.erase(it);

			if (index < (int)dateList.size()) {
				dateList.erase(dateList.begin() + index);
			}
		}
		else {
			std::cout << "\nTask: " << task << " does not exist" << std::endl;
		}
		std::cout << "Updated completed list:" << std::endl;
		showCompletedTasks(dateList, list);
	}
}

void uploadData(const std::string fileName, std::vector<std::string>& list) {
	std::ifstream file(fileName);
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			if (!line.empty()) {
				list.push_back(line);
			}
		}
		file.close();
	}
}

void saveData(const std::string fileName, std::vector<std::string>& list) {
	std::ofstream file(fileName);

	if (file.is_open()) {
		for (const std::string& task : list) {
			file << task << std::endl;
		}
		file.close();
	}
}

int main() {
	std::vector<std::string> pendentList;
	std::vector<std::string> completedList;
	std::vector<std::string> dateList;
	std::vector<std::string> completedDatesList;
	std::vector<std::string> priorityList;

	static const std::string pendent = "pendent.txt"; //list of pendent tasks
	static const std::string completed = "completed.txt"; //list of completed tasks
	static const std::string dates = "dates.txt"; //list of dates of pendent tasks
	static const std::string completedDates = "completedDates.txt"; //list of dates of completed tasks
	static const std::string priorities = "priorities.txt"; //list of priority of pendent tasks


	Priority priority_obj = static_cast<Priority>(0);

	int option = 0;
	int tasksToAdd = 0;
	int tasksToRemove = 0;
	int looping = 0;

	uploadData(pendent, pendentList);
	uploadData(completed, completedList);
	uploadData(dates, dateList);
	uploadData(completedDates, completedDatesList);
	uploadData(priorities, priorityList);

	do {
		std::string input;

		std::cout << "==========================" << std::endl;
		std::cout << "Choose an option: " << std::endl;
		std::cout << "1. Add task" << std::endl;
		std::cout << "2. Remove task" << std::endl;
		std::cout << "3. Show pendent tasks" << std::endl;
		std::cout << "4. Add completed task" << std::endl;
		std::cout << "5. Show completed tasks" << std::endl;
		std::cout << "6. Remove completed tasks" << std::endl;
		std::cout << "Write your number: ";

		while (true) {
			try {
				std::getline(std::cin, input);
				option = stoi(input);
				if (option < 1 || option > 6) {
					throw std::out_of_range("Invalid number");
				}
				break;
			}
			catch (const std::invalid_argument& e) {
				std::cout << "\nError: write a valid number: ";
			}
			catch (const std::out_of_range& e) {
				std::cout << "\nChoose a number between 1 and 6: ";
			}
		}

		switch (option) {
		case 1:
			std::cout << "==========================" << std::endl;
			std::cout << "\nHow many tasks do you want to add?:";

			while (true) {
				try {
					std::getline(std::cin, input);
					tasksToAdd = stoi(input);
					if (tasksToAdd < 0) {
						throw std::out_of_range("\n");
					}
					addTask(dateList, priorityList, pendentList, tasksToAdd, priority_obj);
					break;
				}
				catch (std::out_of_range& e) {
					if (tasksToAdd < 0 || tasksToAdd == 0) {
						std::cout << "You must add a minimum of 1 task: ";
					}
				}
				catch (...) {
					std::cout << "Write a valid number, not letters or symbols: ";
				}
			}

			saveData(pendent, pendentList);
			saveData(dates, dateList);
			saveData(priorities, priorityList);
			break;

		case 2:
			std::cout << "==========================" << std::endl;

			if (pendentList.empty()) {
				std::cout << "The pendent list is empty" << std::endl;
			}
			else {
				showTask(dateList, priorityList, pendentList);
				std::cout << std::endl;
				std::cout << "\nHow many tasks do you want to remove?: ";
				while (true) {
					try {
						std::getline(std::cin, input);
						tasksToRemove = stoi(input);
						if (tasksToRemove > pendentList.size()) {
							throw std::out_of_range("\n");
						}
						removeTask(dateList, priorityList, pendentList, tasksToRemove);
						break;
					}
					catch (std::out_of_range& e) {
						if (tasksToRemove < 0) {
							std::cout << "Write a number between 1 and " << pendentList.size() << ": ";
						}
						else {
							std::cout << "You can't remove more tasks than exist: ";
						}
					}
					catch (...) {

						std::cout << "Write a valid number, not letters or symbols: ";
					}
				}
			}

			saveData(pendent, pendentList);
			saveData(dates, dateList);
			saveData(priorities, priorityList);
			break;

		case 3:
			std::cout << "==========================" << std::endl;
			std::cout << "\nPendent tasks:" << std::endl;
			showTask(dateList, priorityList, pendentList);
			break;

		case 4:
			std::cout << "==========================" << std::endl;
			taskCompleted(dateList, priorityList, completedDatesList, pendentList, completedList);

			saveData(pendent, pendentList);
			saveData(completed, completedList);
			saveData(dates, dateList);
			saveData(completedDates, completedDatesList);
			saveData(priorities, priorityList);
			break;

		case 5:
			std::cout << "==========================" << std::endl;
			std::cout << "\nCompleted tasks:" << std::endl;
			showCompletedTasks(completedDatesList, completedList);
			break;

		case 6:
			std::cout << "==========================" << std::endl;

			if (completedList.empty()) {
				std::cout << "The completed list is empty" << std::endl;
			}
			else {
				showCompletedTasks(completedDatesList, completedList);
				std::cout << std::endl;
				std::cout << "\nHow many tasks do you want to remove?: ";
				while (true) {
					try {
						std::getline(std::cin, input);
						tasksToRemove = stoi(input);
						if (tasksToRemove > completedList.size()) {
							throw std::out_of_range("\n");
						}
						removeCompletedTasks(completedDatesList, completedList, tasksToRemove);
						break;
					}
					catch (std::out_of_range& e) {
						if (tasksToRemove < 0) {
							std::cout << "Write a number between 1 and " << completedList.size() << ": ";
						}
						else {
							std::cout << "You can't remove more tasks than exist: ";
						}
					}
					catch (...) {

						std::cout << "Write a number, not letters or symbols: ";
					}
				}
			}

			saveData(completed, completedList);
			saveData(completedDates, completedDatesList);
			break;
		}

		std::cout << std::endl;
		std::cout << "==========================" << std::endl;
		std::cout << std::endl;
		std::cout << "Do you want to leave?" << std::endl;
		std::cout << "1. Yes / 2. No: ";

		while (true) {
			try {
				std::getline(std::cin, input);
				looping = stoi(input);
				if (looping < 1 || looping > 2) {
					throw std::out_of_range("\n");
				}
				std::cout << std::endl;
				break;
			}
			catch (const std::invalid_argument& e) {
				std::cout << "Write a valid number: ";
			}
			catch (const std::out_of_range& e) {
				std::cout << "Choose 1 or 2: ";
			}
		}

	} while (looping == 2);

	return 0;
}