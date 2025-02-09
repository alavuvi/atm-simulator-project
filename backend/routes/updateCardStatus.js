const express = require('express');
const router = express.Router();
const card = require('../models/card_model');

router.put('/:id', function(request, response) {
    if (!request.body.active) {
        return response.status(400).json({error: 'active card status is required'});
    }

    card.updateCardStatus(request.params.id, request.body.active, function(err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result.affectedRows);
        }
    });
});

module.exports = router;