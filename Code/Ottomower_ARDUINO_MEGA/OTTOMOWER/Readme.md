Upload this code to the Arduino in the perimeter wire transmitter station. 


//TODO:
- robot.noWireFound: indica se il robot sta seguendo il cavo oppure no...si può togliere?

- Fare unico metodo per aggiornare LCD

- usar char per 0/1 o numeri piccoli 1 byte of memory space and ranges from -128 to 127 or 0 to 255. 

- usare ifdefine invece di if(variabile) per i setup

- cos è trampoline cut?


TEST 2022-03-06
- la special find wire function fa qualcosa di strano, va in retro e non si sblocca più...sembra che ruoti di 0 gradi e non esce quando becca il cavo
- curve cavo a 90° impazzisce e continua ad andare dritto anche se su schermo segna che dovrebbe ruotare...forse se mag è alto non blocca la ruota opposta, controlla codice o aumentare la potenza di rotazione se è da tanto che non trova il cavo