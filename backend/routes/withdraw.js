var express = require('express');
var router = express.Router();
var withdrawModel = require('../models/withdraw_model');

router.post('/', function(req, res) {
    const { idcard, amount } = req.body;

    if (!idcard || !amount) {
        return res.status(400).json({ error: 'Missing parameters' });
    }

    withdrawModel.getAccountByCard(idcard, function(err, results) {
        if (err) {
            return res.status(500).json({ error: err.message });
        }
        if (results.length === 0) {
            return res.status(404).json({ error: "No account found for this card" });
        }

        const idaccount = results[0].idaccount;
        console.log(`idcard: ${idcard} -> idaccount: ${idaccount}`);

        // Nyt kutsutaan proseduuria kolmella parametrilla
        withdrawModel.nosto(idaccount, idcard, amount, function(err, result) {
            if (err) {
                return res.status(500).json({ error: err.message });
            }
            res.json({ message: 'Withdrawal successful', result });
        });
    });
});

module.exports = router;
