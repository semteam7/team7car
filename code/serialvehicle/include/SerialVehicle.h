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

#ifndef SERIALVEHICLE_H
#define SERIALVEHICLE_H

#include <memory>

#include <opencv/cv.h>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

#include <string>
#include <memory>

#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>
#include <chrono>

#include "SerialHandler.h"

#define CONNECTION_DELAY 1000

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace odcore;
using namespace odcore::wrapper;


    class SerialVehicle : public odcore::base::module::DataTriggeredConferenceClientModule{
   private:
    SerialVehicle(const SerialVehicle & /*obj*/) = delete;
    SerialVehicle &operator=(const SerialVehicle & /*obj*/) = delete;

   public:
    /**
     * Constructor.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    SerialVehicle(const int &argc, char **argv);
    virtual ~SerialVehicle();
    virtual void nextContainer(odcore::data::Container &c);
    virtual void nextString(const string &s);

   private:
    void setUp();
    void tearDown();
    void reconnect();
    const string   SERIAL_PORT =  "/dev/ttyACM0";
    const uint32_t BAUD_RATE = 115200;
    std::shared_ptr<SerialPort> m_serial;
//  VehicleControl vc_buffer[];
    SerialHandler m_serialHandler;
    int vc_count = 0;
    const chrono::time_point<chrono::system_clock> m_connected_at;
};
}
} // scaledcars::perception

#endif /*SERIALVEHICLE_H*/
