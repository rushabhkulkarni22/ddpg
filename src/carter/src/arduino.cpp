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

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include <sstream>
#define FRAME_LEN 8
using namespace std;
#define pi 3.14159

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
struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;
struct can_frame frame2;

float FR;
float LR;
int i, j = 0;
int s;

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

void chatterCallback(const geometry_msgs::Twist &msg)
{

    geometry_msgs::Twist odom;

    cout << fixed << setprecision(2) << FR << endl;
    cout << fixed << setprecision(2) << LR << endl;

    FR = (msg.linear.x)*800;

    LR = (msg.angular.z );
}

int main(int argc, char **argv)
{
 cout<<"i am in main"<<endl;
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

    ros::init(argc, argv, "vel2");
    cout<<" initiated vel2"<<endl;

    ros::NodeHandle node;

    ros::Subscriber sub = node.subscribe("cmd_vel", 100, chatterCallback);

    // ros::Publisher chatter_pub = node.advertise<geometry_msgs::Twist>("chatter2", 100);

    ros::Rate loop_rate(30);

    // usleep(1);
    //        cout << "FR X  " << FR << endl;
    //        cout << "LR Z  " << LR << endl;
    geometry_msgs::Twist msg1;
    //   msg1.linear.x = FR;
    //    msg1.angular.z = LR;

    //    chatter_pub.publish(msg1);
    ros::spin();
    loop_rate.sleep();
    return 0;
}
// ros::spin();
///}



