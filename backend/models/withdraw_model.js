const db = require('../database');

const withdrawModel = {
    nosto: function(idaccount, amount, callback) { 
        console.log(`Calling procedure nosto with params: idaccount=${idaccount}, amount=${amount}`);
        return db.query(
            'CALL nosto(?, ?)', 
            [idaccount, amount], 
            callback
        );
    }
};

module.exports = withdrawModel;