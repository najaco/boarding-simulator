//
// Created by Nathan Cohen on 1/10/20.
//

#ifndef BOARDING_PROBLEM_PASSENGER_H
#define BOARDING_PROBLEM_PASSENGER_H
class Passenger {
public:
  int id;
  int position;
  int seat_number;

  Passenger(int id, int position, int seat_number) {
    this->id = id;
    this->position = position;
    this->seat_number = seat_number;
  }
};
#endif // BOARDING_PROBLEM_PASSENGER_H
