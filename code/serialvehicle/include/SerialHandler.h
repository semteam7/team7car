//
// Created by jpp on 25/04/17.
//

#ifndef SCALEDCARS_SOFTWARE_SERIALPORT_H
#define SCALEDCARS_SOFTWARE_SERIALPORT_H

#include <stdint.h>
#include <iostream>

#include <string>

#include "opendavinci/odcore/io/conference/ContainerConference.h"
#include "opendavinci/odcore/data/Container.h"


#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>
#include <opendavinci/odcore/io/StringListener.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

#include "odvdscaledcarsdatamodel/GeneratedHeaders_ODVDScaledCarsDataModel.h"


namespace odcore { namespace io { namespace conference { class ContainerConference; } } }

namespace scaledcars {
    namespace team7 {

        using namespace std;
        using namespace automotive::miniature;
        using namespace odcore;
        using namespace odcore::data;
        using namespace odcore::io::conference;
        using namespace chalmersrevere::scaledcars;

        class SerialHandler : public odcore::io::StringListener
        {

        private:
            SerialHandler (const SerialHandler &) = delete;
            SerialHandler &operator=(const SerialHandler & ) = delete;

        public:
            SerialHandler (odcore::io::conference::ContainerConference &);
            stringbuf m_receive_sb;
            iostream m_receive_buffer;
            RawSensorBoardData m_sensorboard_data;
            ContainerConference &m_conference;
            string m_received;
            virtual void nextString(const string &s);
            int m_odo_last_value;
            int m_odo_wrap_count;
        };
    }
}

#endif //SCALEDCARS_SOFTWARE_SERIALPORT_H
