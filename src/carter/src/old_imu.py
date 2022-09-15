#!/usr/bin/env python3

import time
import struct
import rospy
import numpy as np
from sensor_msgs.msg import Imu
from tf import transformations

imu_data = Imu()

def callback(data):
    # print(data.linear_acceleration.x, data.linear_acceleration.y, data.linear_acceleration.z)
    # print(data.angular_velocity.x, data.angular_velocity.y, data.angular_velocity.z)
    # print(data.orientation.x, data.orientation.y, data.orientation.z)
    q = transformations.quaternion_from_euler(data.orientation.x, data.orientation.y, data.orientation.z)
    print(q)
    imu_data.header.frame_id = 'imu_link'
    imu_data.orientation.x = q[0]
    imu_data.orientation.y = q[1]
    imu_data.orientation.z = q[2]
    imu_data.orientation.w = q[3]
    
    imu_data.angular_velocity.x = data.angular_velocity.x
    imu_data.angular_velocity.y = data.angular_velocity.y
    imu_data.angular_velocity.z = data.angular_velocity.z
    
    imu_data.linear_acceleration.x = data.linear_acceleration.x
    imu_data.linear_acceleration.y = data.linear_acceleration.y
    imu_data.linear_acceleration.z = data.linear_acceleration.z
    pub.publish(imu_data)
    
def listener():
    rospy.Subscriber("imu", Imu, callback)
    rospy.spin()


if __name__ == '__main__':
    rospy.init_node('imu_node', anonymous=True)
    pub = rospy.Publisher('imu_data', Imu, queue_size=1)
    listener()
    
