//
// Created by Nathan Cohen on 10/4/19.
//
#include <iostream>
#include <thread>
#include "plane.h"
#define STEP_TIME 100
mutex progress_mutex;
plane::plane(vector<Passenger> passengers) {
  this->passengers = passengers;
  ready = false;
  passengers_seated = 0;
  this->verbose = 0;
}
unsigned int plane::board() {
  vector<thread> passengers_threads;
  // block the "door"
  line[this->passengers.size()].lock();
  
  for (auto p : passengers) {
    passengers_threads.emplace_back(thread(&plane::go_to_seat, this, p));
  }
  plane::begin_boarding();
  for (auto &th : passengers_threads)
    th.join(); // join all threads
  printf("\n");
  return passengers_seated;
}

void plane::go_to_seat(Passenger p) {
  line[p.position].lock(); // take spot in line
  unique_lock<mutex> lk(board_mutex);
  cv.wait(lk);
  lk.unlock();
  while (p.position != p.seat_number + this->passengers.size()) {
    line[p.position + 1].lock(); // try and take the next row position
    line[p.position].unlock(); // unlock the last spot
    this_thread::sleep_for(chrono::nanoseconds(STEP_TIME));
    p.position++;
  }
  line[p.position].unlock();
  progress_mutex.lock();
  passengers_seated++;
  if(this->verbose >= 1) {
    printf("\rPassengers Seated: %d/%lu", passengers_seated, passengers.size());
  }
  progress_mutex.unlock();
}

void plane::begin_boarding() {
  this_thread::sleep_for(chrono::milliseconds(1000));
  line[this->passengers.size()].unlock();
  ready = true;
  cv.notify_all();
}
