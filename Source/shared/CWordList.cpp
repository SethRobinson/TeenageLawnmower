#include "CWordList.h"
#include <cassert>

const int C_DEBUG_OUTPUT_SKIP = 100; //if we printed every word we'd be here forever

bool operator < (const CWord &a, const CWord &b) 
{
  if (a.s_sorted_word==b.s_sorted_word) return a.s_sorted_word<b.s_sorted_word;
  else return a.s_sorted_word<b.s_sorted_word;
}


//help us replace all instances in a piece of text easier
void STLReplace( string& Source, const string& Find, const string& Replacement)
{
    int Length = Find.length();
    string::size_type Pos = 0;
    while( (Pos = Source.find( Find, Pos )) != string::npos ) 
    {
        Source.replace( Pos, Length, Replacement );
    }
}


CWordList::CWordList()
{
    m_p_words_to_ignore_list = NULL;
    SetMaxWordLength(255); //default limit
    SetMinWordLength(1); //default limit
    SetStripMiscOnLoad(false);
    SetIgnoreMiscOnLoad(false);
}

void CWordList::GetRandomWord(char *p_word_out)
{
 
    int i_num = random(a_words.size());
    strcpy(p_word_out, this->a_words[i_num].s_word.c_str());
}

string CWordList::GetWord(const unsigned int ui_index)
{
    assert(ui_index >= 0 && ui_index < a_words.size() && "Invalid access, no word exists at this index.");
    return a_words[ui_index].s_word;
}
bool CWordList::ReadInOptimizedWordlist(const char *fname)
{
 
    FILE *fp = fopen( fname, "r" );


    if (!fp) return false;

    char st_buffer[4096];
    
     char *p_word = NULL; //pointer to regular word
     char *p_word_optimized = NULL; //pointer to optimized word
      int i_space_position;  //where the space separating the two words per line is


        while (get_line_from_file(fp, (char*)&st_buffer))
    {
     
        st_buffer[strlen(st_buffer)-1] = 0; //remove the cr

        //ok, we just loaded two words, let's separate them
         p_word_optimized = &st_buffer[0];

         //find the space
         for(i_space_position=0; st_buffer[i_space_position] != 32; i_space_position++);
         p_word = &st_buffer[i_space_position+1]; //start of normal word
         //go ahead and replace the space with a null so p_word_optimized will have a terminator
         st_buffer[i_space_position] = 0;
         AddWordPair(p_word, p_word_optimized);
       
    }  
 
    //last one
    fclose(fp);
   return true; //success
}




bool CWordList::ReadInWordList(const char *fname)
{
  
    int i_word_length;

    FILE *fp = fopen( fname, "r" );

    if (!fp) return false;

    char st_buffer[4096];
    
    int i_count = 0;
    
    while (get_line_from_file(fp, (char*)&st_buffer))
    {
     
        st_buffer[strlen(st_buffer)-1] = 0; //remove the cr
    
        if (i_count < 1)
        {
         i_count = C_DEBUG_OUTPUT_SKIP;
       // LogMsg("Read %s", st_buffer);
        
        } else
        {
            i_count--;
        }
       
        if (m_b_ignore_misc_on_load)
        {
            //if this word contains a ' or - or space let's skip it
            if (strpbrk(st_buffer, "'- "))
                continue; //skip this word

        }

        if (m_b_strip_misc_on_load)
        {
            StripMiscWord(st_buffer);
        }

        if (m_b_remove_accents)
        {
            RemoveAccentsWord(st_buffer);
        }
        if (m_b_convert_to_lower_case)
        {
            ConvertToLowerCaseWord(st_buffer);
        }
        
        
        
        i_word_length = strlen(st_buffer);

        if (
            !(GetMaxWordLength() < i_word_length)
            && !(GetMinWordLength() > i_word_length)
            )
        {
            AddWord(st_buffer);
        }
      
    }  
 
    fclose(fp);
    return true; //success
}
void CWordList::AddWordPair(char *p_word, char *p_word_optimized)
{
    CWord new_word;

    new_word.s_word = p_word;
    new_word.s_sorted_word = p_word_optimized;
    a_words.push_back(new_word);
 
}


void CWordList::AddWord(char *p_word)
{
    CWord new_word;

    new_word.s_word = p_word;
    a_words.push_back(new_word);
 
}

void CWordList::BuildSortedWordFromWord(CWord &new_word)
{
     string st_temp = new_word.s_word;
    new_word.s_sorted_word = new_word.s_word;

    //let's sort this string alphabetically

      for (unsigned int c=0; c < new_word.s_word.size(); c++ )
    {
        char c_chosen = 32;
        int i_index_of_temp_chosen = 0;
        for (unsigned int i=0; i < new_word.s_word.size(); i++)
        {
            //go through each letter and grab the highest one
            
            if (c_chosen < st_temp[i])
            {
                c_chosen = st_temp[i];
                i_index_of_temp_chosen = i;
            }
        }
    
        new_word.s_sorted_word[(new_word.s_word.size()-1)-c] = c_chosen;
        st_temp[i_index_of_temp_chosen] = 32; //blank it out so we won't count it again
   }


}


void CWordList::StripMiscWord(char *p_word)
{
    replace("'", "", p_word);
    replace("-", "", p_word);
    replace(" ", "", p_word);

}

void CWordList::RemoveAccentsWord(char *p_word)
{
   // replace("\`a", "a", p_word);
    
}

void CWordList::ConvertToLowerCaseWord(char *p_word)
{
    for (int i=0; p_word[i]; i++)
    {
        p_word[i] = tolower(p_word[i]);
    }
}

void CWordList::SortAndAddSortedWord()
{
  int i_count = 0;
    LogMsg("Sorting word list...");
    for (unsigned int i=0; i < a_words.size(); i++) 
    {
        BuildSortedWordFromWord(a_words[i]);
        if (i_count < 1)
        {
          //  LogMsg("Built %s from %s", a_words[i].s_sorted_word.c_str(),
          //      a_words[i].s_word.c_str());
            i_count = C_DEBUG_OUTPUT_SKIP;
        }   else
        {
            i_count--;
        }
    }
    std::stable_sort(a_words.begin(), a_words.end());
}


void CWordList::PrintList()
{
   for (unsigned int i=0; i < a_words.size(); i++) 
   {
       LogMsg("Word %d: %s (%s)", i, a_words[i].s_sorted_word.c_str(),
           a_words[i].s_word.c_str());
   }

}


bool CWordList::SaveOutWordList(const char *fname)
{
  int i_count = 0;
   FILE *fp = fopen(fname, "wb");

   if (!fp)
   {
       LogError("Error creating file %s to save to.", fname);
       return false;
   }
  
   char st_buffer[4096];

   for (unsigned int i=0; i < a_words.size(); i++) 
   {
        if (a_words[i].s_sorted_word[0] == 0)
       {
           //um, this list wasn't sorted. Only write out one word.
           sprintf(st_buffer, "%s\r\n",a_words[i].s_word.c_str());
           
       }   else
       {
           sprintf(st_buffer, "%s %s\r\n",a_words[i].s_sorted_word.c_str(),
               a_words[i].s_word.c_str());
       }
       
       fputs(st_buffer, fp);
       
       if (i_count < 1)
       {
          // LogMsg("Saving out %d (%s from %s)", i, a_words[i].s_sorted_word.c_str(),
           //    a_words[i].s_word.c_str());
           i_count = C_DEBUG_OUTPUT_SKIP;
       }   else
       {
           i_count--;
       }
       
   }
   
   fclose(fp);

    LogMsg("Operation complete, saved out %d words.", a_words.size());
    return true;
}


bool CWordList::IsWordInDictionary(char *p_word)
{
   CWord word;
   word.s_word = p_word;
   BuildSortedWordFromWord(word);
   
   //actually check to see if this word is in the dictionary


   int i_bottom_limit = 0;
   int i_top_limit = (this->a_words.size());
   int i_cur_index = ((i_top_limit-i_bottom_limit-1)/2)+i_bottom_limit;  //start in the middle
   int i_count = 0; 
   while (1)
   {
       i_count++;
       if (word.s_sorted_word== a_words[i_cur_index].s_sorted_word)
       {
           //ok, we matched the letters but this doesn't mean we found the exact word yet.
           //because of how the matching system works to be safe we need to go to the
           //top and check all that start with this letter group..

           //this code could probably be taken out for a huge speed improvement but there
           //are places in the dictionary where although very rare, without this it would
           //miss their word.

           while (
               (a_words[i_cur_index-1].s_sorted_word == a_words[i_cur_index].s_sorted_word)
               && (i_cur_index > 0)
               )
           {
               i_cur_index--;
           }
           //ok, moved to the top.  Now scroll down and check each one against our word.
           while ( (a_words[i_cur_index].s_sorted_word == word.s_sorted_word) 
               && (i_cur_index < int(a_words.size())))
           {
             if (a_words[i_cur_index].s_word == word.s_word)
                 
             {
          //    LogMsg("Match found in %d tries.", i_count);
               return true;
             } else
             {
                 i_cur_index++;
             }
           }
             

//         LogMsg("Failed but had word possible in %d tries.", i_count);
               return false;
       }
       
       if (number_is_close(i_top_limit, i_bottom_limit, 1))
       {
     //     LogMsg("Word not found after %d tries. (orig: %s, found %s)", i_count,
         //    word.s_sorted_word.c_str(), a_words[i_cur_index].s_sorted_word.c_str());
           return false;
       }
       
       if (word.s_sorted_word < a_words[i_cur_index].s_sorted_word)
       {
           //we're closer to the start
           i_top_limit = i_cur_index;
           i_cur_index = (((i_top_limit-i_bottom_limit)/2)+i_bottom_limit);
       }  else
       {
           //we're closer to the end
           i_bottom_limit = i_cur_index;
           i_cur_index = ((i_top_limit-i_bottom_limit)/2)+i_bottom_limit;
           
       }

       if (i_count > 10000) //emergency safeguard
       {
           LogMsg("Failed!");
           return false;
       }

   }
   return false;
}


bool CWordList::CheckThisForAnagrams(char *p_word)
{
   CWord word;
   word.s_word = p_word;
   BuildSortedWordFromWord(word);
   
   //LogMsg("Checking %s", word.s_sorted_word.c_str());
   //actually check to see if this word is in the dictionary

   int i_bottom_limit = 0;
   int i_top_limit = (this->a_words.size());
   int i_cur_index = ((i_top_limit-i_bottom_limit-1)/2)+i_bottom_limit;  //start in the middle
   int i_count = 0; 
   while (1)
   {
       i_count++;
       if (word.s_sorted_word== a_words[i_cur_index].s_sorted_word)
       {
           //ok, we matched the letters but this doesn't mean we found the exact word yet.
           //because of how the matching system works to be safe we need to go to the
           //top and check all that start with this letter group..

           //this code could probably be taken out for a huge speed inmprovement but there
           //are places in the dictionary where although very rare, without this it would
           //miss their word.

           while (
               (a_words[i_cur_index-1].s_sorted_word == a_words[i_cur_index].s_sorted_word)
               && (i_cur_index > 0)
               )
           {
        //       LogMsg("Pushed back");
               i_cur_index--;
           }
           //ok, moved to the top.  Now scroll down and check each one against our word.
           while ((i_cur_index < int(a_words.size())) &&
               (a_words[i_cur_index].s_sorted_word == word.s_sorted_word) )
           {
//             LogMsg("Match found in %d tries. (%s)", i_count, a_words[i_cur_index].s_word.c_str());
               
               if (
                   (
                     (!m_p_words_to_ignore_list) || 
                      ( (m_p_words_to_ignore_list) && (!m_p_words_to_ignore_list->IsWordOnList(a_words[i_cur_index].s_word)) )
                   )
                  &&
                   (!m_anagram_list.IsWordOnList(a_words[i_cur_index].s_word))
                  )

                   m_anagram_list.AddWord(a_words[i_cur_index].s_word);
               i_cur_index++;
           }
    
      //   LogMsg("Failed but had word possible in %d tries.", i_count);
               return false;
       }
       
       if (number_is_close(i_top_limit, i_bottom_limit, 1))
       {
          //LogMsg("Word not found after %d tries. (orig: %s, found %s)", i_count,
           //  word.s_sorted_word.c_str(), a_words[i_cur_index].s_sorted_word.c_str());
           return false;
       }
       
       if (word.s_sorted_word < a_words[i_cur_index].s_sorted_word)
       {
           //we're closer to the start
           i_top_limit = i_cur_index;
           i_cur_index = (((i_top_limit-i_bottom_limit)/2)+i_bottom_limit);
       }  else
       {
           //we're closer to the end
           i_bottom_limit = i_cur_index;
           i_cur_index = ((i_top_limit-i_bottom_limit)/2)+i_bottom_limit;
           
       }

        if (i_count > 10000)  //emergency safeguard
        {
           LogMsg("Failed!");
            return false;
        }

   }
   return false;
}



void CWordList::AnagramCombination(char *str, int i_start_length)
{
 
   int i,j,l,*p;
   char *r;
  
   int i_total = 0;
   l=strlen(str);
  
   char r_buffer[128];
   int p_buffer[128];

    p=&p_buffer[0];
    r = &r_buffer[0];
  
  for(int s=i_start_length; s<=l; ++s) // permutation length
   {   for(i=0; i<s; ++i)p[i]=i; //initialize characters to get
       do
       {   for(i=0; i<s; ++i)r[i]=str[p[i]];
           r[s]='\0';
          //LogMsg("%s\n",r);
           CheckThisForAnagrams(r);
           i_total++;
          for(i=s-1; i>=0; --i)
              // increment least significant index 
           {   ++p[i];
               // make subsequent indexes contiguous 
               for(j=i+1; j<s; ++j)p[j]=p[i]+(j-i);
               if(p[s-1]<l)break;
           }
       } while(p[s-1]<l);
   }

   //LogMsg("Found %d total combinations", i_total);
   return;
}


//clear the anagram word list and add a bunch of new ones to it
//this actually uses a combinatorial function to get all words possible, not just the
//ones the same length as p_word.

//if p_words_to_ignore isn't null, any words matching these will not be allowed.

int CWordList::GenerateAnagrams(char *p_word, const CWordsUsed *p_words_to_ignore)
{
  //let's do it
 
   m_p_words_to_ignore_list = const_cast<CWordsUsed * const>(p_words_to_ignore); 
    
   CWord word;
   word.s_word = p_word;
   BuildSortedWordFromWord(word);
 
   char st_current[100];

   int i_len = strlen(p_word);
   strcpy(st_current, word.s_sorted_word.c_str());
   
   m_anagram_list.Clear();
   
   //the 3 is the smallest word size to check for
   AnagramCombination(st_current, 3);

   //LogMsg("Found %d words. (checked %s)", m_anagram_list.Count(), word.s_sorted_word.c_str());
 
   //the list fo words we found is now available through GetAnagramsFound()
   //for debugging you can print the list of works like this
   //GetAnagramsFound()->PrintWords();

   return 0;
}
