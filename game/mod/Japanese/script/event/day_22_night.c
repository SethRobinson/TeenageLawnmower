//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 int i_temp;
int i_mood = 0; //jerrys mood
}

void Main()
{
    i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "キッチンから、お母さんの声がする。\n\n\"$g_st_name$、またジェリーが来たわよ。\"");
  //  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);
   //setup background
  int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\kitchen.jpg"); 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  DialogSet(id_dialog, "\"ジェリーは冷蔵庫を閉めた。\"最近どうよ？久しぶりに『パーマンドンジャラ』でもしない？\"");
  ConversationSetPic(id_con, "interface\jerry_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"いいよ、やろうか。\"");
  DialogChoiceAdd(id_choice, 2, "\"疲れているんだ、今日は帰ってくれないかな、ジェリー。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
   if (g_i_last_choice == 2)
     {
         //they don't want to play
  DialogSet(id_dialog, "\"そう…。芝刈りビジネスを始めて、随分と付き合いが悪くなったよね…。君は変わってしまったよ。友達の存在、忘れないでくれよ！\"\n\nジェリーはとぼとぼと帰って行った。");
  ConversationSetPic(id_con, "interface\jerry_normal.dds");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
    goto finish;
   }

   //start playing
    DialogSet(id_dialog, "ジェリーは、持参のダッフルバッグを開けた。\n\n\"覚えてるかな？同じ種類のパイを3枚づつ、計3組揃えれば「あがり」だよ。\"");
    ConversationSetPic(id_con, "interface\jerry_smile.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "コンティニュー");
    //let's wait for input, it will be put in a global var called "g_i_last_choice"
    DialogChoiceWait(id_choice);
    
   again:

//start playing
    DialogSet(id_dialog, "\"オーケー、じゃあ僕が親をやろう。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "パーマン、かば雄、パーやんを集める");
    DialogChoiceAdd(id_choice, 2, "さぶ、さぶ、みち子を集める");
    DialogChoiceAdd(id_choice, 3, "パー子、みつお、バードマンを集める");
    //let's wait for input, it will be put in a global var called "g_i_last_choice"
    DialogChoiceWait(id_choice);

   //if (g_i_last_choice  == 1)
    {
     //they don't want to play
  DialogSet(id_dialog, "\"オーケー。さっそく交換パイのバードマンを使おうかな…ヘへへ…《５分経過》それともとっておこうかな…《１０分経過》ウヒヒ…。…「ロン」上がりの時は、そのパイを捨てた人から計算で出た点数を貰うんだから…うへへ!\"\n\n ため息が出た。優柔不断。これがジェリーの問題なのだ。というか、問題のうちのひとつだ…。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "ジェリーに帰ってもらうように言い、部屋に戻る");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
    goto finish;
    }
   if (g_i_last_choice  == 2)
    {
  
  //they don't want to play
  DialogSet(id_dialog, "\"Ok, you start walking north.\"\n\nJerry pretends to roll a few times.\n\n\"Oh bad luck.  A random bolt of lightning killed you.  You should have a found a cave or something I guess.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Lightning?  But you didn't even tell me there was a storm\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
 

  //they don't want to play
  DialogSet(id_dialog, "Jerry smiles.\n\n\"Ah, but you never asked.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Kick Jerry out and go back to your room");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

    goto finish;
    }


  //if they want to piss jerry off...

 if (i_mood == 0)
   {
    ConversationSetPic(id_con, "interface\jerry_normal.dds");
//they don't want to play
  DialogSet(id_dialog, "Jerry looks agitated.\n\n\"You can't go that way!  You can only go north or south.  Let's try this again.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Ok\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   i_mood = 1;
   goto again;
    }

   //jerry is really pissed now

    ConversationSetPic(id_con, "interface\jerry_upset.dds");
//they don't want to play
  DialogSet(id_dialog, "Jerry face turns bright shade of red.\"You always do this!  I'm too pissed to play now.  BYE.\"\n\nJerry has left.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Go back to your room");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);


  finish:
 
   EntKill(id_con); //kill all conversation elements at once 
   //let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
