#ifndef DIFFDRIVE_ARDUINO_WHEEL_H
#define DIFFDRIVE_ARDUINO_WHEEL_H

#include <string>



class Wheel
{
    public:

    std::string name = "";
    // int enc = 0;
    double cmd = 0;   //待发送的速度指令
    double pos = 0;
    double vel = 0;
    double eff = 0;
    double velSetPt = 0;
    // double rads_per_count = 0;

    Wheel() = default;

    Wheel(const std::string &wheel_name);

    void setup(const std::string &wheel_name);

    double calcEncAngle();



};


#endif // DIFFDRIVE_ARDUINO_WHEEL_H