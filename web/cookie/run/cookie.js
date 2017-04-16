const express = require('express');
const fs = require('fs');
const path = require('path');

const IMAGE = fs.readFileSync('./blueguy.jpg');
const PORT = 50501;

var app = express();

app.get('/', (req, res) => {
    res.append('Set-Cookie', 'token=TG17{fikseboller}');
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/blueguy.jpg', (req, res) => {
    res.send(IMAGE);
});

app.listen(PORT);

