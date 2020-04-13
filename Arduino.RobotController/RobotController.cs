using System;
using System.Collections.Immutable;
using System.IO.Ports;

namespace Arduino.RobotController.CommandsGateway
{
    public class RobotController
    {
        private static readonly ImmutableHashSet<string> ValidCommands = ImmutableHashSet.CreateRange(new[] { "Buzz", "LedOn", "LedOff" });

        public RobotController(string serialPort)
        {
            SerialPort = new SerialPort(serialPort, 9600);
            SerialPort.DataReceived += (_, __) => DataReceived();
        }

        private readonly SerialPort SerialPort;

        public bool TrySendCommand(string command, out string errorMessage)
        {
            if (ValidCommands.Contains(command))
            {
                if (!SerialPort.IsOpen)
                {
                    SerialPort.Open();
                }
                SerialPort.WriteLine(command);
                errorMessage = "";
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
