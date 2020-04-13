using Microsoft.Azure.Storage;
using Microsoft.Azure.Storage.Queue;
using Microsoft.Extensions.Configuration;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace Arduino.RobotController.CommandsGateway
{
    class Program
    {
        static async Task Main(string[] args)
        {
            var configuration = new ConfigurationBuilder().AddUserSecrets("50e49a18-58ad-4424-8d8d-bc8216ed67ae").Build();
            var azureStorageQueueConnectionString = configuration.GetValue<string>("StorageAccountConnectionString") ?? throw new Exception("Cannot find connection string setting");

            var robotController = new RobotController("COM3");

            var storageAccount = CloudStorageAccount.Parse(azureStorageQueueConnectionString);
            var queueClient = storageAccount.CreateCloudQueueClient();
            var queue = queueClient.GetQueueReference("commands");

            var commandsReceiver = new CommandsReceiver(queue, robotController);
            var cancellationToken = new CancellationToken();
            await commandsReceiver.Start(cancellationToken);
        }
    }
}
