#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>
#include <atomic>
#include <fstream>
#include <unistd.h>
#include <condition_variable> // std::condition_variable
#include "plane.h"

using namespace std;
#define MAX_SEATS 2048
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
  
  bool file_flag, backwards, random;
  file_flag = backwards = random = false;
  string file_name;
  for (int i = 1; i < argc; i++)
  {
    string arg = argv[i];
    if(arg == "-f"){
      if(i+1 >= argc){
        cout << "Error. No file provided." << endl;
        return 1;
      }
      file_flag = true;
      file_name = argv[i + 1];
    }
    else if (arg == "-b")
    {
      backwards = true;
    }
    else if (arg == "-r")
    {
      random = true;
      break;
    }
  }
  int x;
  if(file_flag){
    fstream file(file_name, ios_base::in);
    while(file >> x){
      passenger_seat_order.push_back(x);
    }
    file.close();
  }else{
    while(cin >> x){
      passenger_seat_order.push_back(x);
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

