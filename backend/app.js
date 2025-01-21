var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var cardRouter = require('./routes/card');
var transactionsRouter = require('./routes/transactions');
var loginRouter = require('./routes/login');
var customerRouter = require('./routes/customer');
var accountRouter = require('./routes/account');
const jwt = require('jsonwebtoken');

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/login', loginRouter);
app.use(authenticateToken);
app.use('/card', cardRouter);
app.use('/transactions', transactionsRouter);
app.use('/account', accountRouter);
app.use('/customer', customerRouter);

// authenticateToken (Tämän funktion voi kommentoida pois, jos haluaa testata sovellusta ilman tokenia)
function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1];
  
    console.log("token = " + token);
    if (token == null) return res.sendStatus(401);
  
    jwt.verify(token, process.env.MY_TOKEN, function(err, user) {
        if (err) return res.sendStatus(403);
        req.user = user;
        next();
    });
}

module.exports = app;
