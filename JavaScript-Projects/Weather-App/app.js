const apiKey = 'your-openweather-api-key'; // Replace with your OpenWeather API key

const searchBtn = document.getElementById('search-btn');
const weatherResult = document.getElementById('weather-result');

searchBtn.addEventListener('click', getWeather);

function getWeather() {
    const city = document.getElementById('city-input').value;
    
    if (city === '') {
        weatherResult.innerHTML = '<p class="error">Please enter a city name.</p>';
        return;
    }

    const url = `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${apiKey}&units=metric`;

    fetch(url)
        .then(response => response.json())
        .then(data => {
            if (data.cod === '404') {
                weatherResult.innerHTML = `<p class="error">City not found. Please try again.</p>`;
            } else {
                const weatherInfo = `
                    <p>Location: ${data.name}, ${data.sys.country}</p>
                    <p>Temperature: ${data.main.temp}°C</p>
                    <p>Weather: ${data.weather[0].description}</p>
                    <p>Humidity: ${data.main.humidity}%</p>
                `;
                weatherResult.innerHTML = weatherInfo;
            }
        })
        .catch(error => {
            weatherResult.innerHTML = `<p class="error">Error fetching weather data.</p>`;
            console.error(error);
        });
}

