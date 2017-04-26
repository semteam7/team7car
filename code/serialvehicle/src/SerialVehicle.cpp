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

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>

#include <opendavinci/GeneratedHeaders_OpenDaVINCI.h>
#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

#include <string>
#include <memory>

#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>

#include "SerialVehicle.h"
#include "SerialHandler.h"

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
            : DataTriggeredConferenceClientModule(argc, argv, "serialvehicle"),
              m_serial(SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE)),
              m_serialHandler(),
              m_connected_at(chrono::system_clock::now()){}

SerialVehicle::~SerialVehicle() {}

void SerialVehicle::setUp() {
    m_serial->setStringListener(&m_serialHandler);
    m_serial->start();
}

void SerialVehicle::tearDown() {
    m_serial->stop();
    m_serial->setStringListener(NULL);
}

void SerialVehicle::nextContainer(odcore::data::Container &c) {
    cout << "next container" << endl;
    if (c.getDataType() == VehicleControl::ID()) {
            vc_count++;
            cout << "Message count: " <<vc_count << endl;
            if(vc_count % 10 == 0)
            {
                VehicleControl vc = c.getData<VehicleControl> ();

                stringstream cs;
                cs << setprecision(2) << vc.getSpeed() << ";" << vc.getSteeringWheelAngle() << "\r\n";
                string command = cs.str();

                m_serial->send(command);
                cout << "Sent command: " << command;
            }
        }

}

void SerialVehicle::reconnect()
{
    m_serial.reset(SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));

    m_serial->setStringListener(&m_serialHandler);
    m_serial->start();
}

void SerialVehicle::nextString(const string &s)
{
    cout << "Received " << s.length() << " bytes: " << s << endl;
}

}
} // scaledcars::perception
