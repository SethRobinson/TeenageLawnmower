class CFRect
{
public:
    
    float left;
    float top;
    float right;
    float bottom;
    
    inline void Set(float f_left, float f_top, float f_right, float f_bottom)
    {
        left = f_left;
        top = f_top;
        right = f_right;
        bottom = f_bottom;
    }
    
    inline bool PointIsInside(float x, float y)
    {
        if(x >= left && x < right &&
            y >= top  && y < bottom) return(true);
        else return(false);
        
        
    }
    
};