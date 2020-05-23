using System;
using System.Collections.Immutable;
using System.IO.Ports;
using System.Threading;

namespace Arduino.RobotController.CommandsGateway
{
    public class RobotController
    {
        /// <summary>
        /// Make sure only recognised commands are passed on the Arduino. This introduces extra work but makes it impossible to "attack" the device with long strings, etc.
        /// </summary>
        private static readonly ImmutableHashSet<string> CommandsWhiteList = ImmutableHashSet.CreateRange(new[] { "Buzz", "LedOn", "LedOff", "UseKeyPad" });

        public RobotController(string serialPort)
        {
            SerialPort = new SerialPort(serialPort, 9600);
            SerialPort.DataReceived += (_, __) => DataReceived();
        }

        private readonly SerialPort SerialPort;

        public bool TrySendCommand(string command, out string errorMessage)
        {
            if (CommandsWhiteList.Contains(command))
            {
                if (!SerialPort.IsOpen)
                {
                    SerialPort.Open();
                }
                SerialPort.WriteLine(command);
                errorMessage = "";

                Thread.Sleep(1000);
                return true;
            }
            else
            {
                errorMessage = $"Unrecognised command: {command}";
                return false;
            }
        }

        private void DataReceived()
        {
            var data = SerialPort.ReadLine();
            Console.WriteLine($"Robot sent message: {data}");
        }
    }
}
