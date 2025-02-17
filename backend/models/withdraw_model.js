const db = require('../database');

const withdrawModel = {
    getAccountByCard: function(idcard, callback) {
        return db.query(
            'SELECT idaccount FROM accountcard WHERE idcard = ?',
            [idcard],
            callback
        );
    },

    nosto: function(idaccount, idcard, amount, callback) {
        console.log(`Calling procedure nosto with params: idaccount=${idaccount}, idcard=${idcard}, amount=${amount}`);
        return db.query(
            'CALL nosto(?, ?, ?)',
            [idaccount, idcard, amount],
            callback
        );
    }
};

module.exports = withdrawModel;
