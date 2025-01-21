const db=require('../database');
const bcrypt=require('bcryptjs');

const card={
    checkPin:function(cardnumber,callback){
        return db.query('SELECT pin FROM card WHERE cardnumber=?',[cardnumber],callback);
    },
}