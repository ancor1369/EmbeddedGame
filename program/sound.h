extern uLCD_4DGL uLCD;
extern wave_player player;
 
// Given the filename of a .wav file in the SD card, play the file over the speaker.
void playSound(char * wav)
{
    // open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
 
    if(wave_file == NULL){
        uLCD.locate(0,4);
        uLCD.printf("Error in SD");
        return;
    }
    // play wav file
    player.play(wave_file);
 
    // close wav file
    fclose(wave_file);
}
