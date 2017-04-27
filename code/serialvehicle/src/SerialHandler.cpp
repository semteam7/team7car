//
// Created by jpp on 25/04/17.
//

#include <stdint.h>
#include <iostream>

#include <string>
#include "SerialHandler.h"

namespace scaledcars {
namespace team7 {

    using namespace std;

    using namespace odcore;
    using namespace odcore::wrapper;

    SerialHandler::SerialHandler(){}

    void SerialHandler::nextString(const string &s){
        cout << "Received : " << s.size() << "'" << s << "'" << endl;
    }
}
}