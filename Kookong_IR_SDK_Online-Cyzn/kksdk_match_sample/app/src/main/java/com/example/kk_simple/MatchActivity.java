/**  
 * Copyright © 2017Kookong.com. All rights reserved.
 *
 * @title: MatchActivity.java
 * @prject: kksdk_full_sample_single
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2017年9月20日 下午1:52:59
 * @version: V1.2.0  
 */
package com.example.kk_simple;

import java.util.List;

import android.R.plurals;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ViewFlipper;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.IntentConstants;
import com.example.kk_simple.util.IrUtil;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KKSingleMatchManager;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.interf.ISingleMatchResult;
import com.hzy.tvmao.utils.LogUtil;
import com.kookong.app.data.IrData;
import com.kookong.app.data.IrDataList;
import com.kookong.app.data.RcRemoteKeyExt;
import com.kookong.app.data.RcTestRemoteKeyV3;

/**
 * @className: MatchActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2017年9月20日 下午1:52:59
 */
public class MatchActivity extends Activity implements OnClickListener
{
    private TextView mDeviceNameText, mTestNumText;
    private View mBottomView;
    private Button mTestKetBt;
    
    private String mDeviceName;
    private String mAllRemoteIds;
    private int mDeviceType;
    private KKSingleMatchManager singleMatch;
    
    private ViewFlipper mViewFlipper;
    private TextView mMatchResultText;
    
    private TextView mKeyRemoteIdText;
    
    private Button mDownloadBt;
    
    //当前测试的一组按键
    private List<RcTestRemoteKeyV3> testKeyList;
    
    //记录当前测试的是按键下标
    private int mIndex;
    
    //记录匹配到的红外码Id
    private String matchedRemoteId;
    
    //压缩码
    private boolean mIsZip;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match);
        mDeviceNameText = (TextView) findViewById(R.id.test_device_name);
        mTestNumText = (TextView) findViewById(R.id.test_key_num);
        mTestKetBt = (Button) findViewById(R.id.test_key_bt);
        mBottomView = findViewById(R.id.test_bottom_view);
        mViewFlipper = (ViewFlipper) findViewById(R.id.match_flipper);
        mMatchResultText = (TextView) findViewById(R.id.match_result);
        mKeyRemoteIdText = (TextView) findViewById(R.id.test_key_remoteid);
        mDownloadBt = (Button) findViewById(R.id.download_ir);
        initData();
    }
    
    private void initData() {
        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            mDeviceType = bundle.getInt("device_type");
            mAllRemoteIds = bundle.getString("all_remote_id");
            mDeviceName = bundle.getString("device_name");
            mIsZip = bundle.getBoolean(IntentConstants.IR_IS_ZIPCODE);
            initView();
            httpGetTestKey(mDeviceType, mAllRemoteIds, true);
        }
        
    }
    
    private void initView() {
        mDeviceNameText.setText(mDeviceName);
    }
    private IrUtil irUtil=new IrUtil(this);
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.test_key_bt://测试按键
                RcTestRemoteKeyV3 testRemoteKeyV3 = getCurTestKey(mIndex);
                String pulseData = testRemoteKeyV3.pulseData;
                TipsUtil.toast(MatchActivity.this, pulseData);
                LogUtil.d("pulseData="+pulseData);
                //发码
                irUtil.sendIr(testRemoteKeyV3.frequency,testRemoteKeyV3.pulseData);
                if(mIsZip){//如果是压缩的,需要用到pulseData和tag=99999的数据
                   List<RcRemoteKeyExt> keyExtList = testRemoteKeyV3.remoteKeyExtList;
                   for(int i=0;i<keyExtList.size();i++){
                       RcRemoteKeyExt keyExt = keyExtList.get(i);
                       if(keyExt.getTag() == 99999){
                           LogUtil.d("99999="+keyExt.getValue());
                           break;
                       }
                   }
                }
                mBottomView.setVisibility(View.VISIBLE);
                break;
            
            case R.id.test_faild://测试键发码后电器无响应
                getGroupNextKey(testKeyList);
                break;
            case R.id.test_ok://测试键发码后电器响应
                reportWorkKey(getCurTestKey(mIndex));
                break;
            case R.id.download_ir://下载匹配到的红外码
                downloadIR(matchedRemoteId);
                mDownloadBt.setVisibility(View.INVISIBLE);
                break;
        }
    }
    
    /**
     * 匹配入口
     * 
     * @title: httpGetTestKey
     * @description: TODO
     * @param deviceType
     * @param allRemoteId
     * @param testSwitch
     * @return: void
     */
    private void httpGetTestKey(int deviceType, String allRemoteId, boolean testSwitch) {
        if(mIsZip){
            singleMatch = new KKSingleMatchManager(mIsZip);
        }else{
            singleMatch = new KKSingleMatchManager();
        }
        //匹配入口函数，获取匹配的按键，testSwitch是否测试电源键，如果电器是开机状态，传false跳过测试电源，测试其他键
        singleMatch.getMatchKey(deviceType, allRemoteId, testSwitch, iSingleMatchResult);
    }
    
    /**
     * 一组按键中某个按键对电器有响应，调用此方法
     * 
     * @title: reportWorkKey
     * @description: TODO
     * @param key
     * @return: void
     */
    private void reportWorkKey(RcTestRemoteKeyV3 key) {
        singleMatch.keyIsWorking(key, iSingleMatchResult);
    }
    
    /**
     * 一组按键对电器都不响应，调用此方法
     * 
     * @title: reportGroupKeyNotWork
     * @description: TODO
     * @param keyList
     * @return: void
     */
    private void reportGroupKeyNotWork(List<RcTestRemoteKeyV3> keyList) {
        singleMatch.groupKeyNotWork(keyList, iSingleMatchResult);
    }
    
    private void downloadIR(String remoteId) {
        //传入remoteId下载对应的红外码，可以一次下载多套红外码，每个remoteid之间用逗号分隔
        doDownloadIR(remoteId, mIsZip, new IRequestResult<IrDataList>()
        {
            
            @Override
            public void onSuccess(String msg, IrDataList result) {
                List<IrData> list = result.getIrDataList();
                if (list != null && list.size() > 0) {
                    TipsUtil.toast(MatchActivity.this, "红外码下载成功");
                    IrData irData = list.get(0);
                }
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchActivity.this, "红外码下载失败：code=" + errorCode + ",msg=" + msg);
                Logger.d("红外码下载失败：code=" + errorCode + ",msg=" + msg);
            }
        });
    }
    
    private void doDownloadIR(String remoteId,boolean isZip,IRequestResult<IrDataList> requestResult){
        if(mIsZip){
            KookongSDK.getIRDataById(remoteId, mDeviceType,true, requestResult);
        }else{
            KookongSDK.getIRDataById(remoteId, mDeviceType,requestResult);
        }
    }
    
    ISingleMatchResult iSingleMatchResult = new ISingleMatchResult()
    {
        
        @Override
        public void onNotMatchIR() {//匹配流程结束，没有匹配到可使用的红外码
            showMatchResultView("匹配结束\n没有匹配的红外码", false);
        }
        
        @Override
        public void onNextGroupKey(List<RcTestRemoteKeyV3> groupKeyList) {//返回一组需要测试的键
            testKeyList = groupKeyList;
            mIndex = 0;
            //显示一个测试按键
            showTestKey(mIndex);
        }
        
        @Override
        public void onMatchedIR(String remoteId) {//匹配结束，匹配到了可使用的红外码
            matchedRemoteId = remoteId;
            showMatchResultView("匹配结束\n匹配到红外码：" + remoteId, true);
            
        }
        
        @Override
        public void onError() {//匹配过程中出错（网络及其他问题），重新走匹配流程
            showMatchErrorView();
            
        }
    };
    
    //显示
    private void showTestKey(int index) {
        if (mTestKetBt.getVisibility() != View.VISIBLE) {
            mTestKetBt.setVisibility(View.VISIBLE);
        }
        RcTestRemoteKeyV3 testKeyV3 = testKeyList.get(mIndex);
        mTestKetBt.setText(testKeyV3.displayName + (mIndex + 1));
        mTestNumText.setText((index + 1) + "/" + testKeyList.size());
        mKeyRemoteIdText.setText(testKeyV3.remoteIds);
        mBottomView.setVisibility(View.INVISIBLE);
    }
    
    //同一组按键依次测试
    private void getGroupNextKey(List<RcTestRemoteKeyV3> groupKeyList) {
        if (mIndex == groupKeyList.size() - 1) {//最后一个测试键了，还是不工作
            LogUtil.d("这一组按键都不工作");
            reportGroupKeyNotWork(groupKeyList);
            return;
        }
        LogUtil.d("测试这一组的下一个键");
        mIndex++;
        showTestKey(mIndex);
    }
    
    //获取当前测试的某个按键
    private RcTestRemoteKeyV3 getCurTestKey(int index) {
        return testKeyList.get(index);
    }
    
    //显示匹配错误页面
    private void showMatchErrorView() {
        mViewFlipper.setDisplayedChild(1);
    }
    
    //显示匹配结果页面
    private void showMatchResultView(String msg, boolean matched) {
        mMatchResultText.setText(msg);
        mViewFlipper.setDisplayedChild(2);
        if (matched) {
            mDownloadBt.setVisibility(View.VISIBLE);
        }
        else {
            mDownloadBt.setVisibility(View.INVISIBLE);
        }
    }
    
}
