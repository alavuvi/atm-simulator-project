const express = require('express');
const router = express.Router();
const accountcard = require('../models/account_model');

router.get('/:id', function (request, response) {
    accountcard.getCreditLimit(request.params.id, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

module.exports = router;