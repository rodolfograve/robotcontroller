This is a quick and dirty project to use a web application to control an Arduino board connected via USB to a local PC.

There are 4 key components:
* **Arduino.RobotProgram** - An Arduino program that can interpret some basic commands.
  * LedOn: turns on LED13. LED13 is built-in and does not require any extra circuitry.
  * LedOff: turns off LED13.
  * Buzz: plays a frequency in a buzzer connected to the Arduino. This requires extra circuitry that I haven't documented in this project.
* A queue accessible from the internet.
  * For this I have used [Azure Storage Queue](https://docs.microsoft.com/en-us/azure/storage/queues/storage-queues-introduction). For authentication and authorization I have used the free tier of [Azure Active Directory](https://azure.microsoft.com/en-gb/services/active-directory/).
  * Even though the Storage Queue endpoint is publicly visible, only certain users are allowed to submit commands. This is an application of the *security by ID principle*.
* **Arduino.RobotController.CommandsGateway** - A .NET Core Console application to read the commands from the queue and send them to the Arduino via Serial port.
  * It is possible to add network connectivity to the Arduino itself, but that requires more hardware. Also, using the Azure Storage APIs from the Arduino would add significant complexity to the project.
* **Arduino.RobotController.WebUI** - A static web application (no server-side code) to put commands in the queue
  * Uses implicit flow and on-behalf-of (scope https://storage.azure.com/user_impersonation) so that no secrets are required.
  * Only users with the right permissions will be able to put commands in the queue.
  
*The reason the projects use **Robot** in the name is because I intend to improve the circuitry and the Arduino program to control more advanced actuators and sensors, ultimately turning it into something close to a fun robot*
