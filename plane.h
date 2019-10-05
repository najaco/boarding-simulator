//
// Created by Nathan Cohen on 10/4/19.
//

#ifndef BOARDING_PROBLEM_PLANE_H
#define BOARDING_PROBLEM_PLANE_H

#include <mutex>
#include <vector>
using namespace std;


struct Passenger{
 public:
  int id;
  int line_number;
  int seat_number;
  Passenger(int id, int line_number, int seat_number){
    this->id = id;
    this->line_number = line_number;
    this->seat_number = seat_number;
  }
};

class plane{
 private:
  vector<Passenger> passengers;
 public:
  explicit plane(vector<Passenger> passengers);
  bool board();
};




#endif //BOARDING_PROBLEM_PLANE_H
