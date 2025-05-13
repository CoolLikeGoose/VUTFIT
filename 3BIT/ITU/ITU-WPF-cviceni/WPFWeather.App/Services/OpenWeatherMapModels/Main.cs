using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public class Main
    {
        [JsonProperty("temp")] public double Temp { get; set; }

        [JsonProperty("humidity")] public long Humidity { get; set; }

        [JsonProperty("pressure")] public double Pressure { get; set; }

        [JsonProperty("temp_min")] public double TempMin { get; set; }

        [JsonProperty("temp_max")] public long TempMax { get; set; }
    }
}