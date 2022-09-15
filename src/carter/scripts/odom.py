#!/usr/bin/python
import rospy 
from nav_msgs.msg import Odometry
from tf.transformations import euler_from_quaternion
from geometry_msgs.msg import Point, Twist
from math import atan2

x= 0.0
y= 0.0
theta = 0.0
def newOdom (msg): 
    global x 
    global y 
    global theta
    x = msg.pose.pose.position.x 
    y = msg.pose.pose.position.y
    rot_q = msg.pose.pose.orientation
rospy.init_node ("speed_controller")

sub = rospy.Subscriber("/odom", Odometry, newOdom)
pub = rospy.Publisher("/cmd_vel",Twist, queue_size=1)

speed = Twist()

r = rospy.Rate(4)

goal = Point()
goal.x = 1

while not rospy.is_shutdown(): 
    inc_x= goal.x - x 
    # print("goal.x", goal.x) # print("x",x) print("inc_X ",inc_x )
    if(inc_x>0.05): 
        speed.linear.x = 0.1 # print("speeed if",speed.linear.x )
    else: 
        speed.linear.x = 0.0 # print("speeed else",speed.linear.x )
    pub.publish(speed)
    r.sleep() 




# while not rospy.is_shutdown(): 
#     inc_x= goal.x - x 
#     # print("goal.x", goal.x) # print("x",x) print("inc_X ",inc_x )
#     if(inc_x>0.05): 
#         speed.linear.x = 0.0 # print("speeed if",speed.linear.x )
#     else: 
#         speed.linear.x = -0.1 # print("speeed else",speed.linear.x )
#     pub.publish(speed)
#     r.sleep() 