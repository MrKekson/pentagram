const express = require('express');
const path = require('path');
const app = express()
const port = 3000
const fs = require('fs');

app.use(express.json());

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '/index.html'));
})

app.post('/savesettings', (req, res) => {
  res.send(req.body);
  let data = JSON.stringify(req.body, null, "\t");
  fs.writeFileSync('settings.json', data);
})

app.get('/settings.json', (req, res) => {
  res.sendFile(path.join(__dirname, '/settings.json'));
})

app.listen(port, () => {
  console.log(`Listening on port ${port}`)
})