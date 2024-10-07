/*
 * Filename:     EyeTracker.cpp
 * Author:       Mohammadhossein Salari
 * Email:        mohammadhossein.salari@gmail.com
 * Last Modified: 2024/10/05
 * Description:  Implementation of the EyeTracker class for Tobii Eye Trackers.
 *               Handles data collection, license validation, and interaction
 *               with the Tobii SDK.
 */

#include "EyeTracker.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

const std::string KEY = "int.lab2024";
const std::string EXPIRATION_DATE = "2024/12/31";

std::chrono::system_clock::time_point parseExpirationDate(const std::string& expirationDate) {
  std::tm t = {};
  std::istringstream ss(expirationDate);
  ss >> std::get_time(&t, "%Y/%m/%d");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse expiration date");
  }
  // Convert std::tm to time_t and then to time_point
  time_t timeT = std::mktime(&t);
  if (timeT == -1) {
    throw std::runtime_error("Failed to convert time_t");
  }
  return std::chrono::system_clock::from_time_t(timeT);
}

EyeTracker::EyeTracker(float w, float h, float off, const std::string& lic)
    : width(w), height(h), offset(off), isRecording(false), license(lic) {
  if (!isLicenseValid()) {
    throw std::runtime_error("Invalid or expired license");
  }

  intlib = IL::UniqueInteractionLibPtr(IL::CreateInteractionLib(IL::FieldOfUse::Interactive));
  intlib->CoordinateTransformAddOrUpdateDisplayArea(width, height);
  intlib->CoordinateTransformSetOriginOffset(offset, offset);

  intlib->SubscribeGazePointData(
      [](IL::GazePointData evt, void* context) {
        auto* self = static_cast<EyeTracker*>(context);
        if (self->isRecording && self->isLicenseValid()) {
          self->data.push_back(evt);
        }
      },
      this);
}

void EyeTracker::start() {
  if (isLicenseValid()) {
    isRecording = true;
  } else {
    throw std::runtime_error("Cannot start: Invalid or expired license");
  }
}

void EyeTracker::stop() {
  isRecording = false;
  intlib.reset();
}

void EyeTracker::pause() { isRecording = false; }

std::vector<IL::GazePointData> EyeTracker::getData() const {
  if (isLicenseValid()) {
    return data;
  } else {
    throw std::runtime_error("Cannot get data: Invalid or expired license");
  }
}

void EyeTracker::update() {
  if (isLicenseValid()) {
    intlib->WaitAndUpdate();
  } else {
    throw std::runtime_error("Cannot update: Invalid or expired license");
  }
}

bool EyeTracker::isLicenseValid() const {
  auto now = std::chrono::system_clock::now();
  auto expirationDate = parseExpirationDate(EXPIRATION_DATE);

  return (license == KEY) && (now < expirationDate);
}

void EyeTracker::clearDataBuffer() { data.clear(); }

EyeTracker::~EyeTracker() {
  // Cleanup is handled automatically by UniqueInteractionLibPtr
}