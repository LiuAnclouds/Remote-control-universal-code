package com.example.kk_simple.util;

import android.content.Context;
import android.widget.Toast;

public class TipsUtil
{
    
    private static Toast toast;
    public static void toast(Context context, String msg) {
        toast(context, msg, Toast.LENGTH_SHORT);
    }
    
    public static void toast(Context context, String msg, int time) {
        if(toast!=null){
            toast.cancel();
        }
        toast = Toast.makeText(context, msg, time);
        //Toast sToast = Toast.makeText(context, msg, time);
        toast.show();
    }
    
}
