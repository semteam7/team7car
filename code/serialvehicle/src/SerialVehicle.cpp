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

#include <string>
#include <memory>

#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>

#include "SerialVehicle.h"

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::data::image;
using namespace automotive;

using namespace odcore;
using namespace odcore::wrapper;

    SerialVehicle::SerialVehicle(const int &argc, char **argv)
    : DataTriggeredConferenceClientModule(argc, argv, "serialvehicle") {}
SerialVehicle::~SerialVehicle() {}

void SerialVehicle::setUp() {
    // Attach to serial port
    // We are using OpenDaVINCI's std::shared_ptr to automatically
    // release any acquired resources.

    try {
        cout << "Connected to serial port ttyACM0" << endl;
    }
    catch(string &exception) {
        cerr << exception << endl;
        exit(0);
    }

}

void SerialVehicle::tearDown() {
    // Release serial port
}

void SerialVehicle::nextContainer(odcore::data::Container &c) {
    m_serial = std::shared_ptr<SerialPort>(SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    m_serial->setStringListener(this);
    m_serial->start();

    if (c.getDataType() == VehicleControl::ID()) {
            vc_count++;
            if(vc_count % 10 == 0)
            {
                VehicleControl vc = c.getData<VehicleControl> ();

                string command = std::to_string(vc.getSpeed())+ ";" + std::to_string(vc.getSteeringWheelAngle()) + "\r\n";
                m_serial->send(command);
                cout << "Sent command: " << command;
            }
        }

    m_serial->stop();
    m_serial->setStringListener(NULL);
}

void SerialVehicle::nextString(const string &s)
{
    cout << "Received " << s.length() << " bytes: " << s << endl;
}

}
} // scaledcars::perception
