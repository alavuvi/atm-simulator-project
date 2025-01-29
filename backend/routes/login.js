const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/login_model');
const card = require('../models/card_model');
const account = require('../models/accountsbycard_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');
dotenv.config();

router.post('/', 
  function(request, response) {
    if(request.body.cardnumber && request.body.pin){
      const cardnumber = request.body.cardnumber;
      const pincode = request.body.pin;
      
        card.checkPin(cardnumber, function(dbError, dbResult) {
          if(dbError){
            response.send("-11");
          }
          else{
            if (dbResult.length > 0) {
              bcrypt.compare(pincode,dbResult[0].pin, function(err,compareResult) {
                if(compareResult) {
                  console.log("success");
                  const token = generateAccessToken({ username: cardnumber });
                  response.send(token);
                
                // Haetaan kortin tietoihin liittyvät tilit
                  account.getAccountsByCard(cardnumber, function(accountError, accountResult) {
                    if(accountError){
                      response.send("-12"); 
                }
                else{
                  if(accountResult.length > 0){
                    // Lähetetään token ja tilitiedot
                    response.json({
                      token: token,
                      accounts: accountResult
                    });
                  }
                  else{
                    // Ei liitettyjä tilejä
                    response.send("-13"); 
                  }
                }
              });
            }
            else {
              console.log("wrong pincode");
              response.send(false);
            }			
          });
        }
        else{
          console.log("card does not exists");
          response.send(false);
        }
      }
    });
  }
  else{
    console.log("cardnumber or pin missing");
    response.send(false);
    }
  }
);

function generateAccessToken(username) {
  return jwt.sign(username, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports=router;