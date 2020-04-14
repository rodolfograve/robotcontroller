This is a quick and dirty project to use a web application to control an Arduino board connected via USB to a local PC.

There are 4 key components:

# Arduino.RobotProgram
An Arduino program that can interpret some basic commands:
1. LedOn: turns on LED13. LED13 is built-in and does not require any extra circuitry.
1. LedOff: turns off LED13.
1. Buzz: plays a frequency in a buzzer connected to the Arduino. This requires extra circuitry that I haven't documented in this project.

# Queue
A queue accessible from the internet.

For this I have used [Azure Storage Queue](https://docs.microsoft.com/en-us/azure/storage/queues/storage-queues-introduction). For authentication and authorization I have used the free tier of [Azure Active Directory](https://azure.microsoft.com/en-gb/services/active-directory/).

Even though the Storage Queue endpoint is publicly visible, only certain users are allowed to submit commands. This is an application of the *security by ID principle*.

# Arduino.RobotController.CommandsGateway
A .NET Core Console application to read the commands from the queue and send them to the Arduino via Serial port.

It is possible to add network connectivity to the Arduino itself, but that requires more hardware. Also, using the Azure Storage APIs from the Arduino would add significant complexity to the project.

Secret settings are dealt with using *dotnet core user-secret*.

# Arduino.RobotController.WebUI
A static web application (no server-side code) to put commands in the queue.

Uses implicit flow and on-behalf-of (scope https://storage.azure.com/user_impersonation) so that **no secrets** are required. Only users with the right permissions will be able to put commands in the queue.

This web application can run locally, but I have also hosted it in an Azure Storage Account using the [Static Web feature](https://docs.microsoft.com/en-us/azure/storage/blobs/storage-blob-static-website) and [Azure CDN](https://azure.microsoft.com/en-gb/services/cdn/), and is only available via HTTPS.
  
*The reason the projects use **Robot** in the name is because I intend to improve the circuitry and the Arduino program to control more advanced actuators and sensors, ultimately turning it into something close to a fun robot*

# Known Issues
If multiple commands are sent in quick succession they will get mixed up in the Arduino and won't be processed correctly. Possible solutions are to either send a correlated acknowledgement from the Arduino to the CommandGateway, or even better, to improve the parsing logic in the Arduino so that it's able to handle multiple commands concatenated.

# Notes
All in all, even though the idea is quite simple, this project does include some non-trivial concepts.

# Useful Links
These are some of the links I have found useful:
1. https://docs.microsoft.com/en-us/azure/storage/common/storage-auth-aad
1. https://devblogs.microsoft.com/premier-developer/setting-up-net-core-configuration-providers/
1. https://docs.microsoft.com/en-us/aspnet/core/security/app-secrets?view=aspnetcore-3.1&tabs=windows
1. http://www.iconarchive.com/tag/robot
1. https://docs.microsoft.com/en-us/rest/api/storageservices/queue-service-error-codes
1. https://docs.microsoft.com/en-us/rest/api/storageservices/queue-service-rest-api
1. https://docs.microsoft.com/en-us/rest/api/storageservices/protocol-version-support-for-.net-client-library-versions
1. https://arlanblogs.alvarnet.com/azure-static-website-setup-error-this-xml-file-does-not-appear-to-have-any-style-information-associated-with-it/
