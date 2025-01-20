const db=require('../database');

const transactions={
    getAll:function(callback){
        return db.query('SELECT * FROM transactions', callback);
    },
    getById:function(id, callback){
        return db.query('SELECT * FROM transactions WHERE idtransactions=?',[id],callback);
    }
}


module.exports=transactions;