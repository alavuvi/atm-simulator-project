const db = require('../database');

const accountcard = {
    getAll:function(callback) {
        return db.query("SELECT * FROM accountcard", callback);
    },

    getById:function(id, callback) {
        return db.query("SELECT * FROM accountcard WHERE idcard=?", [id], callback);
    },

    add:function(accountcard_data, callback) {
        return db.query('INSERT INTO accountcard (idcard, idaccount) VALUES(?,?)', [accountcard_data.idaccount, accountcard_data.idcard], callback);
    },

    update:function(id, accountcard_data, callback) {
        return db.query("UPDATE accountcard SET idaccount=?, idcard=? WHERE idcard=?", [accountcard_data.idaccount, accountcard_data.idcard, id], callback);
    },

    delete:function(id, callback) {
        return db.query("DELETE FROM accountcard WHERE idcard=?", [id], callback);
    },
    getAccountsByCard:function(idcard, callback){
        return db.query('SELECT idaccount FROM accountcard where idcard=?', [idcard], callback);
    },
};

module.exports = accountcard;