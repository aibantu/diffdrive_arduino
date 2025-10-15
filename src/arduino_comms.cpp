#include "diffdrive_arduino/arduino_comms.h"
// #include <ros/console.h>
#include <rclcpp/rclcpp.hpp>
#include <sstream>
#include <cstdlib>


void ArduinoComms::setup(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms)
{  
    serial_conn_.setPort(serial_device);
    serial_conn_.setBaudrate(baud_rate);
    serial::Timeout tt = serial::Timeout::simpleTimeout(timeout_ms);
    serial_conn_.setTimeout(tt); // This should be inline except setTimeout takes a reference and so needs a variable
    serial_conn_.open();
    // serial_conn_.(serial_device, baud_rate, serial::Timeout::simpleTimeout(timeout_ms));

}


void ArduinoComms::sendEmptyMsg()
{
    std::string response = sendMsg("\r");
}



void ArduinoComms::readPositionsAndSpeeds(double &pos_1, double &pos_2, double &vel_1, double &vel_2)
{
    std::string response = sendMsg("j\r");

    std::string delimiter = " ";
    size_t del_pos = response.find(delimiter);
    std::string token_1 = response.substr(0, del_pos);
    std::string token_2 = response.substr(del_pos + delimiter.length());

    pos_1 = std::atoi(token_1.c_str());
    pos_2 = std::atoi(token_2.c_str());
}


void ArduinoComms::readPositionsAndSpeeds(double &pos_1, double &pos_2, double &vel_1, double &vel_2)
{
    // 发送指令请求数据（假设"j\r"仍适用，若指令不同需修改）
    std::string response = sendMsg("j\r");

    // 存储解析出的四个数值字符串
    std::vector<std::string> tokens;
    std::string delimiter = " ";
    size_t start = 0;
    size_t end = response.find(delimiter);

    // 循环分割字符串，提取所有token
    while (end != std::string::npos) {
        // 截取从start到end的子串（不包含分隔符）
        tokens.push_back(response.substr(start, end - start));
        // 更新start为下一个字符的位置
        start = end + delimiter.length();
        // 查找下一个分隔符
        end = response.find(delimiter, start);
    }

    // 提取最后一个token（循环结束后剩余的部分）
    tokens.push_back(response.substr(start));

    // 检查是否成功解析出4个数据（容错处理）
    if (tokens.size() != 4) {
        // 解析失败时可设置默认值或报错
        pos_1 = 0.0;
        pos_2 = 0.0;
        vel_1 = 0.0;
        vel_2 = 0.0;
        // 可选：输出错误日志
        // std::cerr << "数据格式错误，期望4个数值，实际得到" << tokens.size() << "个" << std::endl;
        return;
    }

    // 将字符串转换为浮点数（使用std::stod而非atoi，支持小数）
    pos_1 = std::stod(tokens[0]);  // 第一个数值：左轮位置
    pos_2 = std::stod(tokens[1]);  // 第二个数值：右轮位置
    vel_1 = std::stod(tokens[2]);  // 第三个数值：左轮速度
    vel_2 = std::stod(tokens[3]);  // 第四个数值：右轮速度
}

void ArduinoComms::setMotorValues(double val_1, double val_2)
{
    std::stringstream ss;
    ss << "s " << val_1 << " " << val_2 << "\r";
    sendMsg(ss.str(), false);
}

void ArduinoComms::setPidValues(float k_p, float k_d, float k_i, float k_o)
{
    std::stringstream ss;
    ss << "u " << k_p << ":" << k_d << ":" << k_i << ":" << k_o << "\r";
    sendMsg(ss.str());
}

std::string ArduinoComms::sendMsg(const std::string &msg_to_send, bool print_output)
{
    serial_conn_.write(msg_to_send);
    std::string response = serial_conn_.readline();

    if (print_output)
    {
        // RCLCPP_INFO_STREAM(logger_,"Sent: " << msg_to_send);
        // RCLCPP_INFO_STREAM(logger_,"Received: " << response);
    }

    return response;
}