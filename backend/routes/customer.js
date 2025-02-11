const express = require('express');
const router = express.Router();
const customer = require('../models/customer_model');

// Tämä hakee asiakkaan nimen id:n perusteella
router.get('/name/:accountId', function(req, res) {
    customer.getCustomerName(req.params.accountId, function(err, result) {
        if (err) {
            res.status(500).json(err);
        } else {
            const customerData = result[0];
            res.status(200).json({
                name: `${customerData.fname} ${customerData.lname}`
            });
        }
    });
});

router.get('/', function(req, res) {
    customer.getAll(function(err, dbResult) {
        if (err) {
            res.status(500).json(err);
        } else {
            res.json(dbResult);
        }
    });
});

router.get('/:id', function(req, res) {
    customer.getById(req.params.id, function(err, dbResult) {
        if (err) {
            res.status(500).json(err);
        } else if (dbResult.length > 0) {
            res.json(dbResult[0]);
        } else {
            res.status(404).json({ message: "Customer not found" });
        }
    });
});

router.post('/', function(req, res) {
    customer.add(req.body, function(err, dbResult) {
        if (err) {
            res.status(500).json(err);
        } else {
            res.status(201).json({ id: dbResult.insertId });
        }
    });
});

router.put('/:id', function(req, res) {
    customer.update(req.params.id, req.body, function(err, dbResult) {
        if (err) {
            res.status(500).json(err);
        } else if (dbResult.affectedRows > 0) {
            res.status(200).json({ message: "Customer updated successfully" });
        } else {
            res.status(404).json({ message: "Customer not found" });
        }
    });
});

router.delete('/:id', function(req, res) {
    customer.delete(req.params.id, function(err, dbResult) {
        if (err) {
            res.status(500).json(err);
        } else if (dbResult.affectedRows > 0) {
            res.status(200).json({ message: "Customer deleted successfully" });
        } else {
            res.status(404).json({ message: "Customer not found" });
        }
    });
});

module.exports = router;
