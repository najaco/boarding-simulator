//
// Created by Nathan Cohen on 10/4/19.
//

#ifndef BOARDING_PROBLEM_PLANE_H
#define BOARDING_PROBLEM_PLANE_H

#include "Passenger.h"
#include <mutex>
#include <vector>
#define MAX_SEATS 2048

using namespace std;

class Plane {
private:
  vector<Passenger> passengers;
  mutex line[MAX_SEATS];
  mutex board_mutex;
  condition_variable cv;
  unsigned int passengers_seated;
  int verbose;

public:
  explicit Plane(vector<Passenger> passengers);
  unsigned int board();
  void set_verbose(int verbose) { this->verbose = verbose; }

private:
  void go_to_seat(Passenger);
  void begin_boarding();
  inline void increment_passengers_seated();
};

#endif // BOARDING_PROBLEM_PLANE_H
