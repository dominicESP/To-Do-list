# To-Do List Project

This is the largest project I have built as a junior C++ learner.

---

## Overview

This application is a simple task manager that allows users to add, remove, and track both pending and completed tasks.

Each task includes:
- Timestamp of creation (`current_time`)
- Priority level (`taskPriority`)

---

## Features

- Add and remove tasks
- Display pending and completed tasks
- Persistent storage using `.txt` files
- Automatic loading of saved data on startup
- Automatic updates when tasks are modified

---

## Data storage

All tasks are stored in text files.
On startup, the program reads the files and reconstructs the task list in the original order.

---

## How to run (Visual Studio 2022/2026)

1. Download the source files
2. Create a new C++ project in Visual Studio
3. Right-click on **Source Files** in Solution Explorer
4. Select **Add > Existing Item...**  
5. Import the downloaded files
6. Build and run the project

---

## Future improvements

- Transition from console-based to graphical desktop application (currently in progress)
- Add task categories (e.g. Work, Study, Personal)
- Improve UI and user experience
