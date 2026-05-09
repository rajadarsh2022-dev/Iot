const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(bodyParser.json());

// In-memory storage for latest data
let latestData = {
  temperature: 0,
  humidity: 0,
  soilMoisture: 0,
  healthStatus: 'Initializing...',
  lastUpdated: new Date().toISOString()
};

// API Endpoints
app.post('/update', (req, res) => {
  try {
    const { temperature, humidity, soilMoisture, healthStatus } = req.body;

    latestData = {
      temperature: parseFloat(temperature),
      humidity: parseFloat(humidity),
      soilMoisture: parseInt(soilMoisture),
      healthStatus: healthStatus,
      lastUpdated: new Date().toISOString()
    };

    console.log('Data updated:', latestData);
    res.status(200).json({ message: 'Data updated successfully' });
  } catch (error) {
    console.error('Error updating data:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

app.get('/data', (req, res) => {
  res.json(latestData);
});

// Serve static files (for dashboard if needed)
app.use(express.static('public'));

app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});