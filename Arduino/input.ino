void input_Init() {

  Serial.println("s,input init|");
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);
  pinMode(SELECT_BUTTON_PIN, INPUT);
  Serial.println("s,input init complete|");
}

void input_update() {

  int leftButtonVal = digitalRead(LEFT_BUTTON_PIN);
  int rightButtonVal = analogRead(RIGHT_BUTTON_PIN);
  int selectButtonVal = digitalRead(SELECT_BUTTON_PIN);
  
  // this code is for 'debouncing' the values of the buttons.
  // this should ensure that they do not fire multiple times on one press
  // for more info on what 'debouncing' is, see this article: http://whatis.techtarget.com/definition/debouncing  

  // has the left value changed?
  if (leftButtonVal != lastLeftButtonVal) {

    // was it pressed before changing?
    if (leftButtonPressed == false) {

      // mark it as pressed, call function
      leftButtonPressed = true;

      if (menuDataEntry == false) {

        menuIndex --;

        if (menuIndex < 0)
          menuIndex = 2; // 4; data pull and settings not implimented
        
      } else {
    
        dataEntryDelta --;
      }
      
    } else {

      // mark it as unpressed
      leftButtonPressed = false;
    }
  }

  // has the right value changed?
  if (abs(rightButtonVal - lastRightButtonVal) > 500) {
    
    if (rightButtonPressed == false) {

      // mark it as pressed, call function
      rightButtonPressed = true;

      if (menuDataEntry == false) {

        menuIndex ++;

        if (menuIndex > 2) // 4) data pull and settings not implimented
          menuIndex = 0;
        
      } else {
    
        dataEntryDelta ++;
      }
      
    } else {

      // mark it as unpressed
      rightButtonPressed = false;
    }
  }

  // has the select value changed?
  if (selectButtonVal != lastSelectButtonVal) {
    
    if (selectButtonPressed == false) {
  
      // mark it as pressed, call function
      selectButtonPressed = true;

      if (menuDataEntry == false) {

        menuDataEntry = true;
        menu_FinalizeMenuEntry();
        
      } else {
    
        menuDataEntry = false;
        menu_FinalizeDataEntry();
      }
      
    } else {

      // mark it as unpressed
      selectButtonPressed = false;
    }
  }

  // log last values
  lastLeftButtonVal = leftButtonVal;
  lastRightButtonVal = rightButtonVal;
  lastSelectButtonVal = selectButtonVal;
}

bool input_readExitState() {

  if (digitalRead(SELECT_BUTTON_PIN) == HIGH)
    return true;
  else
    return false;  
}

