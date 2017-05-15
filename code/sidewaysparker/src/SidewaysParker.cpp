
/**
 * sidewaysparker - Sample application for realizing a sideways parking car.
 * Copyright (C) 2012 - 2015 Christian Berger
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
#include <cstdio>
#include <cmath>
#include <iostream>
#include "opendavinci/odcore/io/conference/ContainerConference.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "automotivedata/GeneratedHeaders_AutomotiveData.h"
#include "SidewaysParker.h"
namespace automotive {
    namespace miniature {
        using namespace std;
        using namespace odcore::base;
        using namespace odcore::data;
        using namespace automotive;
        using namespace automotive::miniature;
        VehicleControl vc;
        SidewaysParker::SidewaysParker(const int32_t &argc, char **argv) :
                TimeTriggeredConferenceClientModule(argc, argv, "SidewaysParker") {
        }
        SidewaysParker::~SidewaysParker() {}
        void SidewaysParker::setUp() {
            // This method will be call automatically _before_ running body().
        }
        void SidewaysParker::tearDown() {
            // This method will be call automatically _after_ return from body().
        }
        // This method will do the main data processing job.
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SidewaysParker::body() {
            const double INFRARED_FRONT_RIGHT = 0; //0correct
            const double INFRARED_REAR = 1; //1correct
            const double INFRARED_REAR_RIGHT = 2;
            const double ULTRASONIC_FRONT_CENTER = 3;
            const double ULTRASONIC_FRONT_RIGHT = 4;
            // const double ULTRASONIC_REAR_RIGHT = 5;
            //double distanceOld = 0;
            // int absPathStart = 0;
            // int absPathEnd = 0;
            //double gap_size = 0;
            int stageMoving = 1;
            int hardMoving = 0;
            int sensor_counter = 0;
            int sensor_time = 2;
            int anomaly = 0;
            // char stageMeasuring = 0;
            double gap_size = 0;
            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                // 1. Get most recent vehicle data:
                Container containerVehicleData = getKeyValueDataStore().get(automotive::VehicleData::ID());
                VehicleData vd = containerVehicleData.getData<VehicleData> ();
                // 2. Get most recent sensor board data:
                Container containerSensorBoardData = getKeyValueDataStore().get(automotive::miniature::SensorBoardData::ID());
                SensorBoardData sbd = containerSensorBoardData.getData<SensorBoardData> ();
                cout << sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) << " infrared front right" << endl;
                cout << sbd.getValueForKey_MapOfDistances(INFRARED_REAR) << " infrared rear" << endl;
                cout << sbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) << " infrared rear right" << endl;
                cout << sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) << " ultrasonic front center" << endl;cout << sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT) << " ultrasonic front right" << endl;
                //cout << vd.getAbsTraveledPath() << " Traveled path" << endl;
                if((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 1
                    || sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0)
                   &&
                   (sbd.getValueForKey_MapOfDistances(INFRARED_REAR) > 1
                    || sbd.getValueForKey_MapOfDistances(INFRARED_REAR) < 0)
                   &&
                   (sbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) > 1
                    || sbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) < 0)
                   &&
                   (sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) > 1
                    || sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) < 0)
                   &&
                   (sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT) > 1
                    || sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_RIGHT) < 0)) {
                    // Create vehicle control data.
                    // Moving state machine.
                    if (stageMoving == 1) {
                        // Go forward.
                        //  absPathEnd = vd.getAbsTraveledPath();
                        vc.setSpeed(2);
                        vc.setSteeringWheelAngle(0);
                        //cout << "path end " << absPathEnd << endl;
                        //cout << "path start " << absPathStart << endl;
                        cout << "gap_size  " << gap_size << endl;
                        //cout << "gap size 1 " << gap_size << endl;
                        cout << "sensor counter " << sensor_counter << endl;
                        cout << "sensor time " << sensor_time << endl;
                        if (((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0)
                             ||(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 15))
                            && (sensor_counter < sensor_time)) {
                            //cout << "gap size 2 " << gap_size << endl;
                            sensor_counter++;
                        }
                        else if(sensor_counter == sensor_time){
                            //cout << "gap size 3 " << gap_size << endl;
                            gap_size++;
                            sensor_counter = 0;
                        }
/*
      //New code for finding a gap
                            if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0)
                                && (absPathEnd == (absPathStart + 1))) {
                                gap_size++;
                            }
                            */
                        if (gap_size >= 7
                            && (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0)) {
                            cout << "gap 7 " << endl;
                            cout << "gap size " << gap_size << endl;
                            stageMoving = 2;
                         //   hardMoving = 1;
                        } else if (gap_size >= 14) {
                            cout << "gap 10" << endl;
                            cout << "gap size " << gap_size << endl;
                            stageMoving = 0;
                            hardMoving = 1;
                        }
                        else if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 15)
                                 &&(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 5)
                                 && anomaly < 1){
                            anomaly++;
                        }
                        else if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 15)
                                 &&(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 5)
                                 && anomaly == 1){
                            gap_size = 0;
                            anomaly = 0;
                        }
                        //End of gap finding code
                    }
                    if ((stageMoving > 1) && (stageMoving < 5)) {
                        cout << "gap size " << gap_size << endl;
                        cout << "normal moving " << stageMoving << endl;
                        // Move slightly forward.
                        vc.setSpeed(.4);
                        vc.setSteeringWheelAngle(0);
                        stageMoving++;
                    } else if ((stageMoving == 5)) {
                        cout << "stop normal " << stageMoving << endl;
                        vc.setSpeed(0);
                        vc.setSteeringWheelAngle(0);
                        stageMoving++;
                    }
                    if (stageMoving == 6) {
                        if (((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 5)
                            || (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0))) {
                            vc.setSpeed(-1.3);
                            vc.setSteeringWheelAngle(25);
                            cout << "turn right " << stageMoving << endl;
                        }
                            /*  else if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0)){
                                  vc.setSteeringWheelAngle(0);
                              }*/
                        else {
                            stageMoving++;
                        }
                    }
                    if (stageMoving == 7) {
                        if (((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 1)
                             || (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0))
                            && ((sbd.getValueForKey_MapOfDistances(INFRARED_REAR) > 5)
                                || (sbd.getValueForKey_MapOfDistances(INFRARED_REAR) < 0))) {
                            vc.setSpeed(-0.3);
                            vc.setSteeringWheelAngle(-25);
                            cout << "turn left " << stageMoving << endl;
                            //cout << sbd.getValueForKey_MapOfDistances(INFRARED_REAR) << " infrared " << endl;
                        } else{
                            cout << "stopping " << stageMoving << endl;
                            vc.setSteeringWheelAngle(0);
                            vc.setSpeed(0);
                        stageMoving++;
                        }
                    }

                if(stageMoving == 8 && (sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) > 15)) {
                    vc.setSpeed(2);
                    vc.setSteeringWheelAngle(25);
                }

                else if (stageMoving == 8 && (sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) < 15)){
                    vc.setSpeed(0);
                    vc.setSteeringWheelAngle(0);
                }

                    if (hardMoving >= 1) {
                        if ((hardMoving > 0) && (hardMoving < 5)) {
                            // Move slightly forward.
                            cout << "hardmoving 1" << endl;
                            vc.setSpeed(.4);
                            vc.setSteeringWheelAngle(0);
                            hardMoving++;
                        }
                        if ((hardMoving >= 5) && (hardMoving < 10)) {
                            cout << "hardmoving 2" << endl;
                            // Stop.
                            vc.setSpeed(0);
                            vc.setSteeringWheelAngle(0);
                            hardMoving++;
                        }
                        if ((hardMoving >= 10) && (hardMoving < 45)) {
                            cout << "hardmoving 3" << endl;
                            // Backwards, steering wheel to the right.
                            vc.setSpeed(-1.6);
                            vc.setSteeringWheelAngle(25);
                            hardMoving++;
                        }
                        if ((hardMoving >= 45) && (hardMoving < 80)) {
                            cout << hardMoving <<"hardmoving 4" << endl;
                            // Backwards, steering wheel to the left.
                            vc.setSpeed(-.175);
                            vc.setSteeringWheelAngle(-25);
                            hardMoving++;
                        }
                        if((hardMoving >= 80) && (hardMoving < 90)){
                            cout << "hardmoving 5" << endl;
                            hardMoving++;
                            // Stop.
                            vc.setSpeed(1);
                            vc.setSteeringWheelAngle(25);
                        }

                        else if (hardMoving == 90){
                            vc.setSpeed(0);
                            vc.setSteeringWheelAngle(0);
                        }
                    }
                }
                else{
                    vc.setSpeed(0);
                    vc.setSteeringWheelAngle(0);
                }
/*
                // Measuring state machine.
                    if(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0){
                    gap_size++;
                    if (gap_size >= 7){
                        stageMoving++;
                    }
                    else if (gap_size >= 10){
                        stageMoving = 0;
                        hardMoving = 1;
                    }
                }
                if(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0){
                    gap_size = 0;
                }
                */
                /*
                switch (stageMeasuring) {
                    case 0:
                        {
                            // Initialize measurement.
                            distanceOld = sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT);
                            stageMeasuring++;
                        }
                    break;
                    case 1:
                        {
                            // Checking for sequence +, -.
                            if ((distanceOld > 0) && (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0)) {
                                // Found sequence +, -.
                                stageMeasuring = 2;
                                absPathStart = vd.getAbsTraveledPath();
                                cout << "absPathStart" << absPathStart << endl;
                            }
                            distanceOld = sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT);
                            cout << "distanceOld " <<distanceOld << endl;
                        }
                    break;
                    case 2:
                        {
                            // Checking for sequence -, +.
                            if ((distanceOld < 0) && ( absPathStart - vd.getAbsTraveledPath() >= 0)) {
                                // Found sequence -, +.
                                cout << "TESTING " << endl;
                                stageMeasuring = 1;
                                absPathEnd = vd.getAbsTraveledPath();
                                double const GAP_SIZE = (absPathEnd - absPathStart);
                                cerr << "Size = " << GAP_SIZE << endl;
                                if ((stageMoving < 2) && (GAP_SIZE > 7)) {
                                    stageMoving = 2;
                                }
                                if ((stageMoving == 2) && (GAP_SIZE > 10)) {
                                    stageMoving = 0;
                                    hardMoving = 1;
                                }
                            }
                            distanceOld = sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT);
                        }
                    break;
                }
*/
                // Create container for finally sending the data.
                Container c(vc);
                // Send container.
                getConference().send(c);
            }
            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
        }
    }
} // automotive::miniature
