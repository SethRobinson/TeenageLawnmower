#include "CWordsUsed.h"
#include <cassert>

void CWordsUsed::Clear()
{
    //clear the shiznit out
    a_word.clear();
}


void CWordsUsed::AddWord(char *p_word)
{

    CUsedWord new_word;
    new_word.s_word = p_word;

    a_word.push_back(new_word);

}

void CWordsUsed::AddWord(string p_word)
{

    CUsedWord new_word;
    new_word.s_word = p_word;

    a_word.push_back(new_word);

}

bool CWordsUsed::IsWordOnList(char *p_word)
{

  for (unsigned int i=0; i < a_word.size(); i++)
  {
      if (a_word[i].s_word == p_word)
      {
          return true;
      }
  }
 
  return false;
}


bool CWordsUsed::IsWordOnList(string p_word)
{

  for (unsigned int i=0; i < a_word.size(); i++)
  {
      if (a_word[i].s_word == p_word)
      {
          return true;
      }
  }
 
  return false;
}

 void CWordsUsed::PrintWords()
{

  for (unsigned int i=0; i < a_word.size(); i++)
  {
      LogMsg("Word %d - %s", i, a_word[i].s_word.c_str());
  }
 
}

const string CWordsUsed::GetLastWordAdded()
{
    if (Count() < 1) return NULL;

    return a_word[Count()-1].s_word;
}


const string CWordsUsed::GetLongestWord()
{
    if (Count() == 0) return NULL;

    int i_longest_word = 0;
    
    int i_index_of_longest = -1;

    for (unsigned int i=0; i < a_word.size(); i++)
    {
        if (int(a_word[i].s_word.length()) > i_longest_word)
        {
            i_index_of_longest = i;
        }
    }
 
    if (i_index_of_longest == -1)
    {
        LogError("Couldn't find longest word..");
        return NULL;
    }
    return a_word[i_index_of_longest].s_word;

}

bool CWordsUsed::Save(FILE *fp)
{
    //first save how many words there are
    int i_count = Count();
    fwrite(&i_count, sizeof(int), 1, fp);

    //write out each word
  
    int i_letters_in_word;
    
    for (int i=0; i < i_count; i++)
    {
        //first save its length
        i_letters_in_word   = a_word[i].s_word.size();
        fwrite(&i_letters_in_word, sizeof(int), 1, fp);
        
   //     LogMsg("Writing word %d which is %d characters.", i, i_letters_in_word);
        //now write the actual string out
        fwrite(a_word[i].s_word.c_str(), i_letters_in_word, 1, fp);
    }

    return true;
}

bool CWordsUsed::Load(FILE *fp)
{
    //clear words that were in the list first
    Clear();

    
    int i_count = 0;
    fread(&i_count, sizeof(int), 1, fp);

    char st_buffer[2048];

    for (int i=0; i < i_count; i++)
    {
        //get the size in letters
        int i_letters_in_word;
        fread(&i_letters_in_word, sizeof(int), 1, fp);
        assert(i_letters_in_word < 2048 && "What the wholly hell kind of word is this?");

        //put it in our buffer
        fread(&st_buffer, i_letters_in_word, 1, fp);
        //add the null at the end
       st_buffer[i_letters_in_word] = 0;
       //LogMsg("Loaded the word %s.", st_buffer);
       AddWord(st_buffer);
    }
   return true; //success
}