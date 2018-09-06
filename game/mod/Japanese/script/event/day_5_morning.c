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
  
   //setup background

 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "朝食を食べ、おなかもいっぱいだ。すがすがしい気分のいい朝。とてもいい睡眠が取れるのは、連日肉体労働をしているからだろう。\n\nそれで、今から何をしようか？");
  //ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "お気に入りのサイトをチェックする");
  DialogChoiceAdd(id_choice, 2, "シャワーを浴びる");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);

 string st_response;
  if (g_i_last_choice == 1)
  {
    st_response = "いくつかのサイトをチェックした。毎週読んでいるウェブ漫画、今週はまだ更新されていない…。残念。 あ、面白ろそうなリンクを見つけた…。\n\n2時間後、やっぱりシャワーを浴びたくなった。";
   g_i_time += 2; //two hours lost, haha. 
DialogSet(id_dialog, st_response);
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);


 }
  
    st_response = "シャワーを浴びているとき、ゴミ箱に、使用済みの妊娠検査薬が捨ててあるのを、見つけてしまった…。";
 int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\bathroom.jpg");

  //PART 2

  DialogSet(id_dialog, st_response);
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
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
