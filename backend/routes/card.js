const express = require('express');
const router = express.Router();
const card = require('../models/card_model');

router.get('/', function(request, response){
    card.getAll(function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result);
        }
    });
});

router.get('/:id', function(request, response){
    card.getById(request.params.id, function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result[0]);
        }
    });
});

router.post('/', function(request, response) {
    card.add(request.body, function(err, result) {
        if(err){
            response.json(err);
        }
        else {
            response.json(result.affectedRows);
        }
    })
});

router.put('/:id', function(request, response) {
    card.update(request.params.id,request.body, function(err, result) {
        if(err){
            response.json(err);
        }
        else {
            response.json(result.affectedRows);
        }
    })
});
router.delete('/:id', function(request, response) {
    card.delete(request.params.id, function(err, result) {
        if(err){
            response.json(err);
        }
        else {
            response.json(result.affectedRows);
        }
    }) 

});


module.exports = router;