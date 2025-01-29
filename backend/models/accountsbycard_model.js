const db = require('../database');

exports.getAccountsByCard = function(cardnumber, callback) {
  const query = 'SELECT accountnumber, accounttype FROM account WHERE cardnumber = ?';
  db.query(query, [cardnumber], function(err, results) {
    if (err) {
      return callback(err, null);
    }
    callback(null, results);
  });
};