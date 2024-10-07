/*
 * Filename:     eyetracker_wrapper.cpp
 * Author:       Mohammadhossein Salari
 * Email:        mohammadhossein.salari@gmail.com
 * Last Modified: 2024/10/05
 * Description:  Python binding for the EyeTracker class using pybind11.
 *               Provides a Python interface to interact with the Tobii
 *               Eye Trackers functionality implemented in C++.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "EyeTracker.h"

namespace py = pybind11;

PYBIND11_MODULE(tobii, m) {
  py::class_<EyeTracker>(m, "EyeTracker")
      .def(py::init<float, float, float, const std::string&>())
      .def("start", &EyeTracker::start)
      .def("pause", &EyeTracker::pause)
      .def("stop", &EyeTracker::stop)
      .def("getData", &EyeTracker::getData)
      .def("update", &EyeTracker::update)
      .def("isLicenseValid", &EyeTracker::isLicenseValid)
      .def("clearDataBuffer", &EyeTracker::clearDataBuffer)
      .def("getFormattedData", [](const EyeTracker& et) {
        auto data = et.getData();
        py::list formatted_data;
        for (const auto& point : data) {
          py::dict point_dict;
          point_dict["x"] = point.x;
          point_dict["y"] = point.y;
          point_dict["validity"] = point.validity == IL::Validity::Valid ? "Valid" : "Invalid";
          point_dict["timestamp_us"] = point.timestamp_us;
          formatted_data.append(point_dict);
        }
        return formatted_data;
      });

  py::class_<IL::GazePointData>(m, "GazePointData")
      .def_readwrite("x", &IL::GazePointData::x)
      .def_readwrite("y", &IL::GazePointData::y)
      .def_readwrite("validity", &IL::GazePointData::validity)
      .def_readwrite("timestamp_us", &IL::GazePointData::timestamp_us);

  py::enum_<IL::Validity>(m, "Validity")
      .value("Valid", IL::Validity::Valid)
      .value("Invalid", IL::Validity::Invalid)
      .export_values();
}