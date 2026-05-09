const API_URL = '/data';

function fetchData() {
  fetch(API_URL)
    .then(response => response.json())
    .then(data => {
      document.getElementById('temperature').textContent = data.temperature + '°C';
      document.getElementById('humidity').textContent = data.humidity + '%';
      document.getElementById('soilMoisture').textContent = data.soilMoisture;
      document.getElementById('healthStatus').textContent = data.healthStatus;
      document.getElementById('lastUpdated').textContent = new Date(data.lastUpdated).toLocaleString();

      // Change color based on health status
      const healthElement = document.getElementById('healthStatus');
      if (data.healthStatus === 'Healthy') {
        healthElement.style.color = '#2e7d32';
      } else {
        healthElement.style.color = '#f44336';
      }
    })
    .catch(error => {
      console.error('Error fetching data:', error);
      document.getElementById('healthStatus').textContent = 'Connection Error';
    });
}

// Fetch data on page load
fetchData();

// Auto-refresh every 5 seconds
setInterval(fetchData, 5000);