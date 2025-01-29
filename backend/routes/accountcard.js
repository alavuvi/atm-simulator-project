const express = require('express');
const router = express.Router();
const accountcard = require('../models/accountcard_model');

router.get('/', function (request, response) {
    accountcard.getAllAccountCards(function (err, result) {
        if (err) {
            console.log(err);
            response.send(err);
        }
        else {
            console.log(result);
            response.json(result);
        }
    });
});

router.get('/:id', function (request, response) {
    accountcard.getOneAccountCard(request.params.id, function (err, result) {
        if (err) {
            console.log(err);
            response.send(err);
        }
        else {
            console.log(result);
            response.json(result);
        }
    });
});

router.post('/', function (request, response) {
    accountcard.addAccountCard(request.body, function (err, result) {
        if (err) {
            console.log(err);
            response.send(err);
        }
        else {
            console.log(result);
            response.json(result);
        }
    });
});

router.put('/:id', function (request, response) {
    accountcard.updateAccountCard(request.params.id, request.body, function (err, result) {
        if (err) {
            console.log(err);
            response.send(err);
        }
        else {
            console.log(result);
            response.json(result);
        }
    });
});

router.delete('/:id', function (request, response) {
    accountcard.deleteAccountCard(request.params.id, function (err, result) {
        if (err) {
            console.log(err);
            response.send(err);
        }
        else {
            console.log(result);
            response.json(result);
        }
    });
});

router.getAccountsByCard = function(cardnumber, callback) {
    const query = 'SELECT accountnumber, accounttype FROM account WHERE idcard = ?';
    db.query(query, [idcard], function(err, results) {
      if (err) {
        return callback(err, null);
      }
      callback(null, results);
    });
};

module.exports = router;