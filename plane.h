//
// Created by Nathan Cohen on 10/4/19.
//

#ifndef BOARDING_PROBLEM_PLANE_H
#define BOARDING_PROBLEM_PLANE_H

#include <mutex>
#include <vector>
#define MAX_SEATS 2048

using namespace std;


struct Passenger{
 public:
  int id;
  int position;
  int seat_number;
  Passenger(int id, int position, int seat_number){
    this->id = id;
    this->position = position;
    this->seat_number = seat_number;
  }
};

class plane{
 private:
  vector<Passenger> passengers;
  mutex line[MAX_SEATS];
  mutex board_mutex;
  condition_variable cv;
  std::atomic_uint passengers_seated;
  bool ready;
 
 public:
  explicit plane(vector<Passenger> passengers);
  unsigned int board();
 private:
  void go_to_seat(Passenger);
  void begin_boarding();
};




#endif //BOARDING_PROBLEM_PLANE_H
