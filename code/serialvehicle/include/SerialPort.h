//
// Created by jpp on 25/04/17.
//

#ifndef SCALEDCARS_SOFTWARE_SERIALPORT_H
#define SCALEDCARS_SOFTWARE_SERIALPORT_H

namespace scaledcars {
    namespace team7 {
        class SerialPort
        {

            using namespace std;
        private:
            SerialPort(const SerialPort &) = delete;
            SerialPort &operator=(const SerialPort & ) = delete;

        public:
            SerialPort(string serial_port, int baud_rate);

        private:
            initialize_serial_connection();


            string serial_port;
            uint32_t baud_rate;

            std::deque buf_in;
            std::deque buf_out;
        };
    }
}

#endif //SCALEDCARS_SOFTWARE_SERIALPORT_H
