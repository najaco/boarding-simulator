//
// Created by Nathan Cohen on 10/4/19.
//
#include "Plane.h"
#include <iostream>
#include <thread>
#define STEP_TIME 1

mutex progress_mutex;
Plane::Plane(vector<Passenger> passengers) {
  this->passengers = passengers;
  passengers_seated = 0;
  this->verbose = 0;
}

unsigned int Plane::board() {
  vector<thread> passengers_threads;

  line[this->passengers.size()]
      .lock(); // Lock position in array so no passenger can begin movement

  for (auto p : passengers) {
    passengers_threads.emplace_back(thread(&Plane::go_to_seat, this, p));
  }

  Plane::begin_boarding();
  for (auto &th : passengers_threads)
    th.join(); // Must wait for threads to finish before continuing
  // If any messages were used, must print new line character
  if (this->verbose >= 1) {
    cout << endl;
  }
  return passengers_seated;
}

void Plane::go_to_seat(Passenger p) {
  line[p.position].lock(); // take spot in line
  unique_lock<mutex> lk(
      board_mutex); // unique lock that is shared across all threads
  cv.wait(lk);      // each passenger must wait for the gate to be opened
  lk.unlock();
  while (p.position != p.seat_number + this->passengers.size()) {
    line[p.position + 1].lock(); // try and take the next row position
    line[p.position].unlock();   // allow someone to take previous spot
    this_thread::sleep_for(
        chrono::nanoseconds(STEP_TIME)); // sleep to force a context switch
    p.position++;
  }
  line[p.position].unlock();
  increment_passengers_seated();
}

void Plane::begin_boarding() {
  this_thread::sleep_for(chrono::milliseconds(1000));
  line[this->passengers.size()].unlock(); // unlock gate
  cv.notify_all();                        // wake all threads
}

inline void Plane::increment_passengers_seated() {
  progress_mutex.lock();
  passengers_seated++;
  if (this->verbose >= 1) {
    printf("\rPassengers Seated: %d/%lu", passengers_seated, passengers.size());
  }
  progress_mutex.unlock();
}
