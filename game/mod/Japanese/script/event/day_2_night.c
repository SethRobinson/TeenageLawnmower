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
  DialogSet(i_tb, "キッチンから、誰かの叫び声が聞こえた。");
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
  
  DialogSet(id_dialog, "キッチンを覗いてみた。テーブルの側に、恐ろしい顔をしたトッドがいた。\n\n　お母さんが、ちらっとこっちを見た。ついさっきまで、泣いていたように見える…。\n\n\"あら…。どうしたの？\"");
ConversationSetPic(id_con, "interface\mom_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"叫び声が、聞こえてきたんだけど…。何があったの？\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
 ConversationSetPic(id_con, "interface\father2_normal.dds");
 
  DialogSet(id_dialog, "\"やあ、落ちこぼれクン。お勉強の邪魔して、悪かったねぇ！\" \n\n皮肉たっぷりの声で、トッドが言った。\n\n\"お前には全然関係ねーんだよ。とっととうせろ。\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"お母さん？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
 ConversationSetPic(id_con, "interface\mom_normal.dds");
 
 if (g_i_ate_meat == 1)
  {

      DialogSet(id_dialog, "\"トッドの言うことを聞いて頂戴…。私たちのプライベートなことだから、心配しないでね。 $g_st_name$、晩御飯でも食べる？\"\n\n　お母さんは、冷蔵庫を開けた。一生懸命、冷蔵庫の中から何かを探し出そうとしている。\n\nトッドは、イライラした様子でテーブルを叩いている。");
      DialogChoiceClear(id_choice);
      DialogChoiceAdd(id_choice, 1, "\"そこにあったステーキは、僕が食べたよ。\"");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

     ConversationSetPic(id_con, "interface\father2_normal.dds");
     DialogSet(id_dialog, "それを聞いたトッドは激怒し、僕に飛びかかり、体をオーブンに押し付けた。一生懸命もがいたが、トッドの力にかなうわけもなく、押さえつけられたままであった。\n\n\"にくたらしい小僧め。１７歳の泥棒か…。やっかいな野郎だな。\"");
     DialogChoiceClear(id_choice);
     DialogChoiceAdd(id_choice, 1, "\"あのステーキは、僕が働いた金で、買ったんだ！\"");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

    ConversationSetPic(id_con, "interface\mom_normal.dds");
    DialogSet(id_dialog, "\"離してあげて！\" お母さんは叫んだ。\n\nトッドはお母さんを、にらみつけた。その間に、どうにか逃れることができた。悔しくて、思わず涙が込み上げる…。\n\nどうすることも出来ず、結局部屋に戻り、ドアを閉めた。外から何も聞こえてこないように。");
     DialogChoiceClear(id_choice);
     DialogChoiceAdd(id_choice, 1, "コンティニュー");
  }

 if (g_i_ate_meat == 0)
  {

      DialogSet(id_dialog, "\"トッドは正しいわ。私たちのプライベートなことだから、心配しないでね。 $g_st_name$、晩御飯でも食べる？\"\n\nお母さんは冷蔵庫を開けた。ステーキを取り出し、オーブンに入れた。");
      DialogChoiceClear(id_choice);
      DialogChoiceAdd(id_choice, 1, "部屋に戻る。");
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
