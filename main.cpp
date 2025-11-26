#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

class ToDo
{
public:
    bool done;
    std::string title;

    ToDo(std::string n)
    {
        title = n;
        done = false;
    }

    ToDo() {};
};
class ToDoList
{

public:
    std::vector<ToDo> list;

    ToDoList()
    {
        std::ifstream loadListFile("list.txt");
        if (loadListFile.is_open())
        {
            std::string loadListWhole;
            while (std::getline(loadListFile, loadListWhole))
            {
                std::istringstream loadList(loadListWhole);
                std::string loadTodoStatus;
                loadList >> loadTodoStatus;
                std::string loadTodoTitle;
                std::getline(loadList, loadTodoTitle);
                loadTodoTitle.erase(0, 1);
                bool loadTodoStatusBool = (loadTodoStatus == "1");

                ToDo loadedTask;
                loadedTask.title = loadTodoTitle;
                loadedTask.done = loadTodoStatusBool;

                list.push_back(loadedTask);
            }
        }
        else
        {
            loadListFile.close();
        }
    }

    void addTask(ToDo title)
    {
        list.push_back(title);
    }

    void printTasks()
    {
        for (ToDo i : list)
        {
            std::cout << i.title << " " << (i.done ? "Done" : "Not Done") << "\n";
        }
    }

    void markTaskDone(int index)
    {
        list[index].done = true;
    }
};
void twoSecondsSleep()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

int main()
{
    bool taskListModified = false;

    char choice{};
    ToDoList TaskList;

    while (choice != '5')
    {
        system("cls");
        std::cout << "Options: \n 1- Add a task \n 2- Delete a task \n 3- Mark a task as done \n 4- Show all tasks \n 5- Save & exit \n";
        choice = _getch();

        if (choice == '1')
        {

            system("cls");
            if (TaskList.list.size() < 9)
            {
                std::cout << "Enter the task you want to add: ";
                std::string taskName;
                while (true)
                {
                    getline(std::cin, taskName);

                    if (taskName.length() > 0)
                    {
                        break;
                    }
                }

                ToDo Task(taskName);

                TaskList.addTask(Task);
                system("cls");
                std::cout << "Task Added.";
                taskListModified = true;
                twoSecondsSleep();
            }
            else
            {
                std::cout << "You cannot add more than 9 tasks.";
                twoSecondsSleep();
            }
        }

        if (choice == '2')
        {
            system("cls");
            if (TaskList.list.size())
            {
                std::cout << "Delete a task: \n";
                int index = 1;
                for (ToDo s : TaskList.list)
                {
                    std::cout << index << "- " << s.title << "\n";
                    index++;
                }
                int inputDigit{};
                std::cout << "Back to main menu: b \n";
                while (true)
                {
                    char input = _getch();
                    if (std::isdigit(input))
                    {
                        inputDigit = (input - '0');
                    }
                    if (inputDigit <= (index - 1) && inputDigit > 0 || input == 'b')
                    {
                        break;
                    }
                }
                if (inputDigit)
                {
                    TaskList.list.erase(TaskList.list.begin() + (inputDigit - 1));
                    system("cls");
                    std::cout << "Task deleted. ";
                    taskListModified = true;

                    twoSecondsSleep();
                }
            }
            else
            {
                std::cout << "There are no tasks to delete.";
                twoSecondsSleep();
            }
        }

        if (choice == '3')
        {
            system("cls");

            int index = 1;
            char input{};
            std::vector<std::string> undoneList{};

            for (ToDo s : TaskList.list)
            {
                if (s.done == 0)
                {
                    undoneList.push_back(s.title);
                }
            }
            if (undoneList.size() != 0)
            {
                std::cout << "Select the task you want to mark as done: \n";

                for (std::string s : undoneList)
                {
                    std::cout << index << "- " << s << "\n";
                    index++;
                }
                std::cout << "Back to main menu: b \n";
                int inputInt{};
                while (true)
                {
                    input = _getch();
                    inputInt = (input - '0');
                    if (input == 'b')
                    {
                        break;
                    }
                    if (inputInt > 0 && inputInt < index)
                    {
                        for (ToDo &s : TaskList.list)
                        {
                            if (s.title == undoneList[inputInt - 1])
                            {
                                s.done = true;
                                system("cls");
                                std::cout << "Task is marked as done. \n";
                                twoSecondsSleep();
                                break;
                            }
                        }
                        taskListModified = true;

                        break;
                    }
                }
            }
            else if (TaskList.list.size() == 0)
            {
                std::cout << "You have no tasks. \n";
                twoSecondsSleep();
            }
            else
            {
                bool isUndoneEmpty = true;
                for (ToDo s : TaskList.list)
                {
                    if (!s.done)
                        !isUndoneEmpty;
                }
                if (isUndoneEmpty && TaskList.list.size() > 0)
                {

                    std::cout << "You have no undone tasks. \n";
                    twoSecondsSleep();
                }
            }
        }
        if (choice == '4')
        {
            system("cls");

            if (TaskList.list.size() != 0)
            {
                int index = 1;
                char listOption{};
                for (ToDo task : TaskList.list)
                {
                    std::cout << index << "- " << task.title << ": " << (task.done ? "Done" : "Not Done") << "\n";
                    index++;
                }
                std::cout << "Back to main menu: b \n";

                while (true)
                {
                    listOption = _getch();
                    if (listOption == 'b')
                    {
                        break;
                    }
                }
            }
            else
            {
                std::cout << "You have no tasks. \n";
                twoSecondsSleep();
            }
        }
        if (choice == '5')
        {
            if (taskListModified)
            {
                system("cls");
                ToDo checkLoadedFile;
                std::ifstream loadFile("list.txt");
                std::ofstream saveFile("list.txt");
                std::string line;

                for (int i = 0; i < TaskList.list.size(); i++)
                {
                    saveFile << TaskList.list[i].done << " " << TaskList.list[i].title << "\n";
                }
                std::cout << "Changes saved in a file called List.txt.";
                twoSecondsSleep();
            }
            else
            {
                system("cls");
                std::cout << "No changes to save.";
                twoSecondsSleep();
            }
        }
    }
    return 0;
}