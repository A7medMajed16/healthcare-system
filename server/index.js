const express = require("express");
var bodyParser = require('body-parser')
const app = express();
const path = require("path");
const fs = require("fs");
// constants
const DB_PATH = path.resolve("db.json");
const PORT = process.env.PORT || 3306;
// middlewares
app.use(bodyParser.text());

// routes
app.get("/ping", async (req, res) => {
    res.send("pong");
});
app.get("/", async (req, res) => {
    fs.readFile(DB_PATH, "utf-8", (err, jsonString) => {
        if (err) return console.log("Error in reading from db");
        let values = JSON.parse(jsonString);
        res.status(200).json({
            totalValues: values.length,
            values,
        });
    });
});
app.post("/", async (req, res) => {
    console.log(req.body)
});
app.listen(PORT, () => console.log("Listening on port", PORT));