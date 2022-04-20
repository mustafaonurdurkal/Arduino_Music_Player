#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 4  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins

#define path "/"


  TMRpcm audio;   // create an object for use in this sketch 

  String *songs;
  File dir;
  int nSongs = 0;
  bool sari=true;
  bool yesil=true;
  bool mavi=true;



int receiveInt;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);

  
  digitalWrite(8,HIGH);
  audio.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(10,OUTPUT);  //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45
  
  Serial.begin(9600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  
        Serial.println("SD not OK"); 

    return;
  }else{
    Serial.println("SD OK"); 
  }
  dir = SD.open(path);
  nSongs = numberOfSongs(dir,nSongs);
  songs = listSongs(dir);
  audio.volume(100);

}


void loop() {
  while(Serial.available()>0){
    receiveInt=Serial.parseInt();
    
    if(receiveInt==-1){
       Serial.println("Songs List:");
       for(int i = 1; i < nSongs; i++){
         Serial.println("   "+songs[i]);
         }
      }

      else if(receiveInt==-2){
          if(sari){
            digitalWrite(2,HIGH);
            sari= !sari;
          }
          else{
            digitalWrite(2,LOW);
           sari= !sari;
        }        
       }
      
      else if(receiveInt==-3){
        if(yesil){
          digitalWrite(3,HIGH);
          yesil= !yesil;
          }
        else{
          digitalWrite(3,LOW);
          yesil= !yesil;
        }
      }
     
     else if(receiveInt==-4){
          if(mavi){
            digitalWrite(5,HIGH);
            mavi= !mavi;
          }
          else{
            digitalWrite(5,LOW);
           mavi= !mavi;
        }        
       }
        
      else if(receiveInt==0){}
      
      else if(receiveInt < nSongs){
        int n = songs[receiveInt].length();
      char char_array[n + 1];
      strcpy(char_array,songs[receiveInt].c_str());
      
        audio.play(char_array);
       
         dir.rewindDirectory();
        Serial.print("        "+songs[receiveInt]);
        Serial.println(" caliyor...");
        }
  }

}


String* listSongs(File folder){
  Serial.print("Songs found:");
  Serial.println(nSongs-1);

 String *songList = new String[nSongs];

  Serial.println("Songs List:");

  for(int i = 0; i < nSongs; i++){
    File entry = folder.openNextFile();
    songList[i] = entry.name();
    entry.close();
    if(i>0){
      Serial.print(i);
      Serial.println("    "+songList[i]);
      }
    
  }
   folder.rewindDirectory();
   return songList;
}


int numberOfSongs(File folder, int nSongs){

  while(true){
    File entry = folder.openNextFile();
    if(!entry){
      folder.rewindDirectory();
      break;
    }else{
    nSongs++;
    }   
    entry.close();
  }
     return nSongs;
}
