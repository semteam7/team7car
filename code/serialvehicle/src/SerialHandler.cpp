//
// Created by jpp on 25/04/17.
//

#include <stdint.h>
#include <iostream>

#include <string>
#include <cstring>
#include "SerialHandler.h"

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

#include "opendavinci/odcore/io/conference/ContainerConference.h"
#include "opendavinci/odcore/data/Container.h"
namespace odcore { namespace io { namespace conference { class ContainerConference; } } }

namespace scaledcars {
namespace team7 {

    using namespace std;

    using namespace odcore;
    using namespace odcore::wrapper;
    using namespace odcore::base;
    using namespace odcore::data;
    using namespace odcore::io::conference;

    using namespace automotive;
    using namespace automotive::miniature;
    using namespace scaledcars;


    SerialHandler::SerialHandler(odcore::io::conference::ContainerConference &conference):
        m_receive_sb(),
        m_receive_buffer(&m_receive_sb),
        m_sensorboard_data(),
        m_conference(conference){}

    void SerialHandler::nextString(const string &s){
        cout << "Received : " << s.size() << "'" << s << "'" << endl;
        char reading[128];
        m_receive_buffer << s;

        if(m_receive_buffer.tellp() > 0 ){ //attempt to read the buffer
            m_receive_buffer.getline(reading, 128, '\n');

            double values[5];
            int val_count;
            char *val = std::strtok(reading, ":");
            while(val){
                values[val_count] = atof(val);
                val_count++;
                strtok(NULL, ":");
            }

            m_sensorboard_data.setNumberOfSensors(5);
            std::map<uint32_t, double> distances{
                {1, values[0]},
                {2, values[1]},
                {3, values[2]},
                {4, values[3]},
                {5, values[4]}
            };
            m_sensorboard_data.setMapOfDistances(distances);

            Container c(m_sensorboard_data);
            m_conference.send(c);
        }

    }
}
}