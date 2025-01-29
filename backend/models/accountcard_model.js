const db = require('../database');

const accountcard = {
    getAllAccountCards(callback) {
        return db.query("SELECT * FROM accountcard", callback);
    },

    getOneAccountCard(id, callback) {
        return db.query("SELECT * FROM accountcard WHERE idcard=?", [id], callback);
    },

    addAccountCard(accontcard_data, callback) {
        return db.query("INSERT INTO accountcard (idaccount, idcard) VALUES(?,?)", [accountcard_data.id_account, accountcard_data.id_card], callback);
    },

    updateAccountCard(id, accountcard_data, callback) {
        return db.query("UPDATE accountcard SET idaccount=?, idcard=? WHERE idcard=?", [accountcard_data.id_account, accountcard_data.id_card, id], callback);
    },

    deleteCardAccount(id, callback) {
        return db.query("DELETE FROM accountcard WHERE idcard=?", [id], callback);
    },
};

module.exports = account;