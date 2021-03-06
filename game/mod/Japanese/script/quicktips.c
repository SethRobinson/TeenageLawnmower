void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void Main()
{


if (g_i_difficulty > 0)
  {
    //they are doing a hard mode, so they won't need tips.
// EntKill(i_my_id);  // return;
  //but what if they are just cocky and still need the tips?
  }

if (g_i_quick_tip > 2)
  {
     //already showed all quick tips, don't show anymore
    EntKill(i_my_id); 
     return;
 }


wait(1000);
  //should we show a quick tip?
logmsg("Showing quick tip..");



//pause the whole game
EntSetPauseLevel(i_my_id, 200); //give this script mega access
SetPauseLevel(200);
 CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
//ResetFrameTimer();
int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
EntSetFadeUp(i_tb, 2.0); //fade speed.  

if (g_i_quick_tip == 0)
  {
 string st_temp = VKeyToString("ACTION2");
   DialogSet(i_tb, "攻略テクニック: 進行方向を変えるには《矢印キー》を、ジャンプするには《スペースキー》を用いる。後ろ向きに進むことも出きる。\n\nターンをしながら《$st_temp$》を同時に押すと、「パワーターン」が出来る。パワーターン無しでは、ゲーム攻略は不可能だろう。");
  }


if (g_i_quick_tip == 1)
  {
  DialogSet(i_tb, "攻略テクニック: 《TABキー》で、カメラアングルを変えることが出来る。\n\nまた、プレイ中に《ESCAPEキー》を押すとメニューが現れ、ゲームをキャンセルしたりできる。");
 }
if (g_i_quick_tip == 2)
  {
  DialogSet(i_tb, "攻略テクニック: 《L》で、カメラのアングルをロックすることができる。もう一度《L》で、ロックを解除できる。\n\nまた、《Alt》と《Enter》を同時に押すことによって、ウィンドウモードとフルスクリーンモードの切り替えができる。");
 }

   SoundPlay("sound\quicktip.wav");

SetPauseLevel(200);

   DialogWaitForOk(i_tb);
   EntKill(i_tb);
EntSetPauseLevel(i_my_id, 0); //remove previously mentioned mega access
 CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

SetPauseLevel(0);

//increment quick tip
g_i_quick_tip += 1;

   EntKill(i_my_id);

}

void OnKill()
{

}
