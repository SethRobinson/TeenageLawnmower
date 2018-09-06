void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_dialog;
 int i_tb;
int i_temp;
}


void Main()
{

}

void OnKill()
{
	
}

// st_temp = "Earn $$$g_i_goal_money$ by the morning of day $g_i_goal_day$ for $g_st_goal$";

void Setup()
{
logmsg("Setting up for goal $g_i_goal$.");
    g_i_goal_day = g_i_day;  
   g_i_goal_money = 0;
  
   //uncomment this and basically have extra money doesn't help
  //g_i_goal_money = g_i_money;
  
if (g_i_last_goal == 0)
  {
     //set the very first goal
    g_st_goal = "食費";
    g_i_goal_money += 77; //how much money is needed to make  
    g_st_goal_win_string = "食費に、$$$g_i_goal_money$使った。";
    g_st_goal_lose_string = "君とお母さんは、飢え死にした。ジ・エンド";
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 1)
  {
     //given on day 2
    g_st_goal = "電気料金";
    g_i_goal_money += 180; //how much money is needed to make  
    g_st_goal_win_string = "電気料金 $$$g_i_goal_money$を支払った。";
    g_st_goal_lose_string = "電気料金の支払いが滞ってしまった為、電気をとめられ、インターネットも使えなくなってしまった。よって、芝刈りビジネスのプロモーションも不可能となり、結局お母さんと一緒にトッドの家に引っ越すこととなった…。無念である。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"電気料金の支払いのため、２、３日内に$g_i_goal_money$ ドル必要なの…。今月全く余裕がなくて…。ごめんなさいね。－母より\"";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 2)
  {
     
   i_temp = randomrange(1, 3);

  //let's mix it up a bit

  if (i_temp == 1)
   {

     //goal three, given on day 4 
    g_st_goal = "エアコン修理代金";
    g_i_goal_money += 325; //how much money is needed to make  
    g_st_goal_win_string = "エアコンの修理代金として、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "新しいエアコンを買うことが出来ず、熱射病になってしまった。耐え切れず、死亡。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"昨晩、突然エアコンが壊れてしまったの…。新しいエアコンを購入するのに、どうしても$g_i_goal_money$ドル必要だわ…。－母より\"";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
  if (i_temp == 2)
   {

     //goal three, given on day 4 
    g_st_goal = "ガスコンロ購入代金";
    g_i_goal_money += 315; //how much money is needed to make  
    g_st_goal_win_string = "新しいガスコンロを購入し、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "新しいものを購入できずに、そのまま壊れたものを使用したため、夜中に、突然ガスコンロから火の手があがり、無念にも母と共に焼死。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"ガスコンロの調子がとても悪いのよ…。新しいものを購入するのに、どうしても$g_i_goal_money$ドル必要なので、よろしくね。－母より\"";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
  if (i_temp == 3)
   {

     //goal three, given on day 4 
    g_st_goal = "温水器購入代金";
    g_i_goal_money += 325; //how much money is needed to make  
    g_st_goal_win_string = "新しい温水器を購入。$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "熱いシャワーを浴びようとした。しかし、氷のような冷たい水しか出ず、結局凍死。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"どうやら温水器が壊れてしまったようなの。新しいのを買うのに、どうしても $g_i_goal_money$ドルが必要だわ…。よろしくね。－母より\"";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
 goto fin;

  }
 
if (g_i_last_goal == 3)
  {
     //goal three, given on day 7 
    g_st_goal = "超音波検査代金";
    g_i_goal_money += 105; //how much money is needed to make  
    g_st_goal_win_string = "病院で超音波検査を受け、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "お母さんが、超音波の検査を受けるお金がないためにヒステリックを起こし、暴騰。そして事故死。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"明日、超音波の検査を受けなければいけないの…。だから、そのためのお金を工面してもらいたいのよ。とりあえず今日中に$g_i_goal_money$ドル必要なの…。－母より\"";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }


if (g_i_last_goal == 4)
  {
     //given on day 8 
    g_st_goal = "車の修理代";
    g_i_goal_money += 250; //how much money is needed to make  
    g_st_goal_win_string = "車を修理に出し、修理代金として$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "お母さんが、交通事故で死んだ。恐らく、ブレーキかなにかが壊れていたのだろう…。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"私の車の調子がちょっと悪いみたいなの…。修理に出したいので、申し訳ないんだけど、明日中に$g_i_goal_money$ドル用意してもらえないかしら？　－母より\"";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 5)
  {
     //given on day 10 
    g_st_goal = "新しい洋服代";
    g_i_goal_money += 105; //how much money is needed to make  
    g_st_goal_win_string = "新しい洋服を購入し、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "洋服を買うお金もなく、裸でビジネスをしようとしたが、変質者に間違われて見事に失敗した…。イブを除いては…。\n\nジ・エンド";
    g_st_goal_start = "最近、どうも自分の服のスタイルがダサいことに気が付いた…。新しい洋服を買わないと。今日中に$g_i_goal_money$ドル、用意したい。";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 6)
  {
     //given on day 11 
    g_st_goal = "スパークプラグ代金";
    g_i_goal_money += 2; //how much money is needed to make  
    g_st_goal_win_string = "スパークプラグを購入。$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "2ドルすらも用意できなかったなんて！！！ゲームオーバーになって当たり前かもよ！\n\nジ・エンド";
    g_st_goal_start = "芝刈り機のスパークプラグを新しいもの（$$g_i_goal_money$ドル）と取り替えないといけないな…。";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 7)
  {
     //given on day 12 
    g_st_goal = "家のローン";
    g_i_goal_money += 425; //how much money is needed to make  
    g_st_goal_win_string = "家のローン、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "何ヶ月もローンを滞納したため、持ち家を取り押さえられてしまった！そしてホームレスになった。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"家のローンを支払うのに、お金が足りないの…。どうか3日後までに$g_i_goal_money$ ドル用意してもらえないかしら…？－母より\"";  
    g_i_goal_day += 3; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 8)
  {
     //given on day 15 
    g_st_goal = "クレジットカードの支払い";
    g_i_goal_money += 280; //how much money is needed to make  
    g_st_goal_win_string = "クレジットカードの返済として$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "クレジットカードの負債が大きくなりすぎて、破産宣告をするしか方法がなくなった。\n\nジ・エンド";
    g_st_goal_start = "机の上に、ノートの切れ端がおいてあった。\n\n\"クレジットカードの返済に困っているの…。あさってまでに、どうしても$g_i_goal_money$ドル必要だわ。なんとか助けてもらえないかしら？－母より\"";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 9)
  {
     //given on day 17 
    g_st_goal = "ハードドライブ購入代金";
    g_i_goal_money += 150; //how much money is needed to make  
    g_st_goal_win_string = "120GBのハードドライブを手に入れた！$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "ハードドライブは壊れてしまった。悲しいことに、バックアップを取っていなかった…。たくさんのデータを失い、もうだめだ。\n\nジ・エンド";
    g_st_goal_start = "ハードドライブが今にも壊れそうな音をたてている。急いで新しいのを買わないと…。今日中に$g_i_goal_money$必要だ。";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 10)
  {
     //given on day 18 
    g_st_goal = "弁護士費用";
    g_i_goal_money += 570; //how much money is needed to make  
    g_st_goal_win_string = "いい弁護士を雇い、費用として$$$g_i_goal_money$支払った。お陰で裁判に勝つことが出来た！";
    g_st_goal_lose_string = "弁護士のヘルプなしで裁判で戦うことは時間の無駄であった…。賠償金の支払いを命じられ、どうにも芝刈りビジネスを続けられる状態ではなくなった。\n\nジ・エンド";
    g_st_goal_start = "お客のうちの一人が、仕事が気にいらないので、裁判にもっていくという…。弁護士を雇うために、$g_i_goal_money$ ドル必要だ。今すぐ！！";  
    g_i_goal_day += 4; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 11)
  {
     //given on day 22 
    g_st_goal = "ウイニングビッド代金";
    g_i_goal_money += 375; //how much money is needed to make  
    g_st_goal_win_string = "出品者に$$$g_i_goal_money$支払い、めでたくプレミア付きのゲームを入手できた。";
    g_st_goal_lose_string = "支払い期限が守れず、ブラックリストにのってしまった。もうオンラインオークションには参加させてもらえないだろう…。ショックから鬱がはいり、重症。\n\nジ・エンド";
    g_st_goal_start = "オンラインオークションで、ずっとほしかったプレミア付きのゲームが売り出されている。入札をし、なんとか無事落札できた！が、至急375ドル支払わなくてはならない！！！!";  
    g_i_goal_day += 3; //how many days they have from now
    goto fin;
  }

 //if we got here let's just make a random thingie for them

//first pick the name of the thing

 int i_per_day_price;

 if (g_i_difficulty == 0)
  {
    i_per_day_price = RandomRange(80, 110);
  } 

 if (g_i_difficulty == 1)
  {
    i_per_day_price = RandomRange(80, 130);
  } 

 if (g_i_difficulty == 2)
  {
    i_per_day_price = RandomRange(80, 150);
  } 

  
  int i_days = RandomRange(1, 3);
  g_i_goal_money = i_per_day_price;
  g_i_goal_money *= i_days; 
  g_i_goal_day += i_days; //how many days they have from now


 if (i_days == 1)
  {
   //******** 1 DAY EVENTS ********
  i_temp = RandomRange(1, 3);
 
  if (i_temp == 1)
  {
    g_st_goal = "RTSOFTのゲーム購入代金";
    g_st_goal_start = "RTSOFTから最高に面白いゲームが発売されたようだ…。絶対買わないと！…あら、高い！$g_i_goal_money$ドルするようだ。";  
    g_st_goal_win_string = "RTSOFTの新しいゲームを購入し、$g_i_goal_money$支払った。かなり面白いゲームで、満足だ！";
    g_st_goal_lose_string = "RTSOFTのゲームがほしくてたまらないのに、買うお金もなく、ストレスがたまりにたまり、とうとう気がくるってしまった。芝刈りビジネス続行は不可能だろう。\n\nジ・エンド";
      }

  if (i_temp == 2)
    {
    g_st_goal = "芝刈り専門書購入代金"
    g_st_goal_start = "芝刈りに関する専門書を購入したい。$g_i_goal_money$ドル用意しなければ…。";  
    g_st_goal_win_string = "芝刈りに関する専門書を購入。 $$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "新しい技術を取り入れるための専門書を買うお金もなく、自分のマンネリ化した芝刈りにどのクライアントも飽きがきたようだ。いつしか、クライアント全員離れていき、ビジネス続行は不可能となった。\n\nジ・エンド";
    }
  
  if (i_temp == 3)
    {
    g_st_goal = "ビジネス専門書購入代金";
    g_st_goal_start = "どうもビジネスをするにはコツがいるようだ。自営業の人たちのための専門書を購入したい。明日までに$g_i_goal_money$ドル用意して本屋に行こう。";  
     g_st_goal_win_string = "自営業の人のための専門書を購入。$$$g_i_goal_money$支払った。";
     g_st_goal_lose_string = "毎日忙しいばかりで、いっこうにお金がたまる気配もない。どうもビジネスのやり方を間違えているらしい…。これ以上芝刈りビジネスを続けてもいいことはないだろう。\n\nジ・エンド";
    }
  }


if (i_days == 2)
  {
   //******** 2 DAY EVENTS ********
  i_temp = RandomRange(1, 2);
 
  if (i_temp == 1)
  {
    g_st_goal = "飲み物代";
    g_st_goal_start = "コーヒー豆のストックががもう無い。カフェイン中毒の自分には致命的だ。どうにか$g_i_goal_money$ドル用意して、コーヒー豆を買いにいかないと…。";  
    g_st_goal_win_string = "コーヒー豆を購入し、$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "カフェインが足りず、仕事中にも居眠りをしてしまう毎日。クライアント全ての信用を失い、芝刈りビジネスは終わった。\n\nジ・エンド";
      }

  if (i_temp == 2)
    {
    g_st_goal = "ゲームコンソール購入代金";
    g_st_goal_start = "わぁ、マイクロソニーが今週、新しいゲーム・コンソールを発売したらしい。二日間でなんとかそれを手に入れれるだけのお金$g_i_goal_money$ドルを用意したい。";  
    g_st_goal_win_string = "マイクロソニーのゲーム・コンソールを購入。$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "お金はないが、どうしてもほしかったので万引きを決行。結果、やはり見つかってしまい、警察へ送りになった。\n\nジ・エンド";
    }
  
  }

if (i_days == 3)
  {
   //******** 3 DAY EVENTS ********
  i_temp = RandomRange(1, 2);
 
  if (i_temp == 1)
  {
    g_st_goal = "芝刈り機のオイルチェンジ代金";
    g_st_goal_start = "そろそろ芝刈り機のオイルチェンジが必要だ。点検日は三日後。 $g_i_goal_money$ドル用意しておかなくては。";  
    g_st_goal_win_string = "オイルチェンジと点検に$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "巨大モグラを退治している最中、突然芝刈り機が爆発し、周りにいた全員が即死した。\n\nジ・エンド";
      }

  if (i_temp == 2)
    {
    g_st_goal = "ヘリコプターのラジコン購入代金";
    g_st_goal_start = "最近働いてばかりの人生。なにか趣味がほしくなった。中古のヘリコプターのラジコンがたった $g_i_goal_money$ドルで売り出されているのを発見。絶対ほしい。";  
    g_st_goal_win_string = "ヘリコプターのラジコンを購入。$$$g_i_goal_money$支払った。";
    g_st_goal_lose_string = "ほしかったラジコンを買うお金が無いので、仕方なく、芝刈り機を持ち出してカスタムのヘリコプターを手作りした。それはちゃんと飛んでいたのだが、ある日頭上に落下、首と腕を切断され、死亡。\n\nジ・エンド";
    }
  
  }
fin:
if (g_i_goal_money != 0)
 {
g_i_last_goal += 1;
g_i_goal = 1; //goal is active
logmsg("Set goal to $g_i_goal_day$, last goal is now $g_i_last_goal$..");
  }
EntKill(i_my_id); //kill this script
}
