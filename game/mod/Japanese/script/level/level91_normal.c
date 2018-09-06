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
 
  //create it!
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\house.jpg");
   EntSetLayer(i_bg, -10); //show up behind everything for sure

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "エクスカリバーを押しながら、家に帰った。");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
  
  SoundMusicPlay("sound\room_music.wav", 1000);

  EntSetSprite(i_bg, "interface\kitchen.jpg");
           DialogSet(i_tb, "シーンとした家の中に、自分の声が響く…\n\nお母さんの姿がどこにも見当たらない…。\n\nとりあえず、自分の部屋に戻ろう…。");
           DialogWaitForOk(i_tb);
  EntSetSprite(i_bg, "interface\room.jpg");
        
    EntKill(i_tb);


 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "中年の女性がベッドに横たわっているではないか！\n\n\"お久しぶりネ、ハンサム君！勝手にお邪魔してごめんなさい、でもアナタなら私を歓迎してくれると思って…ウフフ\"");
  ConversationSetPic(id_con, "interface\eve_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"イブ！ 一体何で！？ ちゃんと戸締りして出たと思っていたのに…！！！\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "\"どの窓もロックしてあったわ、でも私、実は鍵師なのよ、フフ。私たちって、やっぱり一緒になる運命なのよ、ダーリン、そう思わない？\"");
  //ConversationSetPic(id_con, "interface\mom_smile.dds");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "イエス、そうだね！一緒になろう！！");
  DialogChoiceAdd(id_choice, 2, "何ですって！？冗談じゃない！！！！！");
 
  DialogChoiceWait(id_choice);
  
if (g_i_last_choice == 1)
   {

  DialogSet(id_dialog, "\"それなら今直ぐこっちに来て、可愛いダーリン！！\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
 
  DialogChoiceWait(id_choice);
 
  DialogSet(id_dialog, "…僕の服も心も、もうここには存在しない…。\n\n 少したって、イブは去っていった。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);
 

   }


if (g_i_last_choice == 2)
   {

  DialogSet(id_dialog, "イブが服を脱ぎだした…とてつもない程、気持ちの悪い物体だ。思わず嘔吐してしまいそうだ…。急いで彼女を部屋から追い出し、隠者デールの住所を書いた紙を渡した。イブは笑顔で去って行った…。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);
 
   }



  EntKill(id_con); //kill all conversation elements at once 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
