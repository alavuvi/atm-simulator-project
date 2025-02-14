const db=require('../database');
const bcrypt=require('bcryptjs');

const card={
    getAll:function(callback){
        return db.query("SELECT * from card",callback);
    },
    getById:function(id, callback){
        return db.query("SELECT * from card where idcard=?",[id],callback);
    },
    add:function(card_data,callback){
        bcrypt.hash(card_data.pin, 10, function(err, hashed_pin){
           return db.query('INSERT INTO card(cardnumber,pin,active) VALUES(?,?,?)',
           [card_data.cardnumber, hashed_pin, card_data.active], 
           callback);   
        });
    },
    update:function(id,card_data,callback){
        bcrypt.hash(card_data.pin, 10, function(err, hashed_pin){
            return db.query('UPDATE card SET cardnumber=?, pin=?, active=? WHERE idcard=?',
            [card_data.cardnumber, hashed_pin, card_data.active, id],
            callback);
        });
    },
    delete:function(id,callback){
        return db.query('DELETE FROM card WHERE idcard=?',[id],callback);
    },
    updateCardStatus: function(id, active, callback){
        return db.query('UPDATE card SET active=? WHERE idcard=?',
            [active, id],
            callback);
        },
    };
module.exports=card;