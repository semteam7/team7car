/**
 * Example - Example code.
 * Copyright (C) 2016 Christian Berger
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdint.h>

#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>

#include <opendavinci/GeneratedHeaders_OpenDaVINCI.h>
#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

#include "SerialVehicle.h"

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::data::image;

SerialVehicle::SerialVehicle(const int &argc, char **argv)
    : DataTriggeredConferenceClientModule(argc, argv, "serialvehicle"){}

SerialVehicle::~SerialVehicle() {}

void SerialVehicle::setUp() {
    // Attach to serial port
    cout << "setup" << endl;
}

void SerialVehicle::tearDown() {
    // Release serial port
    cout << "teardown" << endl;
}

void SerialVehicle::nextContainer(odcore::data::Container &c) {
    if (c.getDataType() == odcore::data::image::SharedImage::ID()) {
            cout << "woop" << endl;
        }
    }
}
} // scaledcars::perception
