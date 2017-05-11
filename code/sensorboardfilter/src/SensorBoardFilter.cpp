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

#include "SensorBoardFilter.h"

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::data::image;

SensorBoardFilter::SensorBoardFilter(const int &argc, char **argv)
    : DataTriggeredConferenceClientModule(argc, argv, "sensorboardfilter"),
    m_sensorboard_data(){}

SensorBoardFilter::~SensorBoardFilter() {}

void SensorBoardFilter::setUp() {
    // Attach to serial port
    cout << "setup" << endl;
}

void SensorBoardFilter::tearDown() {
    // Release serial port
    cout << "teardown" << endl;
}
/**
Read sensorboarddata from serial, start with collecting the last 10 messages,
calculate the average value and pass this on to the waiting movement module

Test if 10 is too many or too few messages in correlation with performance
*/
void SensorBoardFilter::nextContainer(odcore::data::Container &c) {
    if (c.getDataType() == RawSensorBoardData::ID()) {


            rsbd_count++;
            cout << "Message count: " <<rsbd_count << endl;
            RawSensorBoardData rsbd = c.getData<RawSensorBoardData>();

            cout << "rsbd usFC: " << rsbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) << endl;
            cout << "rsbd usFR: " << rsbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT) << endl;
            cout << "rsbd irFR: " << rsbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) << endl;
            cout << "rsbd irRR: " << rsbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) << endl;
            cout << "rsbd irR:  " << rsbd.getValueForKey_MapOfDistances(INFRARED_REAR) << endl;

            usFrontcenter = rsbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER);
            if(usFrontcenter > 59){
              usFrontcenter = 0;
            }
            usFrontright = rsbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT);
            if(usFrontright > 59){
              usFrontright = 0;
            }
            irFrontright = rsbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT);
            irRearright = rsbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT);
            irRear = rsbd.getValueForKey_MapOfDistances(INFRARED_REAR);

          //  if(rsbd_count%3 == 0){

                values[3] = getKalmanValue(usFrontcenter, 3);
                values[4] = getKalmanValue(usFrontright, 4);
                values[0] = getKalmanValue(irFrontright, 0);
                values[2] = getKalmanValue(irRearright, 2);
                values[1] = getKalmanValue(irRear, 1);


                m_sensorboard_data.setNumberOfSensors(6);
                std::map<uint32_t, double> distances{
                      {0, values[0]},
                      {1, values[1]},
                      {2, values[2]},
                      {3, values[3]},
                      {4, values[4]},
                      {5, usFrontcenter}
                };

                m_sensorboard_data.setMapOfDistances(distances);
                Container c2(m_sensorboard_data);
                getConference().send(c2);


                cout << "usFC " << m_sensorboard_data.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) << endl;
                cout << "usFR " << m_sensorboard_data.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT) << endl;
                cout << "irFR " << m_sensorboard_data.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) << endl;
                cout << "irRR " << m_sensorboard_data.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) << endl;
                cout << "irR  " << m_sensorboard_data.getValueForKey_MapOfDistances(INFRARED_REAR) << endl;
             //}
        }
    }
}
} // scaledcars::perception
