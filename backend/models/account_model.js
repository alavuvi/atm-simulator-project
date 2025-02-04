const db = require('../database');
const account = {
    getAll: function(callback) {
        return db.query('SELECT * FROM account', callback);
    },

    getById: function(id, callback) {
        return db.query('SELECT * FROM account WHERE idaccount=?', [id], callback);
    },

    add: function(data, callback){
        return db.query(
            'INSERT INTO account(idcustomer, balance, creditlimit, accountnumber, accounttype) VALUES(?,?,?,?,?)',
            [data.idcustomer, data.balance, data.creditlimit, data.accountnumber, data.accounttype],
            callback);
    },

    update: function(id, account, callback) {
        return db.query(
            'UPDATE account SET balance = ?, creditlimit = ?, accountnumber = ?, accounttype = ? WHERE idaccount = ?',
            [account.balance, account.creditlimit, account.accountnumber, account.accounttype, id],
            callback
        );
    },
    delete: function(id, callback) {
        return db.query('DELETE FROM account WHERE idaccount = ?', [id], callback);
    },
    getCreditLimit: function(idaccount, callback) {
        return db.query('SELECT creditlimit FROM account where idaccount=?', [idaccount], callback);
    },
};

module.exports = account;