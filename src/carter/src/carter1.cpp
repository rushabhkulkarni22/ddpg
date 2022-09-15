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
#include <cmath>

#include <linux/can.h>
#include <linux/can/raw.h>

//////////

#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int16.h"
#include "geometry_msgs/Twist.h"
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
float VL;
float VR;
#define PI 3.14159

struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;
struct can_frame frame2;
struct can_frame frame_rx;
std_msgs::Int16 lpub;
std_msgs::Int16 rpub;
ros::Publisher left_pub;
ros::Publisher right_pub;

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

    frame.can_id = 0x141; // CAN id as 0x036
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

int Multiturn_Right()
{

    struct can_frame frame_tx1;

    frame_tx1.can_id = 0x141;
    frame_tx1.can_dlc = 8;
    frame_tx1.data[0] = 0x92;
    frame_tx1.data[1] = 0x00;
    frame_tx1.data[2] = 0x00;
    frame_tx1.data[3] = 0x00;
    frame_tx1.data[4] = 0x00;
    frame_tx1.data[5] = 0x00;
    frame_tx1.data[6] = 0x00;
    frame_tx1.data[7] = 0x00;

    if (write(s, &frame_tx1, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
        printf("\nwrote ");
    }

    struct can_frame frame_rx1;
    int Current_angle;

    while (1)
    {

        int nbytes = read(s, &frame_rx1, sizeof(struct can_frame));

        if ((frame_rx1.can_id == 0x141) && (frame_rx1.data[0] == 0x92))
        {

            signed long lsbENC = frame_rx1.data[1]; // Torque Current
            signed long msbENC = frame_rx1.data[2];
            signed long combinedENC0 = (msbENC << 8) | lsbENC;

            signed long lsbENC1 = combinedENC0; // Speed
            signed long msbENC1 = frame_rx1.data[3];
            signed long combinedENC1 = (msbENC1 << 16) | lsbENC1;

            signed long lsbENC2 = combinedENC1; // ENcoder Data
            signed long msbENC2 = frame_rx1.data[4];
            signed long combinedENC2 = (msbENC2 << 24) | lsbENC2;

            signed long lsbENC3 = combinedENC2; // Torque Current
            signed long msbENC3 = frame_rx1.data[5];
            signed long combinedENC3 = (msbENC3 << 32) | lsbENC3;

            signed long lsbENC4 = combinedENC3; // Speed
            signed long msbENC4 = frame_rx1.data[6];
            signed long combinedENC4 = (msbENC4 << 40) | lsbENC4;

            signed long lsbENC5 = combinedENC4; // ENcoder Data
            signed long msbENC5 = frame_rx1.data[7];
            int combinedENC5 = (msbENC5 << 48) | lsbENC5;

            Current_angle = (combinedENC5 / 600);
            //  printf("multi turn data right %d\n", Current_angle);
            return Current_angle;
        }
    }
}

//     }

void motorleft(long data2)
{

    store2 = data2;
    // printf("leftspeed input===%d\n\n", store2);

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

    frame2.can_id = 0x142; // CAN id as 0x036
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

int Multiturn_Left()
{

    struct can_frame frame_tx2;

    frame_tx2.can_id = 0x142;
    frame_tx2.can_dlc = 8;
    frame_tx2.data[0] = 0x92;
    frame_tx2.data[1] = 0x00;
    frame_tx2.data[2] = 0x00;
    frame_tx2.data[3] = 0x00;
    frame_tx2.data[4] = 0x00;
    frame_tx2.data[5] = 0x00;
    frame_tx2.data[6] = 0x00;
    frame_tx2.data[7] = 0x00;

    if (write(s, &frame_tx2, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
    }

    struct can_frame frame_rx2;
    int Current_angle2;
    while (1)

    {
        int nbytes = read(s, &frame_rx2, sizeof(struct can_frame));

        if ((frame_rx2.can_id == 0x142) && (frame_rx2.data[0] == 0x92))
        {

            signed long lsbENC6 = frame_rx2.data[1]; // Torque Current
            signed long msbENC6 = frame_rx2.data[2];
            signed long combinedENC6 = (msbENC6 << 8) | lsbENC6;

            signed long lsbENC7 = combinedENC6; // Speed
            signed long msbENC7 = frame_rx2.data[3];
            signed long combinedENC7 = (msbENC7 << 16) | lsbENC7;

            signed long lsbENC8 = combinedENC7; // ENcoder Data
            signed long msbENC8 = frame_rx2.data[4];
            signed long combinedENC8 = (msbENC8 << 24) | lsbENC8;

            signed long lsbENC9 = combinedENC8; // Torque Current
            signed long msbENC9 = frame_rx2.data[5];
            signed long combinedENC9 = (msbENC9 << 32) | lsbENC9;

            signed long lsbENC10 = combinedENC9; // Speed
            signed long msbENC10 = frame_rx2.data[6];
            signed long combinedENC10 = (msbENC10 << 40) | lsbENC10;

            signed long lsbENC11 = combinedENC10; // ENcoder Data
            signed long msbENC11 = frame_rx2.data[7];
            int combinedENC11 = (msbENC11 << 48) | lsbENC11;

            Current_angle2 = (combinedENC11 / 600);
            //  printf("multi turn data left %d\n", Current_angle2);
            return Current_angle2;
        }
    }
}

void chatterCallback(const geometry_msgs::Twist &twist)
{

VR = (((2 * twist.linear.x) + (twist.angular.z * 0.43)) / (0.21));        //2*0.105=0.21
 
   VL = (((2 * twist.linear.x) - (twist.angular.z * 0.43)) / (0.21)); // 43cm==distance between wheels, 0.1m radius

   

//     VR = (twist.linear.x + twist.angular.z * 0.43 / 2) / 0.105;

//   VL = (twist.linear.x - twist.angular.z * 0.43 / 2) / 0.105;




    VL = VL * 57.29577;  //constant
    VR = VR * 57.29577; //constant

    // if ((twist.linear.x < 0.23 && twist.linear.x > -0.23) && (twist.angular.z < 2.84 && twist.angular.z > -2.84))
    // {
    int z, Q;

    z = Multiturn_Right();
    rpub.data = z; //(z*0.00174);
    right_pub.publish(rpub);
    cout << "Right" << rpub.data << endl;
    sleep(0.03);

    Q = Multiturn_Left();
    lpub.data = -Q; //-(Q*0.00174);     // distance covered per degree= 1.74 mm
    left_pub.publish(lpub);
    cout << "Left" << lpub.data << endl;
    sleep(0.03);

    motorright(VR);
    sleep(0.03);

    motorleft(-VL);
    sleep(0.03);

    // }
}

int main(int argc, char **argv)
{

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

    for (int P = 1; P < 3; P++)
    {
        frame.can_id = 320 + P; // CAN id as 0x036   P= 4 and 7
        frame.can_dlc = 8;      // CAN data length as 8
        frame.data[0] = 0x80;   // Update humidity value in [0]
        frame.data[1] = 0x00;   // Update temperature value in [1]
        frame.data[2] = 0x00;   // Rest all with 0
        frame.data[3] = 0x00;
        frame.data[4] = 0x00;
        frame.data[5] = 0x00;
        frame.data[6] = 0x00;
        frame.data[7] = 0x00;
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
            perror("Write");
        }
        P++;
    }

    // Initiate a new ROS node named "listener"
    ros::init(argc, argv, "vel_ard");
    // create a node handle: it is reference assigned to a new node
    ros::NodeHandle node;

    ros::Subscriber sub = node.subscribe("/cmd_vel", 100, chatterCallback);
    left_pub = node.advertise<std_msgs::Int16>("lwheel", 100);
    right_pub = node.advertise<std_msgs::Int16>("rwheel", 100);

    ros::spin();

    return 0;
}
