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

#ifndef SENSORBOARDFILTER_H
#define SENSORBOARDFILTER_H

#include <memory>

#include <opencv/cv.h>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/wrapper/SharedMemory.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace automotive::miniature;
using namespace odcore;
using namespace odcore::data;
using namespace odcore::io::conference;

class SensorBoardFilter : public odcore::base::module::DataTriggeredConferenceClientModule {
   private:
    SensorBoardFilter(const SensorBoardFilter & /*obj*/) = delete;
    SensorBoardFilter &operator=(const SensorBoardFilter & /*obj*/) = delete;

   public:
    /**
     * Constructor.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    SensorBoardFilter(const int &argc, char **argv);

    virtual ~SensorBoardFilter();
    SensorBoardData m_sensorboard_data;

    virtual void nextContainer(odcore::data::Container &c);

   private:
    void setUp();
    void tearDown();
    int rsbd_count = 0;
    int rsbd_limit = 10;
    double usFrontcenter = 0;
    double usFrontright = 0;
    double irFrontright = 0;
    double irRearright = 0;
    double irRear = 0;
    const double INFRARED_FRONT_RIGHT = 0; //0correct
    const double INFRARED_REAR = 1; //1correct
    const double INFRARED_REAR_RIGHT = 2;
    const double ULTRASONIC_FRONT_CENTER = 3;
    const double ULTRASONIC_FRONT_RIGHT = 4;
    double values[5];
};
}
} // scaledcars::perception

#endif /*SENSORBOARDFILTER_H*/
