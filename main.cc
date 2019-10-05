#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>
#include <atomic>
#include <unistd.h>
#include <condition_variable> // std::condition_variable
#include "plane.h"

using namespace std;
#define MAX_SEATS 2048
#define ROWS 1000
#define SEATS_PER_ROW 1
#define PRINT 0

vector<int> passenger_seat_order;
mutex seats[MAX_SEATS];
mutex print_mutex;
mutex step_mutex;

condition_variable cv;

//atomic_uint passengers_seated = 0;
unsigned int passengers_seated = 0;
mutex board_mutex;
bool ready = false;

void go_to_seat(Passenger);
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

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "-i")
        {
            cout << "Inorder" << endl;
            inorder = true;
        }
        else if (arg == "-b")
        {
            cout << "Backwards" << endl;
            backwards = true;
        }
        else if (arg == "-r")
        {
            cout << "Random" << endl;
            random = true;
            break;
        }
    }
    if (!backwards && !random)
    {
        inorder = true;
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
    
    
    vector<Passenger> passengers;
    for(size_t i = 0; i < passenger_seat_order.size(); i++){
      Passenger p(passenger_seat_order[i], i, passenger_seat_order[i]);
      passengers.push_back(p);
    }
#if PRINT
  print_vector(passenger_seat_order);
#endif
    
    
    start = clock();
    int n_passengers = passenger_seat_order.size();
    thread passengers_threads[n_passengers];
    //block the "door"
    seats[passenger_seat_order.size()].lock();

    for (size_t i = 0; i < passengers.size(); i++)
    {
        passengers_threads[i] = thread(go_to_seat, passengers[i]);
    }
    begin_boarding();
    for (auto &th : passengers_threads)
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

void go_to_seat(Passenger p)
{
    // seats[line_number].lock(); //take spot in line
    int position = p.line_number;
    seats[passenger_seat_order[position]].lock(); //take spot in line
    // while (!ready)
    //     ;
    unique_lock<mutex> lk(board_mutex);
    cv.wait(lk, [] { return ready; });
    lk.unlock();
    while (position != p.seat_number + passenger_seat_order.size())
    {
        seats[position + 1].lock(); //try and take the next row position
        seats[position].unlock();   //unlock the last spot
        this_thread::sleep_for(chrono::nanoseconds(1));
        position++;
    }
    seats[position].unlock();
#if PRINT
    print_mutex.lock();
    cout << "Passenger : " << p.id << " has been seated at seat: " << p.seat_number << endl;
    print_mutex.unlock();
#endif
    passengers_seated++;
}
