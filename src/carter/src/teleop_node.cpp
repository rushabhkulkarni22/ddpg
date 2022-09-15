#include "ros/ros.h"

#include <iostream>
// for delay function.
#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <unistd.h>

// for signal handling
#include <signal.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include <JetsonGPIO.h> ///home/agx3/ddpg_ws/src/JetsonGPIO/include/
using namespace std;
int countt = 0;
int z = 0;
bool check = true;
int c = 0;

static bool end_this_program = false;

void signalHandler(int s) { end_this_program = true; }

void chatterCallback(const geometry_msgs::Twist &msg)
{
}

void blink(const std::string &channel)

{

      z = z + 1;
      printf("counts     %d \n", z);
}

int main(int argc, char **argv)
{

      ros::init(argc, argv, "pwm");
      // create a node handle: it is reference assigned to a new node
      ros::NodeHandle node;

      ros::Subscriber sub = node.subscribe("cmd_vel", 100, chatterCallback);
      // Pin Definitions
      int output_pin = 18;
      int pulse_pin = 12;
      int brake = 13;
      // get_output_pin();

      // When CTRL+C pressed, signalHandler will be called
      signal(SIGINT, signalHandler);

      // Pin Setup.
      // Board pin-numbering scheme
      GPIO::setmode(GPIO::BOARD);

      // set pin as an output pin with optional initial state of HIGH
      GPIO::setup(output_pin, GPIO::OUT);
      GPIO::setup(pulse_pin, GPIO::IN);
      GPIO::setup(brake, GPIO::OUT, GPIO::LOW);

      GPIO::PWM p(output_pin, 1000);
      auto val = 10;
      // auto incr = 5.0;
     // p.start(val);

      cout << "PWM running. Press CTRL+C to exit." << endl;
      GPIO::add_event_detect(pulse_pin, GPIO::Edge::BOTH, blink);

      while (!end_this_program)  //
      {

            // if (z > 90)
            // {

            //       c++;
            //       printf("    revolution number     %d \n", c);
            //       z = 0;
            //       // GPIO::output(brake, GPIO::HIGH);
            //       // printf("counts                                                  %d \n",z);
            //       p.stop();
            //       //   sleep(0.1);
            //       // GPIO::output(brake, GPIO::LOW);
            //       break;
            // }
      }
      GPIO::cleanup();

      return 0;
}

// void blink(const std::string& channel)
//       {

//           GPIO::setup(brake, GPIO::OUT,GPIO::LOW);
//       z=z+1;
//       printf("counts                           %d \n",z);

//        if(z>89 )
//              {

//                GPIO::output(brake, GPIO::HIGH);
//                 printf("counts                                                  %d \n",z);
//                p.stop();
//               sleep(0.1);
//               GPIO::output(brake, GPIO::LOW);
//              }
//        }