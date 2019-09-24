#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <thread>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>
#include <unistd.h>
#include <condition_variable> // std::condition_variable

using namespace std;
#define MAX_SEATS 2048
#define ROWS 100
#define SEATS_PER_ROW 1

vector<int> passenger_seat_order;
mutex seats[MAX_SEATS];
mutex print_mutex;
mutex step_mutex;

condition_variable cv;

int passengers_seated = 0;
mutex board_mutex;
bool ready = false;

void go_to_seat(int, int, int);
void begin_boarding();

template <typename T>

inline void print_vector(vector<T> v)
{
    std::cout << "[ ";
    for_each(v.begin(), v.end(), [](T i) {
        cout << i << " ";
    });
    std::cout << "]" << std::endl;
}

int main(int argc, char *argv[])
{
    cout << "Boarding problem" << endl;
    clock_t start;
    double duration;

    bool inorder, backwards, random;
    inorder = backwards = random = false;

    int opt;
    while ((opt = getopt(argc, argv, ":if:lrx")) != -1)
    {
        switch (opt)
        {
        case 'i':
            cout << "Inorder" << endl;
            inorder = true;
            break;
        case 'b':
            cout << "Backwards" << endl;
            backwards = true;
            break;
        case 'r':
            cout << "Random" << endl;
            random = true;
            break;
        default:
            inorder = true;
            break;
        }
    }
    for (int i = 0; i < SEATS_PER_ROW; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            passenger_seat_order.push_back(j);
        }
    }

    if (random)
    {
        auto rng = std::default_random_engine{};
        shuffle(passenger_seat_order.begin(), passenger_seat_order.end(), rng);
    }
    else if (backwards)
    {
        reverse(passenger_seat_order.begin(), passenger_seat_order.end());
    }

    start = clock();
    int n_passengers = passenger_seat_order.size();
    thread passengers[n_passengers];
    //block the "door"
    seats[passenger_seat_order.size()].lock();
    print_vector(passenger_seat_order);
    for (int i = 0; i < passenger_seat_order.size(); i++)
    {
        passengers[i] = thread(go_to_seat, passenger_seat_order[i], i, passenger_seat_order[i]);
    }
    begin_boarding();
    for (auto &th : passengers)
        th.join(); //join all threads

    duration = (clock() - start) / (double)CLOCKS_PER_SEC;

    cout << "All threads joined" << endl;
    cout << passengers_seated << " passengers have been seated" << endl;
    cout << "Time: " << duration << endl;

    return 0;
}
void begin_boarding()
{
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "Begin boarding!" << endl;
    seats[passenger_seat_order.size()].unlock();
    ready = true;
    cv.notify_all();
}

void go_to_seat(int passenger_id, int line_number, int seat_number)
{
    // seats[line_number].lock(); //take spot in line
    int position = line_number;
    seats[passenger_seat_order[position]].lock(); //take spot in line
    // while (!ready)
    //     ;
    unique_lock<mutex> lk(board_mutex);
    cv.wait(lk, []{return ready;});
    lk.unlock();
    while (position != seat_number + passenger_seat_order.size())
    {
        // step_mutex.lock();
        seats[position + 1].lock(); //try and take the next row position
        seats[position].unlock();   //unlock the last spot
        // step_mutex.unlock();
        position++;
    }
    seats[position].unlock();
    print_mutex.lock();
    cout << "Passenger : " << passenger_id << " has been seated at seat: " << seat_number << endl;
    passengers_seated++;
    print_mutex.unlock();
}
