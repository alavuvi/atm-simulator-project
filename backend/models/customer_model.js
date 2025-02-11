const db = require('../database');

const customer = {
    getAll: function(callback) {
        return db.query('SELECT * FROM customer', callback);
    },
    getById: function(id, callback) {
        return db.query('SELECT * FROM customer WHERE idcustomer = ?', [id], callback);
    },
    add: function(data, callback) {
        return db.query(
            'INSERT INTO customer (fname, lname, address, phone) VALUES (?, ?, ?, ?)',
            [data.fname, data.lname, data.address, data.phone],
            callback
        );
    },
    update: function(id, data, callback) {
        return db.query(
            'UPDATE customer SET fname = ?, lname = ?, address = ?, phone = ? WHERE idcustomer = ?',
            [data.fname, data.lname, data.address, data.phone, id],
            callback
        );
    },
    delete: function(id, callback) {
        return db.query('DELETE FROM customer WHERE idcustomer = ?', [id], callback);
    },
    getCustomerName: function(accountId, callback) {
        return db.query(
            'SELECT customer.fname, customer.lname FROM customer ' +
            'JOIN account ON customer.idcustomer = account.idcustomer ' +
            'WHERE account.idaccount = ?',
            [accountId],
            callback
        );
    }
};

module.exports = customer;
