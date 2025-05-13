using Newtonsoft.Json;

namespace WPFWeather.App.Services.OpenWeatherMapModels
{
    public partial class Openweathermap
    {
        [JsonProperty("coord")] public Coord Coord { get; set; }

        [JsonProperty("sys")] public Sys Sys { get; set; }

        [JsonProperty("weather")] public Weather[] Weather { get; set; }

        [JsonProperty("base")] public string Base { get; set; }

        [JsonProperty("main")] public Main Main { get; set; }

        [JsonProperty("wind")] public Wind Wind { get; set; }

        [JsonProperty("clouds")] public Clouds Clouds { get; set; }

        [JsonProperty("dt")] public long Dt { get; set; }

        [JsonProperty("id")] public long Id { get; set; }

        [JsonProperty("name")] public string Name { get; set; }

        [JsonProperty("cod")] public long Cod { get; set; }
    }

    public partial class Openweathermap
    {
        public static Openweathermap FromJson(string json)
        {
            return JsonConvert.DeserializeObject<Openweathermap>(json, Converter.Settings);
        }
    }
}