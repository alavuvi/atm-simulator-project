var express = require('express');
var router = express.Router();
var withdrawModel = require('../models/withdraw_model');
var accountcard = require('../models/account_model');

router.post('/', function(req, res) {
    const { idaccount, amount } = req.body;

    if (!idaccount || !amount) {
        return res.status(400).json({ error: 'Missing parameters' });
    }
    
    const withdrawalAmount = parseFloat(amount);
    if (isNaN(withdrawalAmount) || withdrawalAmount <= 0) {
        return res.status(400).json({ error: 'Invalid withdrawal amount' });
    }
    
    accountcard.getCreditLimit(idaccount, function(err, result) {
        if (err) {
            return res.status(500).json({ error: err.message });
        }
        if (!result || result.length === 0) {
            return res.status(404).json({ error: 'Credit limit not found.' });
        }
        
        const { credit_limit, credit_used } = result[0];
        const availableCredit = credit_limit - credit_used;
        
        if (withdrawalAmount > availableCredit) {
            return res.status(400).json({ error: 'Credit limit overdrawn.' });
        }
        
        withdrawModel.nosto(idaccount, withdrawalAmount, function(err, result) {
            if (err) {
                return res.status(500).json({ error: err.message });
            }
            res.json({ message: 'Withdrawal successful', result });
        });
    });
});

module.exports = router;
