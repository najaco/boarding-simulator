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
  long double duration;
  
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
    Passenger passenger(passenger_seat_order[i], i, passenger_seat_order[i]);
    passengers.push_back(passenger);
  }
#if PRINT
  print_vector(passenger_seat_order);
#endif
  plane p(passengers);
  unsigned int passengers_seated;
  start = clock();
  passengers_seated = p.board();
  duration = (clock() - start) / (long double) CLOCKS_PER_SEC;
  cout << "All threads joined" << endl;
  cout << passengers_seated << " passengers have been seated" << endl;
  cout << "Time: " << duration << endl;
  
  return 0;
}

