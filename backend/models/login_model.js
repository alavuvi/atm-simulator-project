const db = require('../database');

const login = {
    checkPin: function(idcard, callback) {
        return db.query(
            'SELECT pin FROM card WHERE idcard = ?', [idcard],callback
        );
    }
};

module.exports = login;