#include "CRanking.h"


CRanking::~CRanking()
{
    Clear();
}
void CRanking::Clear()
{
     for (list<CScoreObject*>::iterator itor=m_list_scores.begin(); itor != m_list_scores.end(); ++itor)
    {
        delete (*itor);
    }

    m_list_scores.clear();
}

CScoreObject * CRanking::AddRecord()
{
    CScoreObject *p_new = new CScoreObject();
    m_list_scores.push_back(p_new);
    return p_new;

};

template<> bool 
std::greater<CScoreObject*>::operator()(CScoreObject* const &x, CScoreObject* const &y) const
{
  return y->GetSort() <  x->GetSort();  //don't worry, these will get inlined
} 

void CRanking::SortList()
{

   m_list_scores.sort(std::greater<CScoreObject*>());

}

bool CRanking::IsHighScore(long l_score, int i_rank_required)
{
    //first make sure the list is sorted
    SortList();

   int i_count = 1;
    //go through the scores (which are in order) and see how we rate
    for (list<CScoreObject*>::iterator itor=m_list_scores.begin(); itor != m_list_scores.end();)
    {
        if ((*itor)->m_l_points < l_score)
        {
            return true; //we rate!
        }
        
         i_count++;
         if (i_count > i_rank_required)
         {
             //well, we passed into the really sucky scores, so they failed
             break;
         }

        itor++;
    }
    
   if (i_count <= i_rank_required)
   {
      return true; //they are on the list, but only because there are
      //blank spots
   }
    //failed to get a high score
    return false;
}


bool CRanking::LoadScores(char *p_fname)
{
  
    //clear our current scores
    Clear();
    
    //first, does the stupid file exist?
    if (!exist(p_fname))
    {
        return false;
    }

    FILE *fp = fopen(p_fname, "rb");

    if (!fp)
    {
        LogError("Error opening %s", p_fname);
        return false;
    }

    //first load the record size
    int i_record_size;
    fread(&i_record_size, sizeof(int), 1, fp);
    
    ScoreSaveObject score;
    
    while (fread(&score, i_record_size, 1, fp))
    {
        CScoreObject *p_score = AddRecord();

        p_score->m_st_name = score.m_st_name;
        p_score->m_i_dragons_slain = score.m_i_dragons_slain;
        p_score->m_i_level = score.m_i_level;
        p_score->m_l_points = score.m_l_points;
    }
    
    //all done

    fclose(fp);

    SortList();
   return true;
}

//send 0 for how many to disable limiting
bool CRanking::SaveScores(char *p_fname, int i_how_many_to_save)
{
     SortList();
 
    //first, does the stupid file exist?
   
    FILE *fp = fopen(p_fname, "wb");

    if (!fp)
    {
        LogError("Error opening %s for writing", p_fname);
        return false;
    }

    //first write the size of our struct
    int i_record_size = sizeof(ScoreSaveObject());
    fwrite(&i_record_size, sizeof(int),1, fp);


    int i_count = 0;
    //now write each record
    for (list<CScoreObject*>::iterator itor=m_list_scores.begin(); itor != m_list_scores.end();)
    {
        //convert to our private record saving format
        ScoreSaveObject score;
        strcpy(score.m_st_name, (*itor)->m_st_name.c_str());
        score.m_i_dragons_slain = (*itor)->m_i_dragons_slain;
        score.m_i_level = (*itor)->m_i_level;
        score.m_l_points = (*itor)->m_l_points;

        //actually save it out
        if (!fwrite(&score, sizeof(ScoreSaveObject()), 1, fp))
        {
            LogError("Error writing out score record");
        }

        itor++;
        i_count++;

        if (i_how_many_to_save != 0)
        {
            if (i_count == i_how_many_to_save)
            {
                //don't save any more
                break;
            }
        }
    
    }


    fclose(fp);

    return true;
}
