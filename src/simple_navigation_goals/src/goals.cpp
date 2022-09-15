/*
 * Author: Anis Koubaa for Gaitech EDU
 * Year: 2016
 *
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

//////////

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Point.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include <sstream>
#define FRAME_LEN 8
using namespace std;

///////////////////////////////////////////////////////////////////////

long Speed;
long data1;
long data2;
long forrev1;
long forrev2;
long leftright;
long store1;
long store2;
long four;
long five;
long six;
long seven;

long four1;
long five1;
long six1;
long seven1;
int s;

// Topic messages callback
float FR;
float LR;

struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;
struct can_frame frame2;


void motorright(long data1)
{
    store1 = data1;

    if (store1 >= 0)
    {
        store1 = 600 * store1;
    }

    else

    {

        store1 = 4294967294 + (600 * store1);
    }
    four = (store1 & 0x000000FF) >> 0;
    five = (store1 & 0x0000FF00) >> 8;
    six = (store1 & 0x00FF0000) >> 16;
    seven = (store1 & 0xFF000000) >> 24;

    frame.can_id = 0x144; // CAN id as 0x036
    frame.can_dlc = 8;    // CAN data length as 8
    frame.data[0] = 0xA2; // Update humidity value in [0]
    frame.data[1] = 0x00; // Update temperature value in [1]
    frame.data[2] = 0x00; // Rest all with 0
    frame.data[3] = 0x00;
    frame.data[4] = four;
    frame.data[5] = five;
    frame.data[6] = six;
    frame.data[7] = seven;
    if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

void motorleft(long data2)
{

    store2 = data2;

    if (store2 >= 0)
    {
        store2 = 600 * store2;
    }

    else

    {

        store2 = 4294967294 + (600 * store2);
    }
    four1 = (store2 & 0x000000FF) >> 0;
    five1 = (store2 & 0x0000FF00) >> 8;
    six1 = (store2 & 0x00FF0000) >> 16;
    seven1 = (store2 & 0xFF000000) >> 24;

    frame2.can_id = 0x146; // CAN id as 0x036
    frame2.can_dlc = 8;    // CAN data length as 8
    frame2.data[0] = 0xA2; // Update humidity value in [0]
    frame2.data[1] = 0x00; // Update temperature value in [1]
    frame2.data[2] = 0x00; // Rest all with 0
    frame2.data[3] = 0x00;
    frame2.data[4] = four1;
    frame2.data[5] = five1;
    frame2.data[6] = six1;
    frame2.data[7] = seven1;
    if (write(s, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }
}

void chatterCallback(const nav_msgs::Odometry &msg)
{

    FR = (msg.twist.twist.linear.x)*550 ;

    LR = (msg.twist.twist.angular.z) ;

    // LR=msg.angular.z ;

    cout << "FR" << FR << endl;
    cout << "LR" << LR << endl;
    /////////////////////////////////////////
    if ((msg.twist.twist.linear.x < 0.23 && msg.twist.twist.linear.x > -0.23) && (msg.twist.twist.angular.z < 0.28 && msg.twist.twist.angular.z > -0.28))
    {
           
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        if(msg.twist.twist.linear.x > 0.05 || msg.twist.twist.linear.x < -0.05){
             motorright(FR);
             motorleft(-FR);
        }
        else if(-0.05 <= msg.twist.twist.linear.x <= 0.05){
             if (msg.twist.twist.angular.z > 0){

                    motorright(LR);
                    motorleft(0);
                }

                if (msg.twist.twist.angular.z < 0)

                {

                    motorright(0);
                    motorleft(LR);
                }

        }


        // if ((msg.twist.twist.linear.x > 0.0) || (msg.twist.twist.linear.x < 0))

        // {
                
        //      motorright(FR);
        //      motorleft(-FR);
        // }

        // if (msg.twist.twist.linear.x == 0)
        // {

        //     if (msg.twist.twist.angular.z > 0)

        //     {

        //          motorright(LR);
        //          motorleft(0);
        //     }

        //     if (msg.twist.twist.angular.z < 0)

        //     {

        //          motorright(0);
        //          motorleft(LR);
        //     }
        // }
    }
}

int main(int argc, char **argv)
{

    int i, j = 0;

    int ret;
    int ret1, ang;
    // int flag=0;

    float RPS;
    int combinedMTRST3;

    // printf("CAN Sockets Demo\r\n");

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return 1;
    }

    // Initiate a new ROS node named "listener"
    ros::init(argc, argv, "vel9");
    // create a node handle: it is reference assigned to a new node
    ros::NodeHandle node;

    ros::Subscriber sub = node.subscribe("odom9", 100, chatterCallback);

    ros::spin();

    return 0;
}

