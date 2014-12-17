/*
Nucleotide_Control_RGB_LEDs V 1.0
 
 Licence: Creative Commons Attribution-ShareAlike 4.0 International
 https://creativecommons.org/licenses/by-sa/4.0/legalcode
 
 Reads through a gene sequence one nucleotide at a time and blinks RGB LED (strips) according to the
 LED-Pin binding specified in the initiation.
 
 Currnet mappings are:
 A->red, digital pin 3
 T->Purple (red+blue), digital pins 3 & 6
 C->blue, 
 G (or undefined) -> green.
 
 created 14 Dec 2014
 by Jens E. Eriksson
 
 my email is my first name plus the three first letters from my surname
 at the webmail service provided by internet giant that starts with a G 
 
 Inspired by the Fading Arduino example sketch by Tom Igoe 
 
 A schematic wiring diagram of the circuit is available from the GitHub repository.
 
 
 */

#include <avr/pgmspace.h> // library for accessing data stored in the 32 kB flash memory on the Atmega328 chip 

int green = 5;   // green lead from LED strip controlled by PWM pin 5 via a TIP120 NPN darlington transistor 
int red = 3 ;    // red   lead from LED strip controlled by PWM pin 3 via a TIP120 NPN darlington transistor
int blue = 6;    // blue  lead from LED strip controlled by PWM pin 6 via a TIP120 NPN darlington transistor

/* The time (in ms) that the fade in/out cycle takes can be calculated from
 2*pause*max_intensity/gradient. Each incremental brightnes level is displayed
 for pause ms and there are two (max_intensity/gradient) brightnes levels in total per cycle 
 */
 
int gradient = 1; //step size for the fade in/out functions, sets smoothnes of fade (1 is smoothest fade)
int pause = 5;  //ms delay per PWM step, sets speed of fading in/out
int max_intensity = 255; //maximum intensity of LEDs 0-255
int stay_on = 500; //how many ms the LEDs stay lit at max intensity
int stay_off = 50; //how many ms the LEDs stay off between cycles

int k = 0; //Conuter for accessing the nucleotides in the "gene" character array, stored in flash program memory 

/*
 Nucleotide sequences are often too long to be stored in the 2 kB SRAM on the Atmega 328 chip,
 so the "gene" variable has to be stored in flash program memory and read out character by character at runtime
 PROGMEM does this for us with the help of the "k" counter variable.
 This particular nucleotide sequence is the 
 rpoB RNA polymerase beta subunit from Picea abies (Norway spruce, NCBI Gene ID: 16185436) 
 If you drive yor christmas lights with this sequence you have created
 the most meta christmas tree ever!
 */

prog_uchar gene[] PROGMEM  = {
  "ATGCGATTAGATGAAAATGAGGGAGCGTTTACAATCCCCGAATTTGGTAAGATACAATTTGAAGGATTTT"
    "GTCGTTTCATCGATCAGGGCTTGATGGAAGAACTCCATAATTTTCCGAAAATTGAGGATACAGATAAAGA"
    "AATTGAATCCAGGCTTTTTGGTAATGAATATGAATTAGCAGAACCTTTTATAAAAGAGAGAGATGCTGTA"
    "TATCAGTCCCTTACATATTACTCTGAATTATATGTACCAGCAAGATCGATTCGGAGGAATAGTAGTAAGA"
    "TACAGAAACAAACTGTATTTCTCGGAAATATTCCTTTAATGAATTCCCATGGAACGTTTGTAGTAAATGG"
    "AATTTACAGAATCGTGGTGAATCAGATATTAATAAGTCCCGGTATTTATTACCGTTCAGAATTAGACCAT"
    "AATAGAATAAACTATATATATACTGGCACTCTGATTTCAGATTGGGGAAGAAGATCGAAATTAGAGATCG"
    "ATGTGGGAGAAAGGATATGGGCTCGTGTAAGCAGAAAACGAAAAATATCTATTCCAGTTCTATTATCAGC"
    "TATGGGTTTAAATCTCGAAGAGATTCTGGACAATACTCGCTATCCCGAAAAATTTTTCTTTTTACTGAAA"
    "AAGAAGAAGGGGAGGTGGGAGCGGGAGGAATATATCTGGTCGAAGGAAAAAGCCATTCTGGAGTTTTATA"
    "AAAAACTCTACTGTGTAAGTGGCGATTTGGTATTTTCCGAGTCTCTATGTAAAGAATTGCAGGAAAAATT"
    "TTTCCGACAAAGATGTGAATTGGGAAAGATTGGTCGACGAAATCCGAACCAAAAACTGAATCTTGATATA"
    "CCCGAGAACGAGATTTTTTCATTGCCACAAGATGTATTGGCTGCTGTGGATTACCTTATCGGAGTGAAAT"
    "TTGGAATGGGTACACTCGATGATATAGATCACCTTAGAAATCGACGTATTCGTTCTGTAGCAGATTTATT"
    "ACAGAATCAATTCAGATTAGCTCTAGGTCGTTTGGAAGATGCAGTTCGAAGAACTATACACAGAGCAACC"
    "AAGCGTAGATCAACTCCTCAGAACTTGGTAACTTCAACTCTATTAAAAAACACTTTTCAAGATTTTTTTG"
    "GTTCACACCCCTTATCCCAATTTTTAGATCAAACTAATCCATTGACGGAAATAGCTCATGGGCGAAAATT"
    "GAGCCATTTAGGCCCTGGGGGCTTAACAGGGCGAACTGCTAGTTTTCGGACACGGGATATTCATCCCAGT"
    "TATTATGGGCGTATTTGTCCAATCGATACGTCCGAAGGAATGAATGCTGGACTTGTTGCATCATTATCTA"
    "TTCATGCAAAGATTGGTCAGTGTGGTTCTTTACAAAGTCCATTCTATAAGATCTCTGAGAGATCGAGAGA"
    "AGAACATATGGTTTATCTATTACCGGGAGAAGATGAGGATGAATACTATCGGATAGCTACGGGAAATTCT"
    "TTGGCGTTAAATCAAGGGATTCAAGAGGAACAAATTACTCCAGCTCGATACCGACAAGAATTTATAGTTA"
    "TTGCATGGGAACAAATCCATTTCAGAAGTATTTTTCCTTTCCAATATTTTTCTGTTGGAGTTTCCCTTAT"
    "TCCTTTTCTCGAACATAATGATGCGAATCGCGCTCTAATGGGTTCTAATATGCAGCGTCAAGCAGTTCCA"
    "CTTTTTCAACCCGAGAAGTGCATTGCCGGAACTGGGTTGGAAGGTCAAGCAGCTCTAGATTCAGGAAGTG"
    "TAGCTATAGCTACACAAGAGGGAAGGATCGAGTATATCGATGCTGTAAATATCACTTCATCGGTTAATGG"
    "AGACACTGTACGAACAGAATTGGTTATATATCAACGTTCTAATACCAATACTTGTACGCATCAAAAACCT"
    "CAAGTTCGTCAAGGGGAATGTGTAAAGAAGGGACAAATTCTGGCGGACGGTGCGGCTACGGTAGGGGGAG"
    "AACTCTCTTTGGGAAAAAACGTTTTAGTAGCTTATATGCCATGGGAAGGATACAATTTCGAAGACGCAAT"
    "ACTCATTAGTGAACGTCTGGTATATGAAGATATTTATACCTCTTTCCATATCGTAAGGTACAGGATTGAA"
    "ATCTGTATGACGAGCCAGGGTCCTGAAAGAATCACTAGAGAAATACCTCATTTAGATGCTCATTCACTTC"
    "GTCATTTGGACGAAAATGGTCTTGTAATGCTAGGATCTTGGATAGAAACAGGTGATGTTTTGGTAGGTAA"
    "ATTAACGCCTCAAACAACAGAAGAATCATTATGTGCCCCGGAAGGAAGATTATTACAAACCATATTTGGT"
    "ATTGAGGTATCCACTGCGAGAGAAAATTGTCTCAGAACACCTATAGGTGGAAGAGGTCGAGTTATTGATG"
    "TGAGATGGATCAATAGAGTAGATGATTCCGGTGATAATGCGGAAACAGTCCACGTATATATATCACAAAA"
    "ACGTAAGATACAAGTGGGTGATAAAGTAGCTGGAAGGCATGGTAATAAAGGTATTATTTCAATAGTTTTG"
    "CCCAGACAAGATATGCCCTATTTGCAAAATGGAATACCAGTTGATATGGTATTGAATCCATTAGGGGTAC"
    "CTTCACGAATGAATGTAGGACAGATCTTTGAATGTTTGCCCGGTTTAGCAGGAAACCCGATGAACAAACA"
    "TTATAGAATAACACCTTTTGACGAAAGATACGAGCGAGAAGCTTCGAGAAAACTAGTGTTTCCTGAGCTT"
    "TATAAAGCTAGTGAGCAAACAGCTAATCCATGGGTATTCGAACCGGATCATCCTGGAAAACATAGATTAA"
    "TCGATGGAAGAACAGGAGATGTTTTTGAACAACCTGTTACAATAGGAAAAGCTTATATGTCGAAATTGAG"
    "TCATCAAGTTGATGATAAAATACATGCACGTTCGAGTGGACCTTATGCACGGGTTACACAACAACCTCTT"
    "AGAGGAAAATCCAAACGAGGGGGGCAACGAATAGGAGAAATGGAAGTTTGGGCTCTAGAAGGTTTTGGTG"
    "TTGCTTATATTTTACAAGAGATGCTTACTCTCAAATCTGACCATATTAGAACTCGTAATGAAGTACTTGG"
    "TGCCATTATCACTGGAGGGCCAATACCTAAACCTGACACTGCTCCAGAATCTTTCCGATTGCTCATTCGA"
    "GAATTACGATCTTTAGCTCTAGAATTGAATCATGCTATTATATCTGAGAAAGACTTTCAGATAGATAGAG"
    "AGGAAGTTTGA"};

void setup()  { 
  //nothing to setup
}

/*
The fadeIn/fadeOut functions do a PWM sweep from 0 to "max_intensity" on the specified pin with a smoothnes gradient of "gradient"
fadeIn_Two/fadeOut_Two do the same thing, but on two pins simultaneously.
*/

void fadeIn(int pin, int max_intensity, int gradient){
  for(int fadeValue = 0 ; fadeValue <= max_intensity; fadeValue +=gradient) { 
    // sets the PWM value (range from 0 to max_intensity with a step size of gradient):
    analogWrite(pin, fadeValue);
    // wait for pause milliseconds to create the dimming effect    
    delay(pause);                            
  }
}

void fadeOut(int pin, int max_intensity, int gradient){
  for(int fadeValue = max_intensity; fadeValue >= 0; fadeValue -=gradient) { 
    // sets the PWM value (range from max_intensity to 0 with a step size of gradient):
    analogWrite(pin, fadeValue);  
    // wait for pause milliseconds to see the dimming effect    
    delay(pause);     
  }
}

void fadeIn_Two(int pin1, int pin2, int max_intensity, int gradient){
  for(int fadeValue = 0 ; fadeValue <= max_intensity; fadeValue +=gradient) { 
    // sets the value of two pins simultaneously (range from 0 to max_intensity with a step size of gradient):
    analogWrite(pin1, fadeValue);
    analogWrite(pin2, fadeValue);
    // wait for pause milliseconds to create the dimming effect    
    delay(pause);                            
  }
}

void fadeOut_Two(int pin1, int pin2, int max_intensity, int gradient){
  for(int fadeValue = max_intensity; fadeValue >= 0; fadeValue -=gradient) { 
    // sets the value of two pins simultaneously (range from max_intensity to 0 with a step size of gradient):
    analogWrite(pin1, fadeValue);
    analogWrite(pin2, fadeValue);  
    // wait for pause milliseconds to see the dimming effect    
    delay(pause);     
  }
}


void sequencer(char nucleotide) {
  /* Function to translate single nucloetide characters in to pretty
   LED strip blinking. Currently the mapping is A->red, T->Purple (red+blue),
   C->blue, G (or undefined) -> green.
   */

  if (nucleotide == 'A'){
    fadeIn(red, max_intensity, gradient);
    delay(stay_on);
    fadeOut(red, max_intensity, gradient);
    delay(stay_off);
  }
  if (nucleotide == 'T'){
    fadeIn_Two(red, blue, max_intensity, gradient);
    delay(stay_on);
    fadeOut_Two(red, blue, max_intensity, gradient);
    delay(stay_off);
  }  
  if (nucleotide == 'C'){
    fadeIn(blue, max_intensity, gradient);
    delay(stay_on);
    fadeOut(blue, max_intensity, gradient);
    delay(stay_off);
  }
  else {
    fadeIn(green, max_intensity, gradient);
    delay(stay_on);
    fadeOut(green, max_intensity, gradient);
    delay(stay_off);
  }
} 

/* The main program loop reads one character at a time from the character array (=string) called
 "gene", which is stored in flash memory. When it reaches the end of the gene it sets k to 0, and does a 10 s
 all LEDs on signal to celebrate, then it resumes from k = 0. 
 */

void loop()  { 

  char nucleotide = pgm_read_byte_near(gene + k); // reads the character at position "k" from the "gene" array in to the variable "nucleotide"
  k++; //increments the counter "k" by 1 
  sequencer(nucleotide); // sends the nucleotide to the "sequencer" function which runs a fade cycle on the corresponting LEDs

  // When the loop reaches the end of "gene", it turns on all LEDs for 10 s, resets "k" to 0, and restarts.
  if (k == (sizeof(gene)-1)){

    fadeIn(red, max_intensity, 1);
    fadeIn(blue, max_intensity, 1);
    fadeIn(green, max_intensity, 1);

    delay(10000);

    fadeOut(red, max_intensity, 1);
    fadeOut(blue, max_intensity, 1);
    fadeOut(green, max_intensity, 1);

    k=0;
  } 
}


