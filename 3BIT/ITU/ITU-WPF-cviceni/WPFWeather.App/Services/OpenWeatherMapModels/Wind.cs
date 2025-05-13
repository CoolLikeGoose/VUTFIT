using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Wind
    {
        [JsonProperty("speed")] public double Speed { get; set; }

        [JsonProperty("deg")] public double Deg { get; set; }
    }
}