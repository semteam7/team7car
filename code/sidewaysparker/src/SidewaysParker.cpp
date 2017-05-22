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

            //Declaration of sensors
            const double INFRARED_FRONT_RIGHT = 0;
            const double ODOMETER = 6;

            //const double INFRARED_REAR = 1;
            //const double INFRARED_REAR_RIGHT = 2;
            //const double ULTRASONIC_FRONT_CENTER = 3;

            //Variable declarations
            int gap_start = 0;  // Start of gap
            int stageMoving = 1; // Initial state, default to 1 == driving
            int hardMoving = 0; // Hardcoded parking, sets to one to start
            int gap_found = 0; // Keeps track if a gap is found. 0 == no_gap
            int distance_traveled = 0; // current calue of odometer


            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                // 1. Get most recent vehicle data:
                Container containerVehicleData = getKeyValueDataStore().get(automotive::VehicleData::ID());
                VehicleData vd = containerVehicleData.getData<VehicleData> ();
                // 2. Get most recent sensor board data:
                Container containerSensorBoardData = getKeyValueDataStore().get(automotive::miniature::SensorBoardData::ID());
                SensorBoardData sbd = containerSensorBoardData.getData<SensorBoardData> ();


                // Create vehicle control data.
                    // Moving state machine.

                    if (stageMoving == 1) {
                        // Go forward.
                        
                        vc.setSpeed(2);
                        vc.setSteeringWheelAngle(0);
                        distance_traveled = sbd.getValueForKey_MapOfDistances(ODOMETER);  //Sets distance_traveled to current odometer value

                        

                        if (((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0)
                             ||(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 20))
                                && (gap_found == 0)){
                            //Gap found, sensor counter is set to 1 save the odometer value.

                            gap_start = sbd.getValueForKey_MapOfDistances(ODOMETER); //Sets gap start to current value of odometer
                            gap_found = 1;

                        }


                        if (((distance_traveled - gap_start) >= 140 && (distance_traveled - gap_start) < 180)
                            && (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0)) {
                            // Compares current value of odometer to the beginning of the gap.
                            // If an obstacle were to be found in this stage stagemoving should be set to one to use the sensors.
                            // However, that implementation does not work properly.
                            stageMoving = 0;
                            hardMoving = 1;

                        } else if ((distance_traveled -  gap_start) >= 180) {
                           // No obstacles encountered, start hardcoded parking
                            stageMoving = 0;
                            hardMoving = 1;
                        }

                        else if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 20)
                                 &&(sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 5)){
                            // Obstacle found, set counter to 0;
                            // Allows values between 0-4 to make upp for smoothing algorithm issues.

                            gap_start = 0;
                            gap_found = 0;
                        }
                        //End of gap finding code
                    }

                    if (hardMoving >= 1) {
                        if ((hardMoving > 0) && (hardMoving < 5)) {
                            // Move slightly forward.
                            
                            vc.setSpeed(.4);
                            vc.setSteeringWheelAngle(0);
                            hardMoving++;
                        }
                        if ((hardMoving >= 5) && (hardMoving < 10)) {
                            // Stop.
                            
                            vc.setSpeed(0);
                            vc.setSteeringWheelAngle(0);
                            hardMoving++;
                        }
                        if ((hardMoving >= 10) && (hardMoving < 55)) {
                            // Backwards, steering wheel to the right.
                            
                            vc.setSpeed(-1.6);
                            vc.setSteeringWheelAngle(25);
                            hardMoving++;
                        }
                        if ((hardMoving >= 55) && (hardMoving < 85)) {
                            // Backwards, steering wheel to the left.
                            
                            vc.setSpeed(-.175);
                            vc.setSteeringWheelAngle(-25);
                            hardMoving++;
                        }

                        if((hardMoving >= 85) && (hardMoving < 110)){
                            // Drive forward to the right adjusting.

                            hardMoving++;
                            vc.setSpeed(1);
                            vc.setSteeringWheelAngle(25);

                        }

                        else if (hardMoving == 110){
                            //Stop.
                            
                            vc.setSpeed(0);
                            vc.setSteeringWheelAngle(0);
                        }
                    }

                /*
                 //Code that uses sensors to Park. Works from time to time.


                    if ((stageMoving > 1) && (stageMoving < 5)) {
                        //cout << "gap size " << gap_size << endl;
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
                        if ((sbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) < 0)
                               || (sbd.getValueForKey_MapOfDistances(INFRARED_REAR_RIGHT) > 20) )
                           {
                            vc.setSpeed(-1.3);
                            vc.setSteeringWheelAngle(25);
                            cout << "turn wheels right " << stageMoving << endl;
                        }
                              else if ((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 0)){
                                  vc.setSteeringWheelAngle(0);
                              }
                    else {
                        stageMoving++;
                    }
            }
            if (stageMoving == 7) {
                if (((sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) > 1)
                     || (sbd.getValueForKey_MapOfDistances(INFRARED_FRONT_RIGHT) < 0))
                    && ((sbd.getValueForKey_MapOfDistances(INFRARED_REAR) > 10)
                        || (sbd.getValueForKey_MapOfDistances(INFRARED_REAR) < 0))) {
                    vc.setSpeed(-0.3);
                    vc.setSteeringWheelAngle(-25);
                    cout << "turn wheels left " << stageMoving << endl;
                    //cout << sbd.getValueForKey_MapOfDistances(INFRARED_REAR) << " infrared " << endl;
                } else{
                    cout << "stopping " << stageMoving << endl;
                    vc.setSteeringWheelAngle(0);
                    vc.setSpeed(0);
                    stageMoving++;
                }
            }


            if(stageMoving == 8) {
                if((sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) > 15)
                   || (sbd.getValueForKey_MapOfDistances(ULTRASONIC_FRONT_CENTER) < 0)) {
                    vc.setSpeed(2);
                    vc.setSteeringWheelAngle(25);
                }
                else{
                    stageMoving++;
                }}

            if (stageMoving == 9) {
                vc.setSpeed(0);
                vc.setSteeringWheelAngle(0);
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



