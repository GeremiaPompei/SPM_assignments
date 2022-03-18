#include <iostream>
#include <chrono>

using namespace std;

class utimer {
    private:

    chrono::time_point<chrono::system_clock> start;
    chrono::time_point<chrono::system_clock> stop;
    string message;

    public:

    utimer(string m): message(m)
    {
        start = chrono::system_clock::now();
    }

    ~utimer()
    {
        stop = chrono::system_clock::now();
        double ms = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        cout << message << ": " << ms << "ms" << endl;
    }
};