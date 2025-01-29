const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/login_model');
const card = require('../models/card_model');
const account = require('../models/accountcard_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');
dotenv.config();

router.post('/', function(request, response) {
  if(request.body.idcard && request.body.pin){
    const idcard = request.body.idcard;
    const pincode = request.body.pin;
    
    card.checkPin(idcard, function(dbError, dbResult) {
      if(dbError){
        return response.send("-11");
      }
      
      if (dbResult.length === 0) {
        console.log("card does not exists");
        return response.send("false");
      }

      bcrypt.compare(pincode, dbResult[0].pin, function(err, compareResult) {
        if(!compareResult) {
          console.log("wrong pincode");
          return response.send("false");
        }

        console.log("success");
        const token = generateAccessToken({ username: idcard });
     
        account.getAccountsByCard(idcard, function(err, accountResult) {
        if(accountError){
          return response.send("-12");
          }
          
          if(accountResult.length === 0){
            return response.send("-13");
          }


          return response.json({
            token: token,
            accounts: accountResult
          });
        });
      });
    });
  }
  else {
    console.log("idcard or pin missing");
    return response.send("false");
  }
});

function generateAccessToken(username) {
  return jwt.sign(username, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports=router;