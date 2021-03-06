void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_dialog;
 int i_tb;
int i_temp;

 int id_con;
 int id_choice;
}


void Main()
{
 
 //

 //play music
  SoundMusicStop();
   SoundMusicPlay("sound\graveyard_music.wav", 1000);
 
  //create it!
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\house.jpg");
   EntSetLayer(i_bg, -10); //show up behind everything for sure

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "エクスカリバーを押しながら、家に帰った。\n\n…余りにも興奮していたため、トッドの車が家の前に止まっていることなど、全く気が付かなかった…。");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
  

  EntSetSprite(i_bg, "interface\kitchen.jpg");
           DialogSet(i_tb, "家の中に自分の声が響く…\n\nお母さんはどこにもいない。\n\nバスルームの電気がついていることに気が付いた。");
           DialogWaitForOk(i_tb);
  EntSetSprite(i_bg, "interface\bathroom.jpg");
           DialogSet(i_tb, "トッドが、床にうつぶせになって倒れていた。ショットガンが側に落ちている…。\n\n血のちおいがこもっている…。シャワーカーテンを開ける勇気がなかった…。お母さんの死体はそこにあるに違いないのだから…。");
           DialogWaitForOk(i_tb);

  SoundMusicPlay("sound\room_music.wav", 1000);
  EntSetSprite(i_bg, "interface\house.jpg");
        

      DialogSet(i_tb, "…目が覚めた。…頭を振った。あぁ、夢だったのか…。\n\n気を取り直し、お母さんを探した。");
        DialogWaitForOk(i_tb);

  EntSetSprite(i_bg, "interface\kitchen.jpg");
    EntKill(i_tb);


 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "\"おはよう！あら、新しい芝刈り機を貰ったの？！とても素敵じゃない！トーマスは悪い人じゃなかったのね！\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"そうだね、ちょっと変わった人だけど…。とにかく、これは僕のお気に入りの芝刈り機だよ！\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "お母さんは笑った。昔のように、すがすがしい笑顔だ。\n\n\"冷蔵庫にあるもの、何でも好きなもの食べてね！！\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
 
  DialogChoiceWait(id_choice);
   EntKill(id_con); //kill all conversation elements at once 
 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
