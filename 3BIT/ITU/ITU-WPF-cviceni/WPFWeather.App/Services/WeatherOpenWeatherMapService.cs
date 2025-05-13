using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace WPFWeather.App.Services
{
    public class WeatherOpenWeatherMapService : WeatherServiceBase
    {
        private readonly string _openWeatherMapAppId = "5174c48b50122686367191e785842895";

        protected override async Task<string> GetWeatherAsync(string city)
        {
            var address =
                $@"http://api.openweathermap.org/data/2.5/weather?q={city}&APPID={_openWeatherMapAppId}&mode=json&units=metric";

            var client = new HttpClient();
            try
            {
                return await client.GetStringAsync(address);
            }
            catch (HttpRequestException e) when (e.Message.Contains("404"))
            {
                throw new ArgumentOutOfRangeException(nameof(city));
            }
            catch (HttpRequestException)
            {
                throw new InvalidOperationException();
            }
        }
    }
}