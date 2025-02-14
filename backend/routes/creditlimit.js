const express = require('express');
const router = express.Router();
const account = require('../models/account_model');

router.get('/:id', function (request, response) {
    account.getCreditLimit(request.params.id, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

module.exports = router;