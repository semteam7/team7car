
#ifndef SENSORBOARDFILTER_H
#define SENSORBOARDFILTER_H

#include <memory>

#include <opencv/cv.h>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/wrapper/SharedMemory.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>
#include <odvdscaledcarsdatamodel/GeneratedHeaders_ODVDScaledCarsDataModel.h>

namespace scaledcars {
namespace team7 {

using namespace std;
using namespace automotive::miniature;
using namespace odcore;
using namespace odcore::data;
using namespace odcore::io::conference;
using namespace chalmersrevere::scaledcars;

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

    //Kalman components

    //output value
    double x = 0;
    //previous values
    double usFCx = 0;
    double usFRx = 0;
    double irFRx = 0;
    double irRRx = 0;
    double irRx = 0;
    double kg=0; //kalman gain
    double p=1023; //estimation error
    //Fiddle with these
    double q=0.250; // process noise
    double r=4; //sensor noise

    double getKalmanValue(double measurement, int sensor){
      //prediction part
      p = p + q;

      //measurement part
      kg = p / (p+r);
      if(sensor == 0){
          x = irFRx = irFRx + kg * (measurement - irFRx);
      }else if(sensor == 1){
          x = irRx = irRx + kg * (measurement - irRx);
      }else if(sensor == 2){
          x = irRRx = irRRx + kg * (measurement - irRRx);
      }else if(sensor == 3){
          x = usFCx = usFCx + kg * (measurement - usFCx);
      }else if(sensor == 4){
          x = usFRx = usFRx + kg * (measurement - usFRx);
      }
      p = (1 - kg) * p;

      return x;
    }

};
}
} // scaledcars::perception

#endif /*SENSORBOARDFILTER_H*/
