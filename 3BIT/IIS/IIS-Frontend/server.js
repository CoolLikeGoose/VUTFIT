const express = require('express');
const path = require('path');

const app = express();
const PORT = 8080;

// Serve static files from the root directory
app.use(express.static(path.join(__dirname)));

// Fallback to index.html for SPA (Single Page Application) routing
app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'pages', 'index.html'));
});

app.listen(PORT, () => {
    console.log(`Server is running at http://localhost:${PORT}`);
});
