/**  
 * Copyright © 2016Kookong.com. All rights reserved.
 *
 * @title: NonAcActivity.java
 * @prject: kksdk_ir_acext_sample
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2016年9月8日 上午10:30:55
 * @version: V1.2.0  
 */
package com.example.kk_simple;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.kookong.app.data.AppConst;
import com.kookong.app.data.IrData;
import com.kookong.app.data.IrDataList;
import com.kookong.app.data.IrData.IrKey;

/**
 * @className: NonAcActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2016年9月8日 上午10:30:55
 */
public class NonAcActivity extends Activity
{
    public GridView mGridView;
    private Button mTestir_btn;
    private EditText mTestir_et_rid;
    private List<IrKey> mIrKeys = new ArrayList<IrKey>();
    private int mRemoteId;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nonac);
        initView();
    }
    
    private void initView() {
        mTestir_et_rid = (EditText) findViewById(R.id.testir_et_rid);
        mTestir_btn = (Button) findViewById(R.id.testir_btn);
        mGridView = (GridView) findViewById(R.id.testir_gv);
        mGridView.setAdapter(new IrDataAdapter());
        mTestir_btn.setOnClickListener(new OnClickListener()
        {
            
            @Override
            public void onClick(View v) {
                final String rid = mTestir_et_rid.getText().toString().trim();
                KookongSDK.getIRDataById(rid, Device.TV, new IRequestResult<IrDataList>()
                {
                    
                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        if (irDatas.size() <= 0) {
                            TipsUtil.toast(NonAcActivity.this, "遥控码" + rid + "不存在");
                            mIrKeys = new ArrayList<IrKey>();
                            ((IrDataAdapter) mGridView.getAdapter()).notifyDataSetChanged();
                            return;
                        }
                        IrData irData = irDatas.get(0);
                        mIrKeys = irData.keys;
                        mRemoteId = irData.rid;
                        ((IrDataAdapter) mGridView.getAdapter()).notifyDataSetChanged();
                        TipsUtil.toast(NonAcActivity.this, "遥控器数据下载成功");
                    }
                    
                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        //按红外设备授权的客户，才会用到这两个值
                        if (errorCode == AppConst.CUSTOMER_DEVICE_REMOTE_NUM_LIMIT) {//同一个设备下载遥控器超过了50套限制
                            msg = "下载的遥控器超过了套数限制";
                        }
                        else if (errorCode == AppConst.CUSTOMER_DEVICE_NUM_LIMIT) {//设备总数超过了授权的额度
                            msg = "设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(NonAcActivity.this, msg);
                    }
                });
                
            }
        });
    }
    
    private class IrDataAdapter extends BaseAdapter
    {
        
        @Override
        public int getCount() {
            return mIrKeys.size();
        }
        
        @Override
        public Object getItem(int arg0) {
            return mIrKeys.get(arg0);
        }
        
        @Override
        public long getItemId(int arg0) {
            return arg0;
        }
        
        @Override
        public View getView(int arg0, View arg1, ViewGroup arg2) {
            Button textView = new Button(NonAcActivity.this);
            textView.setHeight(150);
            textView.setGravity(Gravity.CENTER);
            final IrKey irKey = mIrKeys.get(arg0);
            textView.setText(irKey.fname);
            textView.setOnClickListener(new OnClickListener()
            {
                
                @Override
                public void onClick(View arg0) {                    
                    Logger.d("remote (" + mRemoteId + ") key (" + irKey.fkey + ") is " + irKey.pulse);
                }
            });
            return textView;
        }
        
    }
}
