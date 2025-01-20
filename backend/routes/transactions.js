const express = require('express');
const router = express.Router();
const user = require('../models/transactions_model');
const transactions = require('../models/transactions_model');

router.get('/', function(request, response){
    transactions.getAll(function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result);
        }
    })
});

router.get('/:id', function(request, response){
    transactions.getById(request.params.id,function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result);
        }
    })
});

router.post('/', function(request, response){
    transactions.add(request.body, function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result);
        }
    })
});

module.exports=router;