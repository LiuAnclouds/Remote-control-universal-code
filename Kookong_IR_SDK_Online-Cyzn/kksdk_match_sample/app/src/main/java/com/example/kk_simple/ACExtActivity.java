package com.example.kk_simple;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.annotation.NonNull;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.TextView;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.adapter.AcExtPadAdapter;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KKACManagerV2;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.hzy.tvmao.ir.ac.ACConstants;
import com.kookong.app.data.IrData;
import com.kookong.app.data.IrDataList;

import java.util.Timer;
import java.util.TimerTask;

public class ACExtActivity extends Activity {
    private TextView btn_sleep, btn_timing_op, tv_timing_display, btn_timing_add, btn_timing_minus;
    private GridView gridView;
    private AcExtPadAdapter adapter;
    private KKACManagerV2 kkacManagerV2;
    private IrData mIrdata;

    public static void start(Context context, int rid, String acState) {
        Intent starter = new Intent(context, ACExtActivity.class);
        starter.putExtra("rid", rid);
        starter.putExtra("acState", acState);
        context.startActivity(starter);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_acext);
        btn_sleep = findViewById(R.id.btn_sleep);
        btn_timing_op = findViewById(R.id.btn_timing_op);
        tv_timing_display = findViewById(R.id.tv_timing_display);
        btn_timing_add = findViewById(R.id.btn_timing_add);
        btn_timing_minus = findViewById(R.id.btn_timing_minus);
        gridView = findViewById(R.id.gv);
        adapter = new AcExtPadAdapter();
        gridView.setAdapter(adapter);

        setListeners();
        kkacManagerV2 = new KKACManagerV2();
        int rid = getIntent().getIntExtra("rid", 0);
        final String acState = getIntent().getStringExtra("acState");
        KookongSDK.getIRDataById(rid + "", Device.AC, new IRequestResult<IrDataList>() {
            @Override
            public void onSuccess(String s, IrDataList irDataList) {
                IrData irData = mIrdata = irDataList.getIrDataList().get(0);
                kkacManagerV2.initIRData(irData);
                kkacManagerV2.setACStateV2FromString(acState);
                //展示所有扩展建，定时按键不包括在内，定时按键比较特殊，有专门方法判断
                adapter.setList(irData.keys, kkacManagerV2);
                updateSleepState();
                updateTimingState();
                updateDiplayTime();
            }

            @Override
            public void onFail(Integer integer, String s) {
                TipsUtil.toast(ACExtActivity.this, "Error:" + s);
            }
        });
    }

    private void updateDiplayTime() {
        int displayTime = kkacManagerV2.getDisplayTime();
        //定时时间 单位分钟
        tv_timing_display.setText(displayTime + "分钟");
    }

    private void setListeners() {
        btn_sleep.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                kkacManagerV2.changeExpandKeyState(ACConstants.FUNCTION_SLEEP);
                updateSleepState();
            }
        });
        btn_timing_op.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                kkacManagerV2.operateTimeing();
                updateTimingState();
                updateDiplayTime();
            }
        });

        btn_timing_add.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (kkacManagerV2.isTimeingCanUse()) {
                    kkacManagerV2.increaseTime();
                    updateDiplayTime();
                }
            }
        });
        btn_timing_minus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (kkacManagerV2.isTimeingCanUse()) {
                    kkacManagerV2.decreaseTime();
                    updateDiplayTime();
                }
            }
        });

        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                IrData.IrKey irKey = mIrdata.keys.get(position);
                if (kkacManagerV2.isExpandCanUse(irKey.fid)) {
                    kkacManagerV2.changeExpandKeyState(irKey.fid);
                    //刷新状态显示
                    adapter.notifyDataSetChanged();
                } else {
                    TipsUtil.toast(parent.getContext(), irKey.fname + "不可用");
                }
            }
        });
    }

    private void updateTimingState() {
        // sdk内部不维护定时器 所以不知道定时是否结束 所以需要手动调用用timeingCheck方法检查定时状态
        // 每次刷新定时状态前调用timeingCheck方法 不然定时计时结束以后isTimingBeenSet方法获取到的状态是错的
        kkacManagerV2.timeingCheck();
        //定时按键是否好用
        btn_timing_op.setEnabled(kkacManagerV2.isTimeingCanUse());
        //已经设置了定时 就展示剩余定时时间
        if (kkacManagerV2.isTimingBeenSet()) {
            startTimingTimer();
        } else {
            cancenTimingTimer();
            btn_timing_op.setText("定时");
        }
    }

    private Timer timer;
    private Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            long offset = (kkacManagerV2.getTimeingEndTime() - System.currentTimeMillis()) / 1000;
            //offset小于0说明定时已经结束
            if (offset > 0) {
                String str = (offset / 3600) + ":" + (offset % 3600 / 60) + ":" + (offset % 60);
                btn_timing_op.setText("定时-" + str);
            } else {
                //定时结束 调用timeingCheck方法刷新定时状态 定时由已设置变回未设置
                kkacManagerV2.timeingCheck();
                btn_timing_op.setText("定时");
                //过期就取消掉定时器
                cancenTimingTimer();
            }
        }
    };

    private void cancenTimingTimer() {
        if (null != timer) {
            timer.cancel();
            timer = null;
        }
    }

    private void startTimingTimer() {
        if (null == timer) {
            timer = new Timer();
            //启动一个没有时间限制的定时器 结束条件在handler里
            timer.schedule(new TimerTask() {
                @Override
                public void run() {
                    handler.sendEmptyMessage(0);
                }
            }, 0, 1000);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        cancenTimingTimer();
    }

    private void updateSleepState() {
        btn_sleep.setEnabled(kkacManagerV2.isExpandCanUse(ACConstants.FUNCTION_SLEEP));
        int state = kkacManagerV2.getExpandKeyState(ACConstants.FUNCTION_SLEEP);
        btn_sleep.setText("睡眠：" + state);
    }
}