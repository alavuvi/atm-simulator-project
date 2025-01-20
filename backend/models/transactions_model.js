const db=require('../database');

const transactions={
    getAll:function(callback){
        return db.query('SELECT * FROM transactions', callback);
    },
    getById:function(id, callback){
        return db.query('SELECT * FROM transactions WHERE idtransactions=?',[id],callback);
    },
    add:function(data, callback){
        return db.query("insert into transactions(idaccount, datetime, transaction, amount) values(?,?,?,?)", [data.idaccount, data.datetime, data.transaction, data.amount], callback);
    }
}


module.exports=transactions;