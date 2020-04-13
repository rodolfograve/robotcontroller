This is a quick and dirty project to use a web application to control an Arduino board connected via USB to a local PC.

There are 4 key components:
* An Arduino program that can interpret some basic commands.
  * LedOn: turns on LED13. LED13 is built-in and does not require any extra circuitry.
  * LedOff: turns off LED13.
  * Buzz: plays a frequency in a buzzer connected to the Arduino. This requires extra circuitry that I haven't documented in this project.
* A queue accessible from the internet.
  * For this I have use Azure Storage Queue. For authentication and authorization I have used Azure Active Directory.
  * Even though the Storage Queue endpoint is publicly visible, only certain users are allowed to submit commands. This is an application of the security by ID principle.
* A gateway application to read the commands from the queue and send them to the Arduino via Serial port.
  * It is possible to add network connectivity to the Arduino itself, but that requires more hardware. Also, using the Azure Storage APIs from the Arduino would add significant complexity to the project.
* A static web application (no server-side code) to put commands in the queue
  * Uses implicit flow and on-behalf-of (scope https://storage.azure.com/user_impersonation) so that no secrets are required.
  * Only users with the right permissions will be able to put commands in the queue.
