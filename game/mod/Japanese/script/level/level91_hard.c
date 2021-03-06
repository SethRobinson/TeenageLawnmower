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
           DialogSet(i_tb, "エクスカリバーを持って、家に帰った。");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
 
    EntKill(i_tb);
SoundMusicPlay("sound\room_music.wav", 1000);

  EntSetSprite(i_bg, "interface\kitchen.jpg");

 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "エクスカリバーと共にキッチンに現れた自分を見て、お母さんは驚いた。\n\n\"なんて美しい芝刈り機なんでしょう！！どうやら芝刈りビジネスで成功しているみたいね！あなたをとても誇りに思うわ！\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"そうだね。お陰様で順調だよ。\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "\"一つ提案があるんだけど…。ねぇ、私たち、これからチームとしてやっていかない？というのも、お母さんね、あなたの知っている通り、失業中で早く仕事を見つけないといけないの。もしアナタが、お母さんにも仕事をくれたら、それって素晴らしいことだわ！私たち絶対成功するわよ！\"");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"いやいや、アル中母さんには無理だよ。\"");
  DialogChoiceAdd(id_choice, 2, "\"そうだね、パートナー決定！\"");
 
  DialogChoiceWait(id_choice);
  
if (g_i_last_choice == 1)
   {
 ConversationSetPic(id_con, "interface\mom_normal.dds");

  DialogSet(id_dialog, "怒ったお母さんは、じたんだを踏んだ。\n\n\"何ですって？ずいぶん前に、酒は一切止めたわよっ！！\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"それじゃあドラッグは？\"");
 
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogSet(id_dialog, "\"あら、ばれちゃってるのね。そうか、私の部屋で注射器見つけて盗んだの、アナタね！でもね、ドラッグぐらいじゃなんともないの、お母さんは。芝刈りには影響しないから大丈夫よ！\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"それならいいよ！パートナー決定！\"");
  DialogChoiceAdd(id_choice, 2, "\"いや、無理だよお母さん。\"");
  DialogChoiceWait(id_choice);
 

  if (g_i_last_choice == 2)
    {
       //said no
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 DialogSet(id_dialog, "お母さんはうなだれてしまった…。けれども彼女なりに、これからはドラッグやアルコール依存症から抜け出し、自分を更生していく努力をするらしい。芝刈りビジネスが彼女の人生を変えたといっても過言ではないだろう…！！");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);
  goto done;
    }

   }

//yes, work with mom

  DialogSet(id_dialog, "お母さんと握手をした。\n\n\"$g_st_name$、本当にあなたのことを誇りに思うわ。死んだお父さんの、古い芝刈り機を持ち出して初めた芝刈りビジネスを、ここまで大きくできたなんて…。（涙を流す）\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  DialogChoiceWait(id_choice);
 
done:


  EntKill(id_con); //kill all conversation elements at once 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
