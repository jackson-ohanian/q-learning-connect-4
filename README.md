# q-learning-connect-4
Q reinforcement learning applied to the connect 4 game. 


About the Training

  Double Q Learning, two AIs compete for given epochs, focused on training one and using the other to influence that training. 

  A convulation-like filter is put over the board (stride=1, padding=0) the larger the size the more computational power and memory is needed.. to do a full 7x6 
  board is pretty signifigant (some billions of boards from quick math), anything under 4 is borderline useless. More info in header documentation.
  
  Training is epsilon greedy to encourage exploratory action. High decay rate to give the AI more freedom to make multi-turn plays. 
  
  While focused on training the dominant AI (red), I have been increasing the chance of non-greedy action in the second AI to ensure some different plays come up 
  through randomness. 
  
  
