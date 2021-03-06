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
  DialogSet(i_tb, "いい朝だ。\n\n歯を磨いた。そして急にホットケーキが食べたくなったので、キッチンへ行く。");
//  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  
  //int i_bg = CreateEntPic("background"); //we can find this by name later if we want

int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\kitchen.jpg");
 EntSetLayer(i_bg, -10); //show up behind everything for sure


   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "バスローブを羽織ったお母さんが、テーブルについていた。\n\n\"おはよう。今日は、あなたに紹介したい人がいるの。\"\n\n瞬きをし、彼女が誰かと一緒なのに気がついた。");
  ConversationSetPic(id_con, "interface\mom_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
 DialogChoiceWait(id_choice);
 ConversationSetPic(id_con, "interface\cult_upset.dds");

  DialogSet(id_dialog, "\"ア、どうもどうも、こんにちは。君のお母さん、とても魅力的だナァ。私たち、会ってすぐに、意気投合したんだ…。\" 彼は、目を合わせることなく、ぶつぶつ言った。");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"お母さん！コイツ世捨て人だよ！！この間、彼の庭の芝刈りをしたばっかりだよ！！最悪だ！！\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogSet(id_dialog, "\"$g_st_name$、年上の人にはもっと丁寧にね。デールは、とても優しい、紳士よ。\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"コイツは、4人もの奥さんを次々と殺していった、殺人者なんだぞ！\"");
  DialogChoiceAdd(id_choice, 2, "\"ごめんよ、デール。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  if (g_i_last_choice == 1)
    {

  DialogSet(id_dialog, "お母さんに、強く頬をぶたれた。\n\n\"二度とそんなこと言うんじゃないよ！絶対ゆるさないからね！部屋に戻りなさい！！\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

    }

  if (g_i_last_choice == 2)
{
  DialogSet(id_dialog, "\"気にしてないよ。君ね、私の悪口はいくら言ってもいいが、決して私の可愛い犬たちの悪口は、いうんじゃないぞ。分かったかね？\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"分かったよ、デール。\"");
  DialogChoiceAdd(id_choice, 2, "\"犬？ああ、あんたのビーグル犬たちね。たくさんいすぎて、本当仕事にならなかったよ！！\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

   if (g_i_last_choice == 1)
   {
  DialogSet(id_dialog, "お母さんはにっこりした。\"あなたたち、どうやらすっかり、仲良くなったみたいね！一緒に朝ごはんをいただきましょうよ。\"\n\n一時間後、デールは帰途へ着いた。僕は、部屋に戻った。");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
  g_i_time += 1; //one hour passes
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);

   }
if (g_i_last_choice == 2)
   {
  DialogSet(id_dialog, "デールは急に立ち上がった。\"私の犬は、人間よりも、素晴らしい！！！分かったような口をきくな！！アァ、ところで昨晩、君の歯ブラシを、勝手に拝借させていただいたよ。すまないと思ったが、今日こんな仕打ちを受けたからな、これでイーブンだ。\"");
  DialogAdd (id_dialog, "\n\nお母さんは途方にくれていた。\"今すぐ部屋に戻りなさい、分かったわね？\"");
   
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);

   }
}

   EntKill(id_con); //kill all conversation elements at once 
  
//let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
