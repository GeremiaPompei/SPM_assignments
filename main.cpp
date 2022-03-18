#include <iostream>
#include "map.cpp"

using namespace std;

template <typename IN, typename OUT>
void log(vector<IN> ins, vector<OUT> outs)
{
    cout << "size: " << ins.size() << endl;
    for (int i = 0; i < ins.size(); i++)
        cout << ins[i] << " -> " << outs[i] << endl;
    cout << endl;
}

int main(int argc, char *argv[])
{
    srand(atoi(argv[1]));

    vector<int> v;
    for (int i = 0; i < atoi(argv[4]); i++)
        v.push_back(rand() % 10 + 1);

    function<int(int)> f = [](int x)  { return x / 2; };
    auto outs = map<int, float>(v, atoi(argv[2]), f, atoi(argv[3]));

    log<int, float>(v, outs);

    return 0;
}