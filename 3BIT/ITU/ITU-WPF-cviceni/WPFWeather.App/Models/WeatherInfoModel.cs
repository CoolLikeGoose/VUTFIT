using System;
using System.Windows.Media;

namespace WPFWeather.App.Models
{
    public struct WeatherInfoModel
    {
        public string CityName { get; set; }
        public decimal Temperature { get; set; }
        public decimal Humidity { get; set; }
        public decimal WindSpeed { get; set; }
        public decimal WindDirection { get; set; }
        public string Description { get; set; }
        public ImageSource Icon { get; set; }
        public DateTime Sunrise { get; set; }
        public DateTime Sunset { get; set; }
    }
}