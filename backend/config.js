'use strict';

const express = require('express');
const debug = require('debug')('backend:config');
const mongoose = require('mongoose');
const cors = require('cors');

const allRoutes = require('./routes/allRoutes.js');

const app = express();
mongoose.connect(process.env.MONGODB_URI, { 
  useCreateIndex: true,
  useNewUrlParser: true
});

// ROUTES & MIDDLEWARE
app.use(cors({
    credentials: true,
    origin: process.env.CORS_ORIGINS,
}));
app.use(allRoutes);

module.exports = app;