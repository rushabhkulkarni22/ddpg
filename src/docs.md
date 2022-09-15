

- kinetic : ubuntu 16.04         
- melodic : ubuntu 18.04    
- noetic  : ubuntu 20.04
=====================================================================

1. ros installation [link](http://wiki.ros.org/noetic/Installation/Ubuntu) :     

2. turtlebot 3 installations for ros [melodic](https://emanual.robotis.com/docs/en/platform/turtlebot3/quick-start/) :
 

3. SLAM package for [reference]( https://github.com/bandasaikrishna/Autonomous_Mobile_Robot):

4. rplidar ros [package](https://github.com/robopeak/rplidar_ros):   

5. AMCL localizatiion package tuning and [info](http://wiki.ros.org/amcl):
    

6. [Move_Base](http://wiki.ros.org/move_base)    
 
7. [DDPG package](https://github.com/hanlinniu/turtlebot3_ddpg_collision_avoidance)

8. docker for ddpg on amd64 :  docker pull 2014629/ddpg_img
   [ddpg](https://hub.docker.com/r/2014629/ddpg_img)

9. To create a map:

    1. by [Gmapping](http://wiki.ros.org/gmapping) method  

    2. by [Hectorslam](https://github.com/samialperen/oko_slam/blob/master/doc/hector_slam_tutorial.md) method : 
   
    3. by [cartographer](https://github.com/cartographer-project/cartographer_ros) method : 



CANBUS SETUP
=====================================================================
- Setting up CAN Bus communication on [agx](https://medium.com/@ramin.nabati/enabling-can-on-nvidia-jetson-xavier-developer-kit-aaaa3c4d99c9): 

- using [IMUdata](https://automaticaddison.com/visualize-imu-data-using-the-mpu6050-ros-and-jetson-nano/) in rviz using MPU6050 : 
**(you can fuse this data with lidar to avoid effect of drifting of wheels, use ekf filter to reduce the yaw drift )**

Script to Setup canbus communication on any system(agx also)
=====================================================================

- prerequisites required;
    - apt install busybox, can-utils 
    - busybox is for AGX
 

# name: xyz.sh

#!/bin/bash

***for agx, we need to modify register values. Below are the 4 lines to modify registers using busybox.***
 
 sudo busybox devmem 0x0c303000 32 0x0000C400
 sudo busybox devmem 0x0c303008 32 0x0000C458
 sudo busybox devmem 0x0c303010 32 0x0000C400
 sudo busybox devmem 0x0c303018 32 0x0000C458


- ***for usb2can***

 sudo modprobe can  
 sudo modprobe can_raw   
 sudo modprobe mttcan   
 sudo ip link set can0 type can bitrate 500000   
 sudo ip link set can1 type can bitrate 1000000   
 sudo ip link set up can0   
 sudo ip link set up can1    
 sudo ip link set can0 type can restart-ms 100   
 sudo ip link set can1 type can restart-ms 100

./xyz.sh 
=====================================================================
