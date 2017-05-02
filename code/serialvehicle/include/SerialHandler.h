//
// Created by jpp on 25/04/17.
//

#ifndef SCALEDCARS_SOFTWARE_SERIALPORT_H
#define SCALEDCARS_SOFTWARE_SERIALPORT_H

#include <stdint.h>
#include <iostream>

#include <string>

#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>
#include <opendavinci/odcore/io/StringListener.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>

namespace scaledcars {
    namespace team7 {

        using namespace std;

        class SerialHandler : public odcore::io::StringListener
        {

        private:
            SerialHandler (const SerialHandler &) = delete;
            SerialHandler &operator=(const SerialHandler & ) = delete;

        public:
            SerialHandler();
            stringbuf m_receive_sb;
            iostream m_receive_buffer;
            virtual void nextString(const string &s);
        };
    }
}

#endif //SCALEDCARS_SOFTWARE_SERIALPORT_H
