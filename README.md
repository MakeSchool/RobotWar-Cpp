#Robot War

In this exercise, you'll learn a bit about basic artificial intelligence through a friendly competition. During *Robot Wars* you'll program the behavior a virtual robot. Your robot will go head to head with other robots in the *Robot Wars Showdown*!

- Create a new Robot by inheriting from the `RobotCpp` class:

        class SimpleRobotCpp : public RobotCpp 
        {
          
        };

- Define your Robot to participate in the Battle by placing the Class name of the robot in "Configuration.h"

		#define ROBOT_ONE_CLASS SimpleRobotCpp
		#define ROBOT_TWO_CLASS MyRobotCpp

- The available C++ interface is documented in `RobotCpp.h` and `CppInterface.h`.
