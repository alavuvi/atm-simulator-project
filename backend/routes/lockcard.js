const express = require('express');
const router = express.Router();
const card = require('../models/card_model');

router.post('/', function(request, response) {
    const idcard = request.body.idcard;
    
    if(!idcard) {
        return response.send("-11");
    }
    
    card.updateCardStatus(idcard, 0, function(err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result.affectedRows);
        }
    });
});

module.exports = router;