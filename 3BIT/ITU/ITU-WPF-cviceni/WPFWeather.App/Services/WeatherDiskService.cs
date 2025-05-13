using System.IO;
using System.Threading.Tasks;

namespace WPFWeather.App.Services
{
    public class WeatherDiskService : WeatherServiceBase
    {
        protected override async Task<string> GetWeatherAsync(string city)
        {
            //Simulate network delay
            await Task.Delay(500);

            var dataFilePath = $@"Data/{city}.json";
            if (!File.Exists(dataFilePath)) return string.Empty;

            return File.ReadAllText(dataFilePath);
        }
    }
}