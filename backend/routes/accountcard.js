const express = require('express');
const router = express.Router();
const accountcard = require('../models/accountcard_model');

router.get('/', function (request, response) {
    accountcard.getAll(function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

router.get('/:id', function (request, response) {
    accountcard.getById(request.params.id, function (err, result) {
        if (err) {
            response.send(err);
        } else {
            response.json(result);
        }
    });
});

router.post('/', function (request, response) {
    accountcard.add(request.body, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

router.put('/:id', function (request, response) {
    accountcard.update(request.params.id, request.body, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

router.delete('/:id', function (request, response) {
    accountcard.delete(request.params.id, function (err, result) {
        if (err) {
            response.send(err);
        }
        else {
            response.json(result);
        }
    });
});

module.exports = router;