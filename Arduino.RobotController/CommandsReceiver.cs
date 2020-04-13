using Microsoft.Azure.Storage.Queue;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace Arduino.RobotController.CommandsGateway
{
    public class CommandsReceiver
    {
        public CommandsReceiver(CloudQueue queue, RobotController robotController)
        {
            Queue = queue;
            RobotController = robotController;
        }

        private readonly CloudQueue Queue;
        private readonly RobotController RobotController;

        public async Task Start(CancellationToken cancellationToken)
        {
            await Queue.ClearAsync();
            while (!cancellationToken.IsCancellationRequested)
            {
                var message = await Queue.GetMessageAsync();
                if (message == null)
                {
                    Console.WriteLine("No message found in the queue. Waiting for 1 second");
                    await Task.Delay(1000);
                }
                else
                {
                    ProcessCommand(message.AsString);
                    await Queue.DeleteMessageAsync(message);
                }
            }
        }

        private void ProcessCommand(string command)
        {
            if (RobotController.TrySendCommand(command, out var errorMessage))
            {
                Console.WriteLine($"Command {command} sent");
            }
            else
            {
                Console.Error.WriteLine($"Command {command} failed: {errorMessage}");
            }
        }

    }
}
