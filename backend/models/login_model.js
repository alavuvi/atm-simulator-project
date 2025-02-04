const db=require('../database');
const bcrypt=require('bcryptjs');

const card={
    checkPin: function(idcard,callback){
        return db.query('SELECT pin FROM card WHERE idcard=?',[idcard],callback);
    },
}