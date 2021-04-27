 #include <ros/ros.h>
 #include <serial/serial.h>
 #include <std_msgs/Byte.h>
 #include <std_msgs/UInt8.h>
 #include <std_msgs/String.h>
 #include <std_msgs/Empty.h>
 
 serial::Serial ser;
 std_msgs::String result;
 
 void write_callback(const std_msgs::String::ConstPtr& msg){
     //ROS_INFO_STREAM("Writing to serial port" << msg->data);
     if(ser.available()){
         ROS_INFO_STREAM("Reading from serial port in callback");
             result.data = ser.readline(65536, "$");
             ROS_INFO_STREAM("Result data " << result.data);
     }
     
     ser.write(msg->data);
     ser.flush();

 }
 
 int main (int argc, char** argv){
     ros::init(argc, argv, "ros_serial_node");
     ros::NodeHandle nh;
 
     ros::Subscriber write_sub = nh.subscribe("motor_values", 1000, write_callback);
     ros::Publisher read_pub = nh.advertise<std_msgs::String>("serial_read", 1000);
 
     try
     {
         ser.setPort("/dev/ttyUSB0");
         ser.setBaudrate(57600);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ser.setTimeout(to);
         ser.setBytesize((serial::bytesize_t)8);
         ser.setParity((serial::parity_t)0);
         ser.setStopbits((serial::stopbits_t)1);
         ser.open();
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
 
     if(ser.isOpen()){
         ROS_INFO_STREAM("Serial Port initialized");
     }else{
         return -1;
     }
 
     ros::Rate loop_rate(5);
     while(ros::ok()){
 
         ros::spinOnce();
 
         if(ser.available()){
             ROS_INFO_STREAM("Reading from serial port");
             result.data = ser.readline(65536, "$");
             ROS_INFO_STREAM("Result data " << result.data);
             read_pub.publish(result);
         }
         loop_rate.sleep();
 
     }
 }