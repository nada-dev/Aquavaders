#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include "std_msgs/Float64.h"
#include <std_msgs/Bool.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <string>

#define motorA motor_def_D
#define motorB motor_def_A
#define motorC motor_def_E
#define motorD motor_def_B
#define motorE motor_def_C
#define motorF motor_def_F

std::string ph = "P0";

int motorA = 1500, motorB = 1500, motorC = 1500, motorD= 1500, motorE= 1500, motorF= 1500;
bool led_button = 0, gripper_button = 0;
std_msgs::String msg;
std::stringstream ss;


void forwardBackward(const sensor_msgs::Joy::ConstPtr &joy)
{
    motorA = 1500 + joy->axes[1]/32767.0 * 300;
    motorB = 1500 + joy->axes[1]/32767.0 * 300;
    motorC = 1500 + joy->axes[1]/32767.0 * 300;
    motorD = 1500 + joy->axes[1]/32767.0 * 300;
}

void rightLeft(const sensor_msgs::Joy::ConstPtr &joy)
{
    motorA = 1500 - joy->axes[0]/32767.0 * 300;
    motorB = 1500 + joy->axes[0]/32767.0 * 300;
    motorC = 1500 - joy->axes[0]/32767.0 * 300;
    motorD = 1500 + joy->axes[0]/32767.0 * 300;
}

void upDown(const sensor_msgs::Joy::ConstPtr &joy)
{
    motorE = 1500 + joy->axes[2]/32767.0 * 300;
    motorF = 1500 + joy->axes[2]/32767.0 * 300;
}


void concatenate_values(){
    ss << "A" << motor_def_A << "B" <<  motor_def_B << "C" <<  motor_def_C
     << "D" <<  motor_def_D << "E" <<  motor_def_E << "F" <<  motor_def_F 
     << "G" << gripper_button << "L" << led_button << "@";

    msg.data = ss.str();
}

void Buttons(const sensor_msgs::Joy::ConstPtr &joy)
{
    if(joy->buttons[8]) led_button = 1;
    else led_button = 0;

    if(joy->buttons[9]) gripper_button = 1;
    else gripper_button = 0;
}

class mapper
{
public:
    void joyCallback(const sensor_msgs::Joy::ConstPtr &joy){//
        if (joy->axes[0] != 0) //Tilting
        {
            forwardBackward(joy);
            ROS_INFO_STREAM("normal forward backward");
        }
        else if(joy->axes[1] != 0)
        {
            rightLeft(joy);
            ROS_INFO_STREAM("normal right left");
        }
        else if(joy->axes[2] != 0){
            upDown(joy);
            ROS_INFO_STREAM("normal up down");
        }
        else motorA = 1500, motorB = 1500, motorC = 1500, motorD= 1500, motorE= 1500, motorF= 1500;

        concatenate_values();
        rovpub.publish(msg);
    
    }

    ros::NodeHandle nh;
    ros::Publisher rovpub;
    ros::Subscriber joysub;

    mapper(){
        rovpub = nh.advertise<std_msgs::String>("motor_values", 1000);
        joysub = nh.subscribe<sensor_msgs::Joy>("joy", 1000, &mapper::joyCallback, this);
    }
};



/*void mapper::joyCallBack(const sensor_msgs::Joy::ConstPtr &joy){//
    if (joy->axes[0] != 0) //Tilting
    {
        forwardBackward(joy);
        ROS_INFO_STREAM("normal forward backward");
    }
    else if(joy->axes[1] != 0)
    {
        rightLeft(joy);
        ROS_INFO_STREAM("normal right left");
    }
    else if(joy->axes[2] != 0){
        upDown(joy);
        ROS_INFO_STREAM("normal up down");
    }
    else motorA = 1500, motorB = 1500, motorC = 1500, motorD= 1500, motorE= 1500, motorF= 1500;

    concatenate_values();
        rovpub.publish(msg);
    
}
*/

void chatterCallback(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "map_motors");
    ROS_INFO_STREAM("Executing main");
    mapper m();


    ros::NodeHandle nh("~");
    nh.getParam("ph", ph);
    ROS_INFO("Got parameter: %s", ph.c_str());

    ros::spin();
    return 0;
}
