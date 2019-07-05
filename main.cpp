/*
// TOP VALUE WITHOUT THREADS
#include <iostream>
#include <thread>
#include <vector>
// Generic vector
template<typename T>
T maxAt(std::vector<T>& vector_name) {
    T max = 0;
    for (auto val = vector_name.rbegin(); val!=vector_name.rend(); val++)
    {
        if (max < *val) max = *val;

    }
    return max;
}

int main() {
    std::vector<double> vector_name {9, 1, 42.5, 32.2, 45.1, 2, 4, 45};
    std::cout<< maxAt(vector_name)<< std::endl;
}

*/
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <future>
using namespace std;
mutex mtx;
void atMax(vector<int>& v1, int start, int stop, promise<int> p) {
    int max = 0;
    try {
        for (int i = start; i < stop; i++)
            if (max < i) max = v1[i];
        p.set_value(max);
    }
    catch (...) {
        p.set_exception(current_exception());
    }
    lock_guard<mutex> lock(mtx);
}
int main() {

    try {
        vector<int> v1 = {4, 5, 6, 1, 8, 9, 10};
        promise<int> prm;
        promise<int> prm1;
        promise<int> prm2;
        promise<int> prm3;
        future<int> ftr = prm.get_future();
        future<int> ftr1 = prm1.get_future();
        future<int> ftr2 = prm2.get_future();
        future<int> ftr3 = prm3.get_future();
        thread t1(atMax, ref(v1), 0, 7,  move(prm));
        t1.join();
        cout << ftr.get();
        thread t2(atMax, ref(v1), 0, 7,  move(prm1));
        t2.join();
        cout << ftr1.get();
        thread t3(atMax, ref(v1), 0, 7,  move(prm2));
        t3.join();
        cout << ftr2.get();
        thread t4(atMax, ref(v1), 0, 7,  move(prm3));
        t4.join();
        cout << ftr3.get();
    }
    catch (exception& e) {
        cout << e.what();
    }
    return 0;
}