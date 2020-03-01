/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#pragma once
#include <Arduino.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SHICommunicator.h"

// namespace std {
// template <>
// struct hash<String> {
//   std::size_t operator()(String const &s) const noexcept {
//     return std::hash<std::string>{}(s.c_str());
//   }
// };
// }  // namespace std

namespace SHI {

class SDS1306OLED : public Communicator {
 public:
  SDS1306OLED(std::pair<std::string, String> firstRow = {"", ""},
              std::pair<std::string, String> secondRow = {"", ""},
              std::pair<std::string, String> thirdRow = {"", ""})
      : Communicator("SDS1306OLED") {
    displayItems.insert({firstRow.first, 0});
    displayItems.insert({secondRow.first, 1});
    displayItems.insert({thirdRow.first, 2});
    displayLineBuf[0] = firstRow.second;
    displayLineBuf[2] = secondRow.second;
    displayLineBuf[4] = thirdRow.second;
  }
  void setupCommunication() override;
  void loopCommunication() override;
  void newReading(const MeasurementBundle &reading) override;
  void newStatus(const Measurement &status, SHIObject *src) override;
  void setBrightness(uint8_t level);

 private:
  std::unordered_map<std::string, int> displayItems;
  String displayLineBuf[7] = {"", "", "", "", "", "", ""};
};

}  // namespace SHI
