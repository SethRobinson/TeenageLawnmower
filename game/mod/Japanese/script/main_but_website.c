void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 int id_con;
 int id_dialog;

}

void Main()
{
  //we're a button script, really we don't do much.
}

void OnClick()
{

//launch website in their browser



//SetPauseLevel(100); //freeze lawn mower controls and other dialog boxes, only things created DURING




 id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "ゲームを終了して、RTSOFTのサイト（英語）に行きますか？テクニカルサポート、コミュニティとニュースはこちらでどうぞ。");
 // ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 0, "はい、ウェブサイトに行きます。");
  DialogChoiceAdd(id_choice, 1, "キャンセル");
  
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
 EntKill(id_con); //kill all conversation elements at once


  if (g_i_last_choice == 0)
  {

SetPauseLevel(0); //freeze lawn mower controls and other dialog boxes, only things created DURING
LaunchRTSoftSite();
//EntKillAll();

PostQuitMessage();
 }

	
}
void OnKill()
{

}