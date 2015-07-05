#Robot War

In this exercise, you'll learn a bit about basic artificial intelligence through a friendly competition. During *Robot Wars* you'll program the behavior a virtual robot. Your robot will go head to head with other robots in the *Robot Wars Showdown*!

- Create a new Robot by inheriting from the "Robot" class:

        class SimpleRobot: Robot {
          
        }

- Define your Robot to participate in the Battle by placing the Class name of the robot in "Configuration.h"

        static NSString *robotClass1 = @"RobotOneClassName";
        static NSString *robotClass2 = @"RobotTwoClassName";

- The available Swift interface is documented in an HTML file in the /docs folder and in "Robot.h" (you'll need to be able to read Objective-C)
