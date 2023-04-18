package com.example.kk_simple.util;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.GridView;

public class GridViewEx extends GridView
{
    public GridViewEx(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
    
    public GridViewEx(Context context) {
        super(context);
    }
    
    public GridViewEx(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }
    
    @Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2, MeasureSpec.AT_MOST);
        super.onMeasure(widthMeasureSpec, expandSpec);
    }
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        // TODO Auto-generated method stub
        super.onLayout(changed, l, t, r, b);
    }
    
}
