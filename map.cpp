#include <iostream>
#include <future>
#include <thread>
#include <functional>
#include <vector>
#include "utimer.cpp"

using namespace std;

template <typename IN>
vector<vector<pair<int, IN>>> split(vector<IN> v, int mode, int pardegree)
{
    vector<vector<pair<int, IN>>> splitted(pardegree);
    for (int i = 0; i < v.size(); i++)
    {
        int index = mode ? i % pardegree : i * pardegree / v.size();
        splitted[index].push_back({i, v[i]});
    }
    return splitted;
}

template <typename IN, typename OUT>
vector<pair<int, OUT>> compute(vector<pair<int, IN>> v, function<OUT(IN)> f, int index)
{
    utimer u("Thread: " + to_string(index+1));
    vector<pair<int, OUT>> outs{};
    for (pair<int, IN> p_in : v)
    {
        pair<int, OUT> p_out{p_in.first, f(p_in.second)};
        outs.push_back(p_out);
    }
    return outs;
}

template <typename OUT>
vector<OUT> merge(vector<vector<pair<int, OUT>>> splitted, int size)
{
    vector<OUT> outs(size);
    for (auto part : splitted)
        for (auto pair : part)
            outs[pair.first] = pair.second;
    return outs;
}

template <typename IN>
void log_splitted(vector<vector<pair<int, IN>>> splitted)
{
    cout << "------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < splitted.size(); i++)
    {
        cout << "Thread " << i + 1 << ": [";
        for (auto c : splitted[i])
        {
            cout << c.first << ",";
        }
        cout << "]" << endl;
    }
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << endl;
}

template <typename IN, typename OUT>
vector<OUT> map(vector<IN> v, int mode, function<OUT(IN)> func, int pardegree)
{
    vector<shared_future<vector<pair<int, OUT>>>> futures{};
    vector<vector<pair<int, OUT>>> outs{};
    vector<vector<pair<int, IN>>> splitted = split<IN>(v, mode, pardegree);
    log_splitted(splitted);
    for (int i = 0; i < splitted.size(); i++)
    {
        shared_future<vector<pair<int, OUT>>>
            future = async(launch::async, compute<IN, OUT>, splitted[i], func, i);
        futures.push_back(future);
    }
    for (int i = 0; i < splitted.size(); i++)
    {
        outs.push_back(futures[i].get());
    }
    return merge<OUT>(outs, v.size());
}