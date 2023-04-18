/**  
 * Copyright © 2017Kookong.com. All rights reserved.
 *
 * @title: DeviceTypeActivity.java
 * @prject: kksdk_full_sample_single
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2017年9月20日 上午11:55:24
 * @version: V1.2.0  
 */
package com.example.kk_simple;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.CountryUtil;
import com.example.kk_simple.util.IntentConstants;
import com.hzy.tvmao.ir.Device;

/**
 * @className: DeviceTypeActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2017年9月20日 上午11:55:24
 */
public class MatchDeviceTypeActivity extends Activity
{
    private List<Integer> mTypeList;
    
    private ListView mListView;

    private String mCountryCode;
    
    private boolean mIsGlobal;
    
  //压缩码
    private boolean mIsZip; 
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match_device);
        mListView = (ListView) findViewById(R.id.device_list);
        mCountryCode = getIntent().getStringExtra(IntentConstants.SELECT_COUNTRY_CODE);
        mIsGlobal = getIntent().getBooleanExtra(IntentConstants.FLOW_IS_GLOBAL, false);
        mIsZip = getIntent().getBooleanExtra(IntentConstants.IR_IS_ZIPCODE, false);
        initData();
    }
    
    private void initData() {
        mTypeList = new ArrayList<Integer>();
        //酷控目前支持的设备类型
        for (int i = Device.STB; i <= Device.WATER_HEATER; i++) {
            mTypeList.add(i);
        }
        
        refreshList(mTypeList,mCountryCode);
    }
    
    private void refreshList(final List<Integer> typeList,final String countryCode) {
        List<String> nameList = new ArrayList<String>();
        for (int i = 0; i < typeList.size(); i++) {
            nameList.add(getDeviceName(typeList.get(i)));
        }
        
        mListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, nameList));
        mListView.setOnItemClickListener(new OnItemClickListener()
        {
            
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                int deviceType = typeList.get(position);
                //机顶盒的流程跟其他设备的流程不一样
                if (deviceType == Device.STB && (mIsGlobal?CountryUtil.isChinaMainLand(countryCode):true)) {
                    Intent intent = new Intent(MatchDeviceTypeActivity.this, MatchSTBActivity.class);
                    Bundle bundle = new Bundle();
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                else {
                    Intent intent = new Intent(MatchDeviceTypeActivity.this, MatchBrandActivity.class);
                    Bundle bundle = new Bundle();
                    bundle.putInt("device_type", deviceType);
                    bundle.putString(IntentConstants.SELECT_COUNTRY_CODE, countryCode);
                    bundle.putBoolean(IntentConstants.FLOW_IS_GLOBAL, mIsGlobal);
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
            }
        });
    }
    
    private String getDeviceName(int type) {
        String name = "";
        switch (type) {
            case Device.STB:
                name = "机顶盒";
                break;
            case Device.TV:
                name = "电视机";
                break;
            case Device.BOX:
                name = "盒子";
                break;
            case Device.DVD:
                name = "DVD";
                break;
            case Device.AC:
                name = "空调";
                break;
            case Device.PRO:
                name = "投影仪";
                break;
            case Device.PA:
                name = "功放";
                break;
            case Device.FAN:
                name = "风扇";
                break;
            case Device.SLR:
                name = "单反";
                break;
            case Device.LIGHT:
                name = "灯泡";
                break;
            case Device.AIR_CLEANER:
                name = "空气净化器";
                break;
            case Device.WATER_HEATER:
                name = "热水器";
                break;
        }
        return name;
    }
}
