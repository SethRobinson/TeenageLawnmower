//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 int i_temp;
}



void Main()
{
   
//let's show some dialog
 i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "誰かが、ドアを強く叩いている。");
  wait(300);
  SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  
  //int i_bg = CreateEntPic("background"); //we can find this by name later if we want



   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "\"くそババァがよぉ、俺に向かって、少しはお前と一緒に時間を過ごす努力をするように、なんて、命令してきやがった。\"\n\n言われるがまま、彼の後を追い、キッチンへと向かった。これから一体、何が起こるのだろう…。全く予想ができない。");
 ConversationSetPic(id_con, "interface\father2_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  
 int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\kitchen.jpg");
 EntSetLayer(i_bg, -10); //show up behind everything for sure

    DialogSet(id_dialog, "トッドは、冷蔵庫からビールを取り出した。\n\n\"小僧、お前も飲むか？\"");
    ConversationSetPic(id_con, "interface\father2_smile.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "もちろん。");
    DialogChoiceAdd(id_choice, 2, "いらないよ。");
    DialogChoiceWait(id_choice);
 

if (g_i_last_choice == 1)
   {
    DialogSet(id_dialog, "トッドは、もう一本ビールを取り出し、こちらに投げた。\n\n\"ほらよ。オラ、小僧、俺のこのビールの隠し場所は、絶対誰にもいうな。分かったか？\"\n\n僕はうなずいた。");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
    DialogChoiceWait(id_choice);
    }

if (g_i_last_choice == 2)
   {
    DialogSet(id_dialog, "トッドは、にやっとした。\n\n\"本当に、馬鹿な小僧だな。医学的には、アルコールは、なんかのリスクを下げるって証明されてんだ。飲まねえと損ってやつよ。\"");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
    DialogChoiceWait(id_choice);
    }


   DialogSet(id_dialog, "トッドは煙草に火をつけ、いすにもたれかかった。\n\n数分間の嫌な沈黙が続いた。");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

   DialogSet(id_dialog, "\"おっ、フットボールの試合が始まるな。おい、くそババアに、トッドと仲良くなれてうれしかったって、伝えるんだぞ、分かったか。\"\n\nトッドはテレビを見るために、リビングルームへ向かった。");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "部屋に戻る");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);


   EntKill(id_con); //kill all conversation elements at once 
  
//let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
