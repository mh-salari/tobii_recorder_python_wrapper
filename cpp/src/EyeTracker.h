/*
 * Filename:     EyeTracker.h
 * Author:       Mohammadhossein Salari
 * Email:        mohammadhossein.salari@gmail.com
 * Last Modified: 2024/08/19
 * Description:  Header file for the EyeTracker class. Defines the interface
 *               for interacting with the Tobii Eye Trackers, including
 *               data collection and management functions.
 */

#pragma once

#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>

#include <chrono>
#include <string>
#include <vector>

class EyeTracker {
 private:
  IL::UniqueInteractionLibPtr intlib;
  float width;
  float height;
  float offset;
  std::vector<IL::GazePointData> data;
  bool isRecording;
  std::string license;
  std::chrono::system_clock::time_point expirationDate;

 public:
  EyeTracker(float w, float h, float off, const std::string& lic);
  void start();
  void stop();
  void pause();
  std::vector<IL::GazePointData> getData() const;
  void update();
  bool isLicenseValid() const;
  ~EyeTracker();
};