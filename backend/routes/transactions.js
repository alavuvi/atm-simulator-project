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

router.get('/:accountid', function(request, response){
    transactions.getByaccountId(request.params.accountid,function(err, result){
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
            response.json(result.affectedRows);
        }
    })
});

router.put('/:id', function(request, response){
    transactions.update(request.params.id,request.body, function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result.affectedRows);
        }
    })
});

router.delete('/:id', function(request, response) {
    transactions.delete(request.params.id, function(err, result) {
        if(err){
            response.json(err);
        }
        else {
            response.json(result.affectedRows);
        }
    }) 

});

module.exports=router;