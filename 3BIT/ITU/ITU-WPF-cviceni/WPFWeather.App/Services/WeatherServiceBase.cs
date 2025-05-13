using System;
using System.Threading.Tasks;
using WPFWeather.App.Models;
using WPFWeather.App.Services.OpenWeatherMapModels;

namespace WPFWeather.App.Services
{
    public abstract class WeatherServiceBase : IWeatherService
    {
        public async Task<WeatherInfoModel> DownloadWeatherAsync(string city)
        {
            if (string.IsNullOrEmpty(city)) throw new ArgumentOutOfRangeException(nameof(city));

            var jsonString = await GetWeatherAsync(city.ToLower());

            if (string.IsNullOrEmpty(jsonString)) throw new ArgumentOutOfRangeException(nameof(city));

            var weatherFromJson = Openweathermap.FromJson(jsonString);

            return WeatherInfoMapper.Map(weatherFromJson);
        }

        protected abstract Task<string> GetWeatherAsync(string city);
    }
}