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
        m_conference(conference),
        m_received(){}

    void SerialHandler::nextString(const string &s){
        char reading[128];
        cout << "getting serial data: " << s << endl;
        //cout << "buffer is " << m_receive_buffer << endl;
        m_received += s;
        int start = m_received.find('S');
        int end = m_received.find('\n', start);
        cout << "Start is " << start << endl;
        cout << "End is " << end << endl;
        if( start > -1 && end > start)
        {
            if((end - start) < 5)
            {
                cout << "wrong message length, skipping" << endl;
                return;
            }

            strcpy(reading, m_received.substr(start, end - start).c_str());
            cout << "Reading " << reading << endl;
            int values[5];

            for (int i=0;i<5;i++){
                values[i] = ((int) reading[i+1]) - 31;
            }

            m_sensorboard_data.setNumberOfSensors(5);
            std::map<uint32_t, double> distances{
                    {0, values[0]},
                    {1, values[2]},
                    {2, values[1]},
                    {3, values[3]},
                    {4, values[4]}
            };
            m_sensorboard_data.setMapOfDistances(distances);
            Container c(m_sensorboard_data);
            m_conference.send(c);


            m_received = "";
        }
        else
        {
            cout << "start and end not found in " << m_received << endl ;
        }
    }
}
}
