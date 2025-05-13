using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows.Input;
using Microsoft.Toolkit.Mvvm.Input;
using WPFWeather.App.Models;
using WPFWeather.App.Services;

namespace WPFWeather.App.ViewModels
{
    /// <summary>
    ///     Main ViewModel for application.
    /// </summary>
    public class WeatherViewModel : INotifyPropertyChanged
    {
        private readonly IWeatherService _weatherService;
        private bool _downloadingWeather;
        private string _errorMessage;
        private WeatherInfoModel _weather;

        public WeatherViewModel(IWeatherService weatherService)
        {
            _weatherService = weatherService;
            DownloadWeatherCommand = new AsyncRelayCommand<string>(async city => await UpdateWeatherAsync(city));
        }

        /// <summary>
        ///     True when weather is currently being downloaded from online service.
        /// </summary>
        public bool DownloadingWeather
        {
            get => _downloadingWeather;
            private set
            {
                _downloadingWeather = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        ///     Indicates if the download was not successful.
        /// </summary>
        public bool IsWeatherInvalid => !string.IsNullOrEmpty(ErrorMessage);

        /// <summary>
        ///     Error message returned by server.
        /// </summary>
        public string ErrorMessage
        {
            get => _errorMessage;
            private set
            {
                _errorMessage = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(IsWeatherInvalid));
            }
        }

        public WeatherInfoModel Weather
        {
            get => _weather;
            private set
            {
                _weather = value;
                OnPropertyChanged();
            }
        }

        public ICommand DownloadWeatherCommand { get; }

        public event PropertyChangedEventHandler PropertyChanged;

        private async Task UpdateWeatherAsync(string city)
        {
            DownloadingWeather = true;
            ErrorMessage = string.Empty;

            try
            {
                Weather = await _weatherService.DownloadWeatherAsync(city);
            }
            catch (ArgumentOutOfRangeException)
            {
                HandleUpdateWeatherFailed("Weather forecast for selected city is not available.");
            }
            catch (InvalidOperationException)
            {
                HandleUpdateWeatherFailed("Weather forecast service is not available.");
            }
            catch (Exception)
            {
                HandleUpdateWeatherFailed("Weather forecast service failed.");
            }
            finally
            {
                DownloadingWeather = false;
            }
        }

        private void HandleUpdateWeatherFailed(string message)
        {
            ErrorMessage = message;
            Weather = new WeatherInfoModel();
        }

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}