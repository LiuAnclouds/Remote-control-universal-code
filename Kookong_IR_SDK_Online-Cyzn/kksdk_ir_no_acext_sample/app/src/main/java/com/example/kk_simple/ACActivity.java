/**  
 * Copyright © 2016Kookong.com. All rights reserved.
 *
 * @title: ACActivity.java
 * @prject: kksdk1.2_full_sample_noepg
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2016年4月8日 下午2:10:52
 * @version: V1.2.0  
 */
package com.example.kk_simple;

import java.util.Arrays;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KKACManagerV2;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.hzy.tvmao.ir.ac.ACConstants;
import com.hzy.tvmao.ir.ac.ACStateV2.UDWindDirectKey;
import com.hzy.tvmao.ir.ac.ACStateV2.UDWindDirectType;
import com.hzy.tvmao.utils.DataStoreUtil;
import com.kookong.app.data.AppConst;
import com.kookong.app.data.IrData;
import com.kookong.app.data.IrDataList;

/**
 * @className: ACActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2016年4月8日 下午2:10:52
 */
public class ACActivity extends Activity implements OnClickListener
{
    /** 数字面板 */
    private TextView mScreen_model;
    private TextView mScreen_WindDirection_Auto;
    private TextView mScreen_WindDirection;
    private TextView mScreen_degree;
    private TextView mScreen_windSpeed;
    private TextView mScreen_timing;
    private TextView mScreen_sleep;
    
    /** 空调面板 */
    private View mACPannel;
    
    /** 红外码 */
    private IrData mIrData;
    
    /** 控制器 */
    private KKACManagerV2 mKKACManager = new KKACManagerV2();
    
    /** 搜索按钮 */
    private Button mSearchBt;
    
    /** 输入框 */
    private EditText mEditText;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ac_remote);
        initView();
        initData();
        searchACRemote();
    }
    
    private void initView() {
        mScreen_model = (TextView) findViewById(R.id.ac_state_model);
        mScreen_WindDirection_Auto = (TextView) findViewById(R.id.ac_state_wind_auto);
        mScreen_WindDirection = (TextView) findViewById(R.id.ac_state_wind);
        mScreen_degree = (TextView) findViewById(R.id.ac_state_degree);
        mScreen_windSpeed = (TextView) findViewById(R.id.ac_state_wind_speed);
        mScreen_timing = (TextView) findViewById(R.id.ac_state_timing);
        mScreen_sleep = (TextView) findViewById(R.id.ac_state_sleep);
        
        mACPannel = findViewById(R.id.ac_pannel);
        
        mSearchBt = (Button) findViewById(R.id.ac_search_btn);
        mEditText = (EditText) findViewById(R.id.ac_edit_rid);
    }
    
    private void searchACRemote() {
        mSearchBt.setOnClickListener(new OnClickListener()
        {
            
            @Override
            public void onClick(View v) {
                String rid = mEditText.getText().toString().trim();
                if (TextUtils.isEmpty(rid)) {
                    return;
                }
                KookongSDK.getIRDataById(rid, Device.AC, new IRequestResult<IrDataList>()
                {
                    
                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        mIrData = irDatas.get(0);
                        mKKACManager.initIRData(mIrData.rid, mIrData.exts, mIrData.keys);//根据空外数据初始化空调解码器
                        String acState = DataStoreUtil.i().getString("AC_STATE", "");//获取以前保存过的空调状态
                        mKKACManager.setACStateV2FromString(acState);
                        
                        updateACDisplay();//刷新空调面板
                        mACPannel.setVisibility(View.VISIBLE);
                        
                    }
                    
                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        mACPannel.setVisibility(View.INVISIBLE);
                        //按红外设备授权的客户，才会用到这两个值
                        if (errorCode == AppConst.CUSTOMER_DEVICE_REMOTE_NUM_LIMIT) {//同一个设备下载遥控器超过了50套限制
                            msg = "下载的遥控器超过了套数限制";
                        }
                        else if (errorCode == AppConst.CUSTOMER_DEVICE_NUM_LIMIT) {//设备总数超过了授权的额度
                            msg = "设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(ACActivity.this, msg);
                    }
                    
                });
                
            }
        });
    }
    
    private void initData() {
        
    }
    
    /** 更新面板和按键显示 */
    private void updateACDisplay() {
        updatePanelDisplay();
    }
    
    private void updatePanelDisplay() {
        //根据空调的开关状态，显示还是关闭面板
        if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
            mScreen_model.setVisibility(View.INVISIBLE);
            mScreen_degree.setVisibility(View.INVISIBLE);
            mScreen_WindDirection_Auto.setVisibility(View.INVISIBLE);
            mScreen_windSpeed.setVisibility(View.INVISIBLE);
            mScreen_WindDirection.setVisibility(View.INVISIBLE);
            mScreen_timing.setVisibility(View.INVISIBLE);
            mScreen_sleep.setVisibility(View.INVISIBLE);
        }
        else {
            mScreen_model.setVisibility(View.VISIBLE);
            //空调模式显示
            int modelType = mKKACManager.getCurModelType();
            String modelInfo = "";
            switch (modelType) {
                case ACConstants.AC_MODE_COOL:
                    modelInfo = "制冷";
                    break;
                case ACConstants.AC_MODE_HEAT:
                    modelInfo = "制热";
                    break;
                case ACConstants.AC_MODE_AUTO:
                    modelInfo = "自动";
                    break;
                case ACConstants.AC_MODE_FAN:
                    modelInfo = "送风";
                    break;
                case ACConstants.AC_MODE_DRY:
                    modelInfo = "除湿";
                    break;
            }
            mScreen_model.setText(modelInfo);
            
            //风量显示
            updateWindSpeedDisplay();
            
            //温度显示
            updateTmpDisplay();
            
            //风向显示           
            updateUDWindDirectDisplay();
        }
        
        //睡眠图标显示
        updateSleepDisplay();
        
        //定时图标显示
        updateTimeingDisplay();
        
    }
    
    /** 更新面板风速显示 */
    private void updateWindSpeedDisplay() {
        if (mKKACManager.isWindSpeedCanControl()) {//如果当前风速可以调节，更新面板风速显示
            mScreen_windSpeed.setVisibility(View.VISIBLE);
            int windSpeed = mKKACManager.getCurWindSpeed();//获取当前风速
            String speedInfo = "";
            switch (windSpeed) {
                case ACConstants.AC_WIND_SPEED_AUTO:
                    speedInfo = "自动风量";
                    break;
                case ACConstants.AC_WIND_SPEED_LOW:
                    speedInfo = "低";
                    break;
                case ACConstants.AC_WIND_SPEED_MEDIUM:
                    speedInfo = "中";
                    break;
                case ACConstants.AC_WIND_SPEED_HIGH:
                    speedInfo = "高";
                    break;
            }
            mScreen_windSpeed.setText(speedInfo);
        }
        else {
            mScreen_windSpeed.setVisibility(View.INVISIBLE);
        }
        
    }
    
    /** 更新面板温度显示 */
    private void updateTmpDisplay() {
        mScreen_degree.setVisibility(View.VISIBLE);
        if (mKKACManager.isTempCanControl()) {//查看当前模式下，温度是否可以调节
            mScreen_degree.setText(mKKACManager.getCurTemp() + "");
        }
        else {
            mScreen_degree.setText("NA");//温度不可调节时，mKKACManager.getCurTemp()获取的温度值是-1
        }
    }
    
    /** 更新面板上下风向显示 */
    private void updateUDWindDirectDisplay() {
        mScreen_WindDirection_Auto.setVisibility(View.VISIBLE);
        UDWindDirectType directType = mKKACManager.getCurUDDirectType();//获取该空调支持风向类型
        switch (directType) {
            case UDDIRECT_ONLY_SWING://没有风向
                mScreen_WindDirection_Auto.setText("没有风向");
                mScreen_WindDirection.setVisibility(View.INVISIBLE);
                break;
            case UDDIRECT_ONLY_FIX://只有固定风向或不区分扫风和固定风
                mScreen_WindDirection_Auto.setText("手动风向");
                mScreen_WindDirection.setText("风向" + mKKACManager.getCurUDDirect());
                mScreen_WindDirection.setVisibility(View.VISIBLE);
                break;
            case UDDIRECT_FULL://同时具备固定风和扫
                int windDirect = mKKACManager.getCurUDDirect();
                if (windDirect == 0) {//风向为扫风
                    mScreen_WindDirection_Auto.setText("自动风向");
                    mScreen_WindDirection.setVisibility(View.INVISIBLE);
                }
                else {
                    mScreen_WindDirection_Auto.setText("手动风向");
                    mScreen_WindDirection.setText("风向" + mKKACManager.getCurUDDirect());
                    mScreen_WindDirection.setVisibility(View.VISIBLE);
                }
                break;
        }
    }
    
    /** 更新面板睡眠键显示 */
    private void updateSleepDisplay() {
        //睡眠图标处理
        if (mKKACManager.isExpandCanUse(ACConstants.FUNCTION_SLEEP)) {//睡眠键是否可用
            int state = mKKACManager.getExpandKeyState(ACConstants.FUNCTION_SLEEP);//获取睡眠键的状态
            if (state == 0) {//睡眠功能是关闭状态
                mScreen_sleep.setVisibility(View.INVISIBLE);
            }
            else {
                mScreen_sleep.setVisibility(View.VISIBLE);
                mScreen_sleep.setText("睡眠" + state);
            }
        }
        else {
            mScreen_sleep.setVisibility(View.INVISIBLE);
        }
    }
    
    /** 更新面板定时显示 */
    private void updateTimeingDisplay() {
        mKKACManager.timeingCheck();//定时自检，检索上次设置的定时是否失效
        if (mKKACManager.isTimeingCanUse()) {//定时功能是否可用
            if (mKKACManager.isTimingBeenSet()) {//如果已经设置了定时，显示图标
                mScreen_timing.setVisibility(View.VISIBLE);
                mScreen_timing.setText(mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF ? "定时开" : "定时关");
            }
            else {
                mScreen_timing.setVisibility(View.INVISIBLE);
            }
        }
        else {
            mScreen_timing.setVisibility(View.INVISIBLE);
        }
    }
    
    @Override
    public void onClick(View v) {
        
        if (mKKACManager.stateIsEmpty() || mIrData == null) {
            return;
        }
        
        if (v.getId() == R.id.ac_command_min_temp) {
            int minTemp = mKKACManager.getMinTemp();
            TipsUtil.toast(ACActivity.this, minTemp == -1 ? "当前模式下温度不能调节" : minTemp + "");
            return;
        }
        
        if (v.getId() == R.id.ac_command_max_temp) {
            int maxTemp = mKKACManager.getMaxTemp();
            TipsUtil.toast(ACActivity.this, maxTemp == -1 ? "当前模式下温度不能调节" : maxTemp + "");
            return;
        }
        
        if (v.getId() == R.id.ac_command_power) {
            mKKACManager.changePowerState();//切换开关机
            updateACDisplay();
        }
        else {//摁空调的其他键
            switch (v.getId()) {
                case R.id.ac_command_model://模式键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    mKKACManager.changeACModel();//顺序切换模式
                    updateACDisplay();
                    break;
                case R.id.ac_command_warm://制热键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    
                    if (!mKKACManager.isContainsTargetModel(ACConstants.AC_MODE_HEAT)) {//是否支持制热模式
                        TipsUtil.toast(ACActivity.this, "该空调不具备制热模式");
                        return;
                    }
                    mKKACManager.changeACTargetModel(ACConstants.AC_MODE_HEAT);//切换到制热模式
                    updateACDisplay();
                    break;
                case R.id.ac_command_cold://制冷键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    
                    if (!mKKACManager.isContainsTargetModel(ACConstants.AC_MODE_COOL)) {//是否支持制冷模式
                        TipsUtil.toast(ACActivity.this, "该空调不具备制冷模式");
                        return;
                    }
                    mKKACManager.changeACTargetModel(ACConstants.AC_MODE_COOL);//切换到制冷模式
                    updateACDisplay();
                    break;
                case R.id.ac_command_heat_up://温度+键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    if (!mKKACManager.isTempCanControl()) {//该模式下是否支持调节温度
                        TipsUtil.toast(ACActivity.this, "在该模式下温度不能调节");
                        return;
                    }
                    mKKACManager.increaseTmp();
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_heat_down://温度-键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    
                    if (!mKKACManager.isTempCanControl()) {//该模式下是否支持调节温度
                        TipsUtil.toast(ACActivity.this, "在该模式下温度不能调节");
                        return;
                    }
                    mKKACManager.decreaseTmp();
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_sweep_wind://上下扫风键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    
                    if (mKKACManager.getCurUDDirectType() == UDWindDirectType.UDDIRECT_ONLY_SWING) {//没有风向，风向不能使用
                        TipsUtil.toast(ACActivity.this, "没有风向可调节");
                        return;
                    }
                    
                    if (mKKACManager.getCurUDDirectType() == UDWindDirectType.UDDIRECT_ONLY_FIX) {//只有固定风向可用，扫风不能使用
                        TipsUtil.toast(ACActivity.this, "扫风键不可用");
                        return;
                    }
                    mKKACManager.changeUDWindDirect(UDWindDirectKey.UDDIRECT_KEY_SWING);//切换风向，对于同时具备扫风和固定的空调：如果当前是扫风，再摁扫风时，切换到了固定风向；如果当前是固定风向，再摁扫风时，切换到扫风
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_wind_direct://上下固定风向键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    if (mKKACManager.getCurUDDirectType() == UDWindDirectType.UDDIRECT_ONLY_SWING) {//没有风向，风向不能使用
                        TipsUtil.toast(ACActivity.this, "没有风向可调节");
                        return;
                    }
                    mKKACManager.changeUDWindDirect(UDWindDirectKey.UDDIRECT_KEY_FIX);//切换固定风向
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_wind_speed://风速键
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    if (!mKKACManager.isWindSpeedCanControl()) {//该模式下是否支持调节风速
                        TipsUtil.toast(ACActivity.this, "该模式下风速不能调节");
                        return;
                    }
                    mKKACManager.changeWindSpeed();//切换风速档
                    updatePanelDisplay();
                    break;
                
                case R.id.ac_command_16:
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    boolean result = mKKACManager.setTargetTemp(16);
                    if (!result) {
                        TipsUtil.toast(ACActivity.this, "该模式下温度不可调节或不在温度范围内");
                        return;
                    }
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_25:
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    boolean result2 = mKKACManager.setTargetTemp(25);
                    if (!result2) {
                        TipsUtil.toast(ACActivity.this, "该模式下温度不可调节或不在温度范围内");
                        return;
                    }
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_speed_auto://自动风量
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    boolean autoResult = mKKACManager.setTargetWindSpeed(ACConstants.AC_WIND_SPEED_AUTO);
                    if (!autoResult) {
                        TipsUtil.toast(ACActivity.this, "该模式下风量不可调节或不具备该风量");
                        return;
                    }
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_speed_low://低风量
                    if (mKKACManager.getPowerState() == ACConstants.AC_POWER_OFF) {
                        TipsUtil.toast(ACActivity.this, "关机下不能使用");
                        return;
                    }
                    boolean lowResult = mKKACManager.setTargetWindSpeed(ACConstants.AC_WIND_SPEED_LOW);
                    if (!lowResult) {
                        TipsUtil.toast(ACActivity.this, "该模式下风量不可调节或不具备该风量");
                        return;
                    }
                    updatePanelDisplay();
                    break;
                case R.id.ac_command_ud_direct://设置风量
                    UDWindDirectType directType = mKKACManager.getCurUDDirectType();
                    if (directType == UDWindDirectType.UDDIRECT_ONLY_SWING) {
                        TipsUtil.toast(ACActivity.this, "风向不可调节");
                        return;
                    }
                    mKKACManager.setTargetUDWindDirect(1);
                    TipsUtil.toast(ACActivity.this, "风向：" + mKKACManager.getCurUDDirect());
                    updatePanelDisplay();
                    break;
            }
        }
        sendIr();//这些码可以直接给ConsumerIR发送出去
    }
    
    /** 发送解析出来的红外码 */
    private void sendIr() {
        int[] patternsInArray = mKKACManager.getACIRPatternIntArray();//这些码可以直接给ConsumerIR发送出去
        Log.d("IRPattern", Arrays.toString(patternsInArray));
    }
    
    /** Activity生命周期处理 */
    @Override
    protected void onPause() {
        mKKACManager.onPause();
        super.onPause();
    }
    
    @Override
    protected void onResume() {
        mKKACManager.onResume();
        super.onResume();
    }
    
    @Override
    protected void onStop() {
        //使用 getACStateV2InString  保存当前空调的状态，测试时注释掉了
                        String acState = mKKACManager.getACStateV2InString();
                        DataStoreUtil.i().putString("AC_STATE", acState);
        super.onStop();
    }
    
    public static int[] getScreenWH() {
        DisplayMetrics dm = KookongSDK.getContext().getResources().getDisplayMetrics();
        int[] res = new int[2];
        res[0] = dm.widthPixels;
        res[1] = dm.heightPixels;
        return res;
    }
    
}
