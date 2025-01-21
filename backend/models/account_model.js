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
    }
}
module.exports = account;