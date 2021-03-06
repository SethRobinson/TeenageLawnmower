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
  

//   int i_bg = CreateEntPic("background"); //we can find this by name later if we want
//   EntSetSprite(i_bg, "interface\room.jpg");
//   EntSetLayer(i_bg, -10); //show up behind everything for sure
   
//let's show some dialog
 i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "誰かが、ドアをノックする音がする。");
   wait(500);
   SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "お母さんが部屋に入ってきた。足元がふらついている。そしてアルコールのにおいが、漂ってくる。\n\n\"もう起きてる？ところで、昨晩のこと、本当にごめんなさいね…。\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"全然大丈夫、心配しないで。\"");
  DialogChoiceAdd(id_choice, 2, "\"お母さん、今日は、やたらにおいのきつい香水をつけてるね。ジャックダニエルの香水かなんか？\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);

 string st_response;
  if (g_i_last_choice == 1)
  {
    st_response = "うん…。本当に申し訳なかったと思っているのよ、$g_st_name$。";
  }
  
  if (g_i_last_choice == 2)
  {
    st_response = "ちょっと$g_st_name$、そういう説教じみたことは、聞きたくもないわ。しかし長い一日だったわ…。本当に疲れた。";
  } 
 st_response = StringExpand(st_response); //make sure $ vars get converted
  //PART 2

  DialogSet(id_dialog, "\"$st_response$\n\nところで、イブがね、あなたが最近いろんなところで、芝刈りの仕事を頑張ってるってことを教えてくれたわ。えらいわね！\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"なるほど…。イブのストーカー癖についての噂は、本当だったんだ。でもどうして、イブは他人の行動を、やたら干渉するのが好きなんだろう？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "お母さんはにっこりした直後、今度は真剣な顔で、咳払いをした。\n\n");
  DialogAdd(id_dialog, "\"彼女はね、ちょっと、いっちゃってるから…。もしイブの家の芝刈りの仕事を依頼されても、受けないほうがいいと思うわよ。あら、もう寝る時間だわ。明日は早く出ないといけないの。\"");

 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"そうだね、確かに彼女、普通じゃない感じがしたよ。ところで、明日はどこに行くの？仕事は休みじゃなかったっけ？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);


  DialogSet(id_dialog, "\"ちょっと、用があるの…。じゃあおやすみなさい。\"");
 
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
