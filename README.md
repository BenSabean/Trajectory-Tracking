# Trajectory-Tracking
System to track the trajectory of an object through two-dimensional space


# Overview

For the Advanced Embedded Systems project, we will develop a trajectory tracking system that will use a client-server architecture. The client will be hosted on a Raspberry Pi and the server will be located on a second computer on the same local area network as the client. This will allow us to demonstrate the concepts necessary for networking without having to worry about router configuration or port forwarding.

The server will be responsible for modeling the trajectory based on user defined values for angle and thrust. The following formulas will be used to model the trajectory of the projectile:

----------

x position → x = x<sub>o</sub> + v<sub>i</sub>cos⁡(θ)t<br> 
Where x<sub>o</sub> = initial position<br> 
And t = time step<br>  


y position → y = y<sub>o</sub> + v<sub>y</sub>t<br> 
Where v<sub>y</sub> = v<sub>i</sub>sin⁡(θ)-at<br> 
a = acceleration of gravity<br> 
t = the time of travel<br> 

----------

This will allow us to calculate x and y position for a predetermined time step as the projectile moves through its’ predicted trajectory. The model will be passed back to the client using TCP/IP to display the results to the user. To fulfil the requirements for this project, the server will also be capable of handling connections from multiple clients. This will be accomplished by using multithreading to handle communication with the different clients.

The client will use potentiometers as user input to determine angle and initial thrust of the projectile. The client will then send this information to the server where the trajectory will be modeled and sent back to the client. The client will then graphically show the trajectory that the projectile would take in ideal conditions.

