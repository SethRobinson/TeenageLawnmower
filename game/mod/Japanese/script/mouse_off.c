//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate()
{
}

void Main()
{
 
   int id_mouse = EntGetByName("MOUSE");

   if (id_mouse != -1)
   {
	//mouse exists, kill the sucker.. hmm, maybe we should just hide it later?
	EntKill(id_mouse);        
        EntKill(i_my_id);
	return;
   }

 EntKill(i_my_id);
}

void OnKill()
{
}
