//
// Created by jpp on 25/04/17.
//

#include <stdint.h>
#include <iostream>

#include <string>
#include "SerialHandler.h"

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>


namespace scaledcars {
namespace team7 {

    using namespace std;

    using namespace odcore;
    using namespace odcore::wrapper;

    using namespace automotive;
    using namespace scaledcars;

    SerialHandler::SerialHandler():
        m_receive_sb(),
        m_receive_buffer(&m_receive_sb){}

    void SerialHandler::nextString(const string &s){
        cout << "Received : " << s.size() << "'" << s << "'" << endl;

//        m_receive_buffer << s;
//        if(m_receive_buffer.tellp() > 66){ //attempt to read the buffer
//            IRData ird;
//            USData usd;
//
//            ird << m_receive_buffer;
//            usd << m_receive_buffer;
//
//            m_receive_sb = stringbuf();
//            m_receive_buffer = iostream(&m_receive_sb);
//        }

    }
}
}