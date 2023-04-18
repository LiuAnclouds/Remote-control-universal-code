package com.example.kk_simple.util;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.annotation.NonNull;
import android.widget.Toast;

public class TipsUtil {
    private static Context context;
    private static Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            toast(TipsUtil.context, (String) msg.obj, msg.arg1);
            TipsUtil.context = null;
        }
    };

    public static void toast(Context context, String msg) {
        toast(context, msg, Toast.LENGTH_SHORT);
    }

    public static void toast(Context context, String msg, int time) {
        if (Looper.myLooper() == Looper.getMainLooper()) {
            Toast sToast = Toast.makeText(context, msg, time);
            sToast.show();
        } else {
            TipsUtil.context = context;
            Message mMsg = new Message();
            mMsg.obj = msg;
            mMsg.arg1 = time;
            handler.sendMessage(mMsg);
        }
    }

}
