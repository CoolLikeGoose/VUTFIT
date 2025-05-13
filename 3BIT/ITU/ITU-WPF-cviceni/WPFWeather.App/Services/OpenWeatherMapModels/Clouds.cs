using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Clouds
    {
        [JsonProperty("all")] public long All { get; set; }
    }
}