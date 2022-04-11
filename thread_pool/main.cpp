#include <iostream>
#include "./thread_pool.cpp"

using namespace std;

bool isNumber(const string &str)
{
    for (char const &c : str)
    {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}

int todoList(ThreadPool *tp)
{
    vector<string> todo_list;
        while(true) {
            string new_task = "";
            getline(cin, new_task);
            function<void()> task;
            int num = atoi(new_task.c_str());
            if(new_task == "exit") {
                return 0;
            } else if(isNumber(new_task) && todo_list.size() > num) {
                task = [&todo_list, num]() {
                    todo_list.erase(todo_list.begin() + num);
                };
            } else {
                task = [&todo_list, new_task]() {
                    todo_list.push_back(new_task);
                };
            }
            tp->run(task);
            tp->run([&todo_list]() {
                for (int i = 0; i < todo_list.size(); i++)
                    {
                        cout << i << ": " << todo_list[i] << endl;
                    }
            });
        }
}

int main(int argc, char *argv[])
{
    int thread_pool_size = argc > 1 ? atoi(argv[1]) : 2;
    ThreadPool *tp = new ThreadPool(thread_pool_size);

    return todoList(tp);
}