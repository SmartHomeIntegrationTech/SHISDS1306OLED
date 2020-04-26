/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

// WARNING, this is an automatically generated file!
// Don't change anything in here.
// Last update 2020-04-25

# include <iostream>
# include <string>


# include "SHISDS1306OLED.h"
// Configuration implementation for class SHI::SDS1306OLEDConfig

namespace {
    
}  // namespace

SHI::SDS1306OLEDConfig::SDS1306OLEDConfig(const JsonObject &obj):
      firstRowName(obj["firstRowName"] | ""),
      firstRowUnit(obj["firstRowUnit"] | ""),
      secondRowName(obj["secondRowName"] | ""),
      secondRowUnit(obj["secondRowUnit"] | ""),
      thirdRowName(obj["thirdRowName"] | ""),
      thirdRowUnit(obj["thirdRowUnit"] | ""),
      bootUpText(obj["bootUpText"] | "OLED initial done!"),
      rstOLED(static_cast<uint8_t>(obj["rstOLED"].as<int>() | 16)),
      sclOLED(static_cast<uint8_t>(obj["sclOLED"].as<int>() | 15)),
      sdaOLED(static_cast<uint8_t>(obj["sdaOLED"].as<int>() | 4)),
      defaultBrightness(obj["defaultBrightness"] | 5)
  {}

void SHI::SDS1306OLEDConfig::fillData(JsonObject &doc) const {
    doc["firstRowName"] = firstRowName;
  doc["firstRowUnit"] = firstRowUnit;
  doc["secondRowName"] = secondRowName;
  doc["secondRowUnit"] = secondRowUnit;
  doc["thirdRowName"] = thirdRowName;
  doc["thirdRowUnit"] = thirdRowUnit;
  doc["bootUpText"] = bootUpText;
  doc["rstOLED"] = rstOLED;
  doc["sclOLED"] = sclOLED;
  doc["sdaOLED"] = sdaOLED;
  doc["defaultBrightness"] = defaultBrightness;
}

int SHI::SDS1306OLEDConfig::getExpectedCapacity() const {
  return JSON_OBJECT_SIZE(11);
}

