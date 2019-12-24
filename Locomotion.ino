#define IRlowspd 80
#define IRhighspd 150
#define IRbackspd 40

#define IRlow 30
#define IRhigh 110

#define FRONTLIM 200
#define LEFTLIM 25
#define BACKLIM 25
#define BACKGOAL 15
#define RIGHTLIM 25

/*
boolean hasBall() {
   return int(analogRead(LDR))<ldrthres;
}
*/

void trackball() {
  int tempspd=0, tempdir=0;
  int rangeL=getRangeL(), rangeB=getRangeB(), rangeR=getRangeR();
  //Serial.println(String(rangeL)+"\t"+String(rangeB)+"\t"+String(rangeR));
  
  if (bearing<=180 && bearing>compTolerance) northing = pow((bearing/20),2)+12;
  else if (bearing>180 && bearing<360-compTolerance) northing = -(pow(((bearing-360)/20),2)+12);
  else northing = 0;
  /*
  if (rangeL > LEFTLIM*1.2 && rangeR > RIGHTLIM*1.2) { // Within goal
    BACKLIM = BACKGOAL;
  }
  */
  if (IRstr < IRlow) { // Ball not detected
    /*
    tempspd = 0;
    tempdir = 0;
    */
    
    //tempspd = IRlowspd;
    
    if (rangeL > LEFTLIM && rangeR > RIGHTLIM) { //behind goal post
      if(rangeB > BACKLIM) {
        tempdir = 180;
        tempspd = IRlowspd;
      }
      else {
        tempdir = 0;
        tempspd = 0;
      }
    }
    else {
      if(rangeB > BACKLIM*1.5) {
        tempdir = 180;
        tempspd = IRlowspd;
        
        if (rangeL < LEFTLIM) {
          tempdir = 135;
          tempspd = IRlowspd;
        }
        else if (rangeR < RIGHTLIM) {
          tempdir = 225;
          tempspd = IRlowspd;
        }
      }
      else {
        tempspd = 0;
        tempdir = 0;
      }
    }
  }
  else if (IRstr >= IRlow && IRstr < IRhigh) { // Ball detected far away
    
    tempspd = IRlowspd;
    
    if (IRFB == 0) { // Ball in front
      if (IRdir == 1) tempdir = 110;
      else if (IRdir == 2) tempdir = 90;
      else if (IRdir == 3) tempdir = 55;
      else if (IRdir == 4) {tempdir = 0; tempspd = IRhighspd;}
      else if (IRdir == 5) tempdir = 305;
      else if (IRdir == 6) tempdir = 270;
      else if (IRdir == 7) tempdir = 250;
    }
    else { // Ball behind
      if (IRdir == 1) tempdir = 250;
      else if (IRdir == 2) tempdir = 215;
      else if (IRdir == 3) tempdir = 165;
      else if (IRdir == 4) tempdir = 140;
      else if (IRdir == 5) tempdir = 195;
      else if (IRdir == 6) tempdir = 145;
      else if (IRdir == 7) tempdir = 110;
    }
  }
  else if (IRstr >= IRhigh) { // Ball close
    tempspd = IRlowspd;
    /*
    if (hasBall()) {
      if (IRdir == 4) { // Ball directly in front and in possession
        tempdir = 0;
        tempspd = IRhighspd;
      }
    }
    */
    //else { // Ball not in possession
      if (IRFB == 0) { // Ball in front
        if (IRdir == 1) tempdir = 105;
        else if (IRdir == 2) tempdir = 95;
        else if (IRdir == 3) tempdir = 40;
        else if (IRdir == 4) {tempdir = 0; tempspd = IRhighspd;}
        else if (IRdir == 5) tempdir = 320;
        else if (IRdir == 6) tempdir = 265;
        else if (IRdir == 7) tempdir = 255;
      }
      else { // Ball behind
        tempspd = IRhighspd;
        if (IRdir == 1) tempdir = 220;
        else if (IRdir == 2) tempdir = 190;
        else if (IRdir == 3) tempdir = 120;
        else if (IRdir == 4) {
          
          if (rangeL < rangeR) tempdir = 260;
          else tempdir = 100;
          
          //tempdir = 260;
          //tempspd = IRhighspd;
        }
        else if (IRdir == 5) tempdir = 240;
        else if (IRdir == 6) tempdir = 170;
        else if (IRdir == 7) tempdir = 160;
      }
    //}
  }
  
  
  if (rangeL <= LEFTLIM) { // Too left
    tempspd = IRlowspd;
    
    if (tempdir > 180) tempdir = 90;
  }
  else if (rangeB <= BACKLIM) {
    tempspd = IRlowspd;
    
    if (tempdir > 90 && tempdir < 270) tempdir = 0;
  }
  else if (rangeR <= RIGHTLIM) {
    tempspd = IRlowspd;
    
    if (tempdir < 180) tempdir = 270;
  }
  else if (rangeB >= FRONTLIM) { // Too front
    tempspd = IRlowspd;
    
    if (tempdir > 270) tempdir = 260;
    else if (tempdir < 90) tempdir = 100;
    else if (tempdir == 0) tempdir = 180;
  }
  else if (rangeB <= BACKLIM) { // Too back
    tempspd = IRlowspd;
    
    if (tempdir > 90 && tempdir < 270) tempdir = 0;
  }
  else if (rangeL > LEFTLIM && rangeR <= RIGHTLIM) { // Too right
    tempspd = IRlowspd;
  
    if (rangeB > BACKLIM) { // Within BACKLIM
      if (tempdir > 10 && tempdir < 90) tempdir = 0;
      else if (tempdir >= 90 && tempdir < 180) tempdir = 180;
    }
    else { // (rangeB <= BACKLIM) Too back
      if (tempdir > 10 && tempdir < 90) tempdir = 0;
      else if (tempdir >= 90 && tempdir < 270) tempdir = 330;
    }
  }
  else if (rangeR > RIGHTLIM && rangeL <= LEFTLIM) { // Too left
    tempspd = IRlowspd;
  
    if (rangeB > BACKLIM) { // Within BACKLIM
      if (tempdir > 270 && tempdir <= 350) tempdir = 0;
      else if (tempdir > 190 && tempdir <= 270) tempdir = 180;
    }
    else { // (rangeB <= BACKLIM) // Too back
      if (tempdir > 270 && tempdir <= 350) tempdir = 0;
      else if (tempdir >= 90 && tempdir <= 270) tempdir = 30;
    }
  }
  else if (rangeL <= LEFTLIM && rangeR <= RIGHTLIM) { // Sandwich
    tempspd = IRlowspd;
    if (tempdir < 90 || tempdir > 270) tempdir = 0;
    else if (tempdir >= 90) tempdir = 180;
  }
  
  /*  
  // Ultrasonic Limitation
    if (rangeR<=RIGHTLIM) {
      if (tempdir>0 && tempdir<=90) tempdir=345;
      else if (tempdir>90 && tempdir<180) tempdir=195;
      //Serial.println(tempdir);
    } else if (rangeR>=LEFTLIM) {
      if (tempdir<360 && tempdir<270) tempdir=15;
      else if (tempdir<270 && tempdir>180) tempdir=165;
      //Serial.println(tempdir);
    }
    /*
    if (rangeB=>FRONTLIM) {
      tempdir=-1;
    }// else if (rangeB<=BACKLIM) tempdir=-1;
    
    if (tempdir!=-1) setMove(tempspd,tempdir);
    else setMove(0,0);
    
    } else {
      if (rangeR<=RIGHTGOALLIM) setMove(IRhighspeed*1.2, 270);
      else if (rangeR>=LEFTGOALLIM) setMove(IRhighspeed*1.2, 180);
      else setMove(IRhighspeed*1.2,180);
      //setMove(0,0);
    }
  }
  */
  setMove(tempspd,tempdir);
  //Serial.print(tempspd);Serial.print("\t");Serial.println(tempdir);
}
