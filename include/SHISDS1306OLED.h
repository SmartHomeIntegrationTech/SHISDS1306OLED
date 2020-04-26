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
#include "oled/SSD1306Wire.h"

// namespace std {
// template <>
// struct hash<String> {
//   std::size_t operator()(String const &s) const noexcept {
//     return std::hash<std::string>{}(s.c_str());
//   }
// };
// }  // namespace std

namespace SHI {

class SDS1306OLEDConfig : public Configuration {
 public:
  SDS1306OLEDConfig() {}
  explicit SDS1306OLEDConfig(const JsonObject &obj);
  std::string firstRowName = "";
  std::string firstRowUnit = "";
  std::string secondRowName = "";
  std::string secondRowUnit = "";
  std::string thirdRowName = "";
  std::string thirdRowUnit = "";
  std::string bootUpText = "OLED initial done!";
  uint8_t rstOLED = 16;
  uint8_t sclOLED = 15;
  uint8_t sdaOLED = 4;
  int defaultBrightness = 5;
  void fillData(
      JsonObject &obj) const override;  // NOLINT Yes, non constant reference
 protected:
  int getExpectedCapacity() const override;
};

class SDS1306OLED : public Communicator {
 public:
  SDS1306OLED(std::pair<std::string, std::string> firstRow = {"", ""},
              std::pair<std::string, std::string> secondRow = {"", ""},
              std::pair<std::string, std::string> thirdRow = {"", ""})
      : Communicator("SDS1306OLED") {
    config.firstRowName = firstRow.first;
    config.firstRowUnit = firstRow.second;
    config.secondRowName = secondRow.first;
    config.secondRowUnit = secondRow.second;
    config.thirdRowName = thirdRow.first;
    config.thirdRowUnit = thirdRow.second;
  }
  explicit SDS1306OLED(const SDS1306OLEDConfig &config)
      : Communicator("SDS1306OLED"), config(config) {}
  void setupCommunication() override;
  void loopCommunication() override;
  void newReading(const MeasurementBundle &reading) override;
  void newStatus(const Measurement &status, SHIObject *src) override;
  void setBrightness(uint8_t level);

  const Configuration *getConfig() const { return &config; }
  bool reconfigure(Configuration *newConfig) {
    config = castConfig<SDS1306OLEDConfig>(newConfig);
    return reconfigure();
  }
  SDS1306OLEDConfig config;

 private:
  bool reconfigure();
  std::unordered_map<std::string, int> displayItems;
  String displayLineBuf[7] = {"", "", "", "", "", "", ""};
  SSD1306Wire display;
  bool displayUpdated = false;
};

}  // namespace SHI
