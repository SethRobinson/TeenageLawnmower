void OnCreate()
{

  int h_sound = 0;
  
}


void Main()
{
   vector3 vec_pos = EntGetPosition(i_my_id);
   h_sound = Sound3DPlay(vec_pos, "sound\beagle_bark.wav", 9999); //play 1 time   
//SoundEntPlay(i_my_id, "sound\rain_loop.wav");

//SoundEntPlay(i_my_id, "sound\beagle_bark.wav");


//  h_sound = SoundPlayInstance("sound\rain_loop.wav");  
//    SoundPlayH(h_sound, 9999);   //loop it
//    SoundStayAlive(h_sound, 1); //don't die after finish playing.  Just stop and wait.
  
}

void OnKill()
{
SoundStayAlive(h_sound, 0);
SoundKill(h_sound);
}