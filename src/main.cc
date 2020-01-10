#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>
#include "Plane.h"

using namespace std;
#define MAX_SEATS 2048

template<typename T>
inline void print_vector(vector<T> v) {
  std::cout << "[ ";
  for_each(v.begin(), v.end(), [](T i) { cout << i << " "; });
  std::cout << "]" << std::endl;
}

vector<int> get_order(bool file_flag,
                      string file_name) {
  vector<int> passenger_seat_order;
  int x;
  if (file_flag) {
    fstream file(file_name, ios_base::in);
    while (file >> x) {
      passenger_seat_order.push_back(x);
    }
    file.close();
  } else {
    while (cin >> x) {
      passenger_seat_order.push_back(x);
    }
  }
  
  return passenger_seat_order;
}
void parse_command_line_arguments(int argc, char * argv[], bool * file_flag, string * file_name){
  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    if (arg == "-f") {
      if (i + 1 >= argc) {
        throw "File not provided";
      }
      *file_flag = true;
      *file_name = argv[i + 1];
    }
  }
}

int main(int argc, char *argv[]) {
  cout << "Boarding Problem" << endl;
  
  bool file_flag = false;
  string file_name;
  
  parse_command_line_arguments(argc, argv, &file_flag, &file_name);
  
  vector<int>
      passenger_seat_order = get_order(file_flag, file_name);
  
  vector<Passenger> passengers;
  for (size_t i = 0; i < passenger_seat_order.size(); i++) {
    Passenger passenger(passenger_seat_order[i], i, passenger_seat_order[i]);
    passengers.push_back(passenger);
  }
  
  Plane p(passengers);
  p.set_verbose(1); // Set to 1 for number of passengers seated message
  
  
  clock_t start = clock();
  p.board();
  
  long double duration = (clock() - start) / (long double) CLOCKS_PER_SEC;
  cout << "Time: " << duration << endl;
  
  return 0;
}

