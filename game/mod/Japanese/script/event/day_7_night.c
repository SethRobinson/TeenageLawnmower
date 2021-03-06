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

//int i_bg = EntGetByName("background");
// EntSetSprite(i_bg, "interface\kitchen.jpg");
// EntSetLayer(i_bg, -10); //show up behind everything for sure


   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "トッドが、バン！と音をたてて、無理やりドアを開けた。\n\n\"おい、小僧。１０ドル貸してくれないか。\"");
 ConversationSetPic(id_con, "interface\father2_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"お母さんから、もらいなよ。\"");
  DialogChoiceAdd(id_choice, 2, "\"いいよ。じゃ、これ。\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  
 if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "\"お前のかあさんは、一銭も持ってねぇよ。役にたたねぇ。\n\nでもよぉ、おめえが最近芝刈り業で、結構いい金稼いでるって、どこかで聞いたんだよ。儲かってるんだろう？もちろんちょっとはこっちにも、回してくれるよなぁ。\"");
  ConversationSetPic(id_con, "interface\father2_smile.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "トッドにお金を渡す");
  DialogChoiceAdd(id_choice, 2, "トッドに向かって、中指を立てる");
  DialogChoiceWait(id_choice);
 
   if (g_i_last_choice == 1)
    {
     //do nothing
    }

   if (g_i_last_choice == 2)
    {
    DialogSet(id_dialog, "トッドに向かって、中指を立てた。\n\n\"この野郎！！！\"\n\nトッドは僕の腕を鷲づかみし、壁に押し付け、ジーンズのポケットから、あっという間に、財布を抜き出した。");
    ConversationSetPic(id_con, "interface\father2_mad.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
    DialogChoiceWait(id_choice);
    }

  } 
 

 //this happens no matter what

    DialogSet(id_dialog, "トッドは、にやっと笑った。\n\n\"おめぇみたいな、オタク野郎と知り合えてうれしいよ。\n\n母ちゃんは、子育て失敗したなぁ。フットボールでもさせとけばよかったのによぉ。こんなコンピューターオタクになっちまってなぁ！\"");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
 
//actually take the money

 g_i_money -= 10;
  if (g_i_money < 0)
   {
     g_i_money = 0;
   }

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
