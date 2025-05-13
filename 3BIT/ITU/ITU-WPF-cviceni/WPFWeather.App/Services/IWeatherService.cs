using System.Threading.Tasks;
using WPFWeather.App.Models;

namespace WPFWeather.App.Services
{
    public interface IWeatherService
    {
        Task<WeatherInfoModel> DownloadWeatherAsync(string city);
    }
}