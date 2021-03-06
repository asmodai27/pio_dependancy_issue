/* Copyright 2020, RespiraWorks
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**********************************************************************************************
 * Adapted from Arduino PID Library - Version 1.2.1 - under MIT License
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * Original project hash: 9b4ca0e5b6d7bab9c6ac023e249d6af2446d99bb
 **********************************************************************************************/

#include "pid.h"
#include <algorithm>

float PID::Compute(int now, float input, float setpoint) {
  if (!initialized_) {
    last_input_ = input;
    last_error_ = setpoint - input;
    last_update_time_ = now;
    output_sum_ = 0;
    initialized_ = true;
  }

  // Compute time between now and last sample.
  float dt = (now - last_update_time_);

  // Compute all the working error variables
  float error = setpoint - input;
  float dInput = input - last_input_;

  output_sum_ += (ki_ * error * dt);

  if (p_term_ == ProportionalTerm::ON_MEASUREMENT) {
    output_sum_ -= kp_ * dInput;
  }

  float res = output_sum_;
  if (p_term_ == ProportionalTerm::ON_ERROR) {
    res += kp_ * error;
  }
  // dt may be 0 (e.g. on the first call to Compute()), in which case we simply
  // skip using the derivative term.
  if (dt > 0) {
    if (d_term_ == DifferentialTerm::ON_MEASUREMENT) {
      res -= kd_ * dInput / dt;
    } else {
      res += kd_ * (error - last_error_) / dt;
    }
  }

  // Remember some variables for next time
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;

  return res;
}

void PID::Observe(int now, float input, float setpoint, float actual_output) {
  // All the observable variables are updated the same way as in Compute();
  last_input_ = input;
  last_error_ = setpoint - input;
  last_update_time_ = now;
}
