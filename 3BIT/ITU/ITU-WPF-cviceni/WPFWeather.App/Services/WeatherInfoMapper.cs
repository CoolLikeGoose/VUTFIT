using System;
using System.Linq;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using WPFWeather.App.Models;
using WPFWeather.App.Services.OpenWeatherMapModels;

namespace WPFWeather.App.Services
{
    public class WeatherInfoMapper
    {
        public static WeatherInfoModel Map(Openweathermap openweathermap)
        {
            return new WeatherInfoModel
            {
                CityName = openweathermap.Name,
                Description = openweathermap.Weather.First()?.Description,
                Humidity = openweathermap.Main.Humidity,
                Icon = GetIcon(openweathermap.Weather.First().Icon),
                Sunrise = DateTimeOffset.FromUnixTimeSeconds(openweathermap.Sys.Sunrise).DateTime,
                Sunset = DateTimeOffset.FromUnixTimeSeconds(openweathermap.Sys.Sunset).DateTime,
                Temperature = (decimal)openweathermap.Main.Temp,
                WindDirection = (decimal)openweathermap.Wind.Deg,
                WindSpeed = (decimal)openweathermap.Wind.Speed
            };
        }

        private static ImageSource GetIcon(string icon)
        {
            if (icon == null) return null;
            return new BitmapImage(new Uri("/Icons/" + icon + ".png", UriKind.RelativeOrAbsolute));
        }
    }
}