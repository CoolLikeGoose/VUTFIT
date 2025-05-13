using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Sys
    {
        [JsonProperty("country")] public string Country { get; set; }

        [JsonProperty("sunrise")] public long Sunrise { get; set; }

        [JsonProperty("sunset")] public long Sunset { get; set; }
    }
}