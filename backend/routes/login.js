const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/login_model');
// const card = require('../models/card_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');
dotenv.config();

router.post('/', function(request, response) {
  if(request.body.idcard && request.body.pin) {
    const idcard = request.body.idcard;
    const pincode = request.body.pin;
    
    login.checkPin(idcard, function(dbError, dbResult) {
      if(dbError) {
        return response.send("-11");
      }
      
      if (dbResult.length > 0) {
        console.log("idcard found");
        bcrypt.compare(pincode, dbResult[0].pin, function(err, compareResult) {
          if(!compareResult) {
            console.log("wrong pincode");
            return response.send("false");
          }

          console.log("success");
          const token = generateAccessToken({ username: idcard });
      
          response.setHeader('Content-Type', 'application/json'); 
          response.send(token);
        });
      } else { 
        console.log("idcard does not exist");
        return response.send("false");
      }
    });
  } else {
    console.log("idcard or pin missing");
    return response.send("false");
  }
});

function generateAccessToken(username) {
  return jwt.sign(username, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;