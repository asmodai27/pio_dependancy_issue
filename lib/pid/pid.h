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
 * Adapted from Arduino PID Library - Version 1.2.1 under MIT License
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * Original project hash: 9b4ca0e5b6d7bab9c6ac023e249d6af2446d99bb
 **********************************************************************************************/

#ifndef PID_H
#define PID_H

enum class ProportionalTerm {
  ON_ERROR,
  ON_MEASUREMENT,
};

enum class DifferentialTerm {
  ON_ERROR,
  ON_MEASUREMENT,
};

class PID {
public:
  // Constructs the PID using the given parameters.
  PID(float kp, float ki, float kd, ProportionalTerm p_term,
      DifferentialTerm d_term, float output_min, float output_max)
      : kp_(kp), ki_(ki), kd_(kd), p_term_(p_term), d_term_(d_term),
        out_min_(output_min), out_max_(output_max) {}

  // Performs one step of the PID calculation.
  float Compute(int now, float input, float setpoint);

  // Call this instead of Compute in case on this step of the control loop
  // you intend apply different control logic instead of the PID.
  // "actual_output" contains the value of output you plan to act on.
  //
  // This is a variation on the "manual" mode:
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-onoff/
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-initialization/
  void Observe(int now, float input, float setpoint, float actual_output);

  void SetKP(float kp) { kp_ = kp; }
  void SetKI(float ki) { ki_ = ki; }
  void SetKD(float kd) { kd_ = kd; }

  void Reset() { initialized_ = false; }

private:
  float kp_; // * (P)roportional Tuning Parameter
  float ki_; // * (I)ntegral Tuning Parameter
  float kd_; // * (D)erivative Tuning Parameter

  const ProportionalTerm p_term_;
  const DifferentialTerm d_term_;

  const float out_min_;
  const float out_max_;

  bool initialized_ = false;
  int last_update_time_ = 0;
  float output_sum_ = 0;
  float last_input_ = 0;
  float last_error_ = 0;
};
#endif // PID_H
