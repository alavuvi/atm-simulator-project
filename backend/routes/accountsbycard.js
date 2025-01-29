const express = require('express');
const router = express.Router();
const cardaccount = require('../models/accountcard_model');

router.get('/:id', function (request, response) {
    accountcard.getAccountsByCard(request.params.id, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

module.exports = router;