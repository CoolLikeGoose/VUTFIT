using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Weather
    {
        [JsonProperty("id")] public long Id { get; set; }

        [JsonProperty("main")] public string Main { get; set; }

        [JsonProperty("description")] public string Description { get; set; }

        [JsonProperty("icon")] public string Icon { get; set; }
    }
}