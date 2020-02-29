/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include <Arduino.h>
#include <oled/SSD1306Wire.h>

#include "SHIHardware.h"
#include "SHISDS1306OLED.h"

namespace {
SSD1306Wire display =
    SSD1306Wire(0x3c, SDA_OLED, SCL_OLED, RST_OLED, GEOMETRY_128_64);
bool displayUpdated = false;
}  // namespace

void SHI::SHISDS1306OLED::setupCommunication() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "OLED initial done!");
  display.setBrightness(5);
  display.display();
  SHI::hw->feedWatchdog();
}

void SHI::SHISDS1306OLED::loopCommunication() {
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

void SHI::SHISDS1306OLED::newReading(const SHI::MeasurementBundle &reading) {
  const std::string baseName = reading.src->getName();
  for (auto &&data : reading.data) {
    auto sensorName = baseName + data.getMetaData()->getName();
    auto value = displayItems.find(sensorName);
    if (value != displayItems.end()) {
      displayLineBuf[(value->second * 2) + 1] =
          String(data.toTransmitString().c_str());
      displayUpdated = true;
    }
  }
}

void SHI::SHISDS1306OLED::newStatus(const Measurement &status, SHIObject *src) {
  if (status.stringRepresentation != STATUS_OK) {
    displayLineBuf[6] = String(status.stringRepresentation.c_str());
    displayUpdated = true;
  }
}

void SHI::SHISDS1306OLED::setBrightness(uint8_t level) {
  display.setBrightness(level);
}
