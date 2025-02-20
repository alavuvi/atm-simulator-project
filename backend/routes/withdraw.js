
var express = require('express');
var router = express.Router();
var withdrawModel = require('../models/withdraw_model');

router.post('/', function(req, res) {
    const { idaccount, amount } = req.body; 

    if (!idaccount || !amount) {
        return res.status(400).json({ error: 'Missing parameters' });
    }

    withdrawModel.nosto(idaccount, amount, function(err, result) { 
        if (err) {
            return res.status(500).json({ error: err.message });
        }
        res.json({ message: 'Withdrawal successful', result });
    });
});

module.exports = router;