package com.example.kk_simple.util;

import android.content.Context;
import android.hardware.ConsumerIrManager;
import android.widget.Toast;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class IrUtil {
    /**
     * 发码工具类
     * 最小发码间隔300毫秒
     * 队列发码
     * 处理翻转码
     * 关于翻转码：以&分隔的两组码，需要交替发送这两组码，在酷控码库中占比很少，建议统一处理
     * 如果不处理翻转码，只以,分隔pulse字段然后转化成int数组，会导致app crash(其中包含&字符不能转成int类型)
     */
    private int reversCounter = 0;
    private long lastSendTime;
    //最小发码间隔 两次发码间隔大于300毫秒 避免干扰
    private long MIN_SEND_INTERVAL = 300;
    private Context context;
    //单线程发码保证先后顺序
    private ExecutorService service = Executors.newSingleThreadExecutor();

    public IrUtil(Context context) {
        this.context = context;
    }

    public void sendIr(int fre, int[] patterns) {
        sendIr(fre, null, patterns);
    }

    public void sendIr(int fre, String pulse) {
        sendIr(fre, pulse, null);
    }

    private void sendIr(final int frequence, final String pulse, final int[] intPattern) {
        service.submit(new Runnable() {
            @Override
            public void run() {
                long timeOff = System.currentTimeMillis() - lastSendTime - MIN_SEND_INTERVAL;
                //发码间隔不满300ms 等待满300毫秒后发送
                if (timeOff < 0) {
                    try {
                        Thread.sleep(-timeOff);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                if (null != pulse)
                    sendIrSync(frequence, pulse);
                else
                    sendIrSync(frequence, intPattern);
                lastSendTime = System.currentTimeMillis();
            }
        });
    }

    private void sendIrSync(int fre, int[] patterns) {
        final ConsumerIrManager consumerIrManager = (ConsumerIrManager) context.getSystemService(Context.CONSUMER_IR_SERVICE);
        if (null == consumerIrManager) {
            TipsUtil.toast(context, "获取ConsumerIr失败", Toast.LENGTH_SHORT);
            return;
        }
        if (!consumerIrManager.hasIrEmitter()) {
            TipsUtil.toast(context, "不支持红外", Toast.LENGTH_SHORT);
            return;
        }
        consumerIrManager.transmit(fre, patterns);
    }

    private void sendIrSync(final int frequence, String pulse) {
        //首先分割翻转码 翻转码：以&分割为两组码,发送时需要交替发送
        String[] sArray = pulse.split("&");
        int[][] patterns = new int[sArray.length][];
        for (int i = 0; i < sArray.length; i++) {
            //分割时间数组字符串 转化为int数组
            String[] sarr = sArray[i].split(",");
            int[] iarr = new int[sarr.length];
            for (int j = 0; j < sarr.length; j++) {
                iarr[j] = Integer.parseInt(sarr[j].trim());
            }
            patterns[i] = iarr;
        }
        //无翻转码 直接发送
        if (patterns.length == 1)
            sendIrSync(frequence, patterns[0]);
        else {
            //翻转码 交替发送 patterns[0] -> patterns[1] -> patterns[0] -> patterns[1]
            sendIrSync(frequence, reversCounter % 2 == 0 ? patterns[0] : patterns[1]);
            reversCounter++;
        }
    }
}
