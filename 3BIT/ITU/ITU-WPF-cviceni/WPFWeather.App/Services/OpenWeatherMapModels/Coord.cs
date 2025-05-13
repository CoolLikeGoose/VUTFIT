using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Coord
    {
        [JsonProperty("lon")] public double Lon { get; set; }

        [JsonProperty("lat")] public double Lat { get; set; }
    }
}