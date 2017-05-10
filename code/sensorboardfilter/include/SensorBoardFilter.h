
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
    double x=0; //output value
    double q=4; // process noise
    double p=1023; //estimation error
    double r=2; //measurement noise
    double kg=0; //kalman gain

    double getKalmanValue(double measurement){
      p = p + q;

      kg = p / (p+r);
      x = x + kg * (measurement - x);
      p = (1 - kg) * p;

      return x;
    }

};
}
} // scaledcars::perception

#endif /*SENSORBOARDFILTER_H*/
