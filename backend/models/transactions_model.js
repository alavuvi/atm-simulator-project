const db=require('../database');

const transactions={
    getAll:function(callback){
        return db.query('SELECT * FROM transactions', callback);
    },
    getByid:function(id, callback){
        return db.query('SELECT date_format(datetime, "%Y-%m-%d %H:%i:%s") as "datetime", transaction, amount FROM transactions WHERE idaccount=?',[id],callback);
    },
    add:function(transactions_data, callback){
        return db.query("insert into transactions(idaccount, datetime, transaction, amount) values(?,?,?,?)", [transactions_data.idaccount, transactions_data.datetime, transactions_data.transaction, transactions_data.amount], callback);
    },
    update:function(id, transactions_data, callback){
        return db.query('UPDATE transactions SET idaccount=?, datetime=?, transaction=?, amount=? WHERE idtransactions=?',[transactions_data.idaccount, transactions_data.datetime, transactions_data.transaction, transactions_data.amount, id], callback);
    },
    delete:function(id, callback){
        return db.query('DELETE FROM transactions WHERE idtransactions=?', [id], callback);
    }
}


module.exports=transactions;