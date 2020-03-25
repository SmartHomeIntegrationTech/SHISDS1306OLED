/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include "SHISDS1306OLED.h"

#include <Arduino.h>
#include <oled/SSD1306Wire.h>

#include "SHIHardware.h"

namespace {
SSD1306Wire display =
    SSD1306Wire(0x3c, SDA_OLED, SCL_OLED, RST_OLED, GEOMETRY_128_64);
bool displayUpdated = false;
}  // namespace

void SHI::SDS1306OLED::setupCommunication() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  reconfigure();
  display.display();
  SHI::hw->feedWatchdog();
}

void SHI::SDS1306OLED::loopCommunication() {
  if (displayUpdated) {
    displayUpdated = false;
    display.clear();
    for (int i = 0; i < 6; i += 2)
      display.drawString(0, (i / 2) * 13, displayLineBuf[i]);
    for (int i = 1; i < 6; i += 2)
      display.drawString(90, (i / 2) * 13, displayLineBuf[i]);
    display.drawStringMaxWidth(0, 3 * 13, 128, displayLineBuf[6]);
    display.display();
  }
}

void SHI::SDS1306OLED::newReading(const SHI::MeasurementBundle &reading) {
  static int lastBrightness = -1;
  const std::string baseName = reading.src->getName();
  for (auto &&data : reading.data) {
    auto sensorName = baseName + data.getMetaData()->getName();
    if (sensorName == "APDS9960Luminosity") {
      int value = data.getFloatValue();
      int newBrightness = 0;
      if (value > 500) {
        newBrightness = 10;
      } else if (value > 250) {
        newBrightness = 5;
      } else if (value > 100) {
        newBrightness = 2;
      } else {
        newBrightness = 0;
      }
      if (newBrightness != lastBrightness) {
        display.setBrightness(newBrightness);
        SHI_LOGINFO(std::string("Auto adjust brightness to ") +
                    String(newBrightness).c_str());
      }
      lastBrightness = newBrightness;
    }
    auto value = displayItems.find(sensorName);
    if (value != displayItems.end()) {
      displayLineBuf[(value->second * 2) + 1] =
          String(data.toTransmitString().c_str());
      displayUpdated = true;
    }
  }
}

void SHI::SDS1306OLED::newStatus(const Measurement &status, SHIObject *src) {
  if (status.stringRepresentation != STATUS_OK) {
    displayLineBuf[6] = String(status.stringRepresentation.c_str());
    displayUpdated = true;
  }
}

void SHI::SDS1306OLED::setBrightness(uint8_t level) {
  display.setBrightness(level);
}

bool SHI::SDS1306OLED::reconfigure() {
  display.drawString(0, 0, String(config.bootUpText.c_str()));
  display.setBrightness(config.defaultBrightness);
  displayItems.clear();
  displayItems.insert({config.firstRowName, 0});
  displayItems.insert({config.secondRowName, 1});
  displayItems.insert({config.thirdRowName, 2});
  displayLineBuf[0] = String(config.firstRowUnit.c_str());
  displayLineBuf[2] = String(config.secondRowUnit.c_str());
  displayLineBuf[4] = String(config.thirdRowUnit.c_str());
  return true;
}
