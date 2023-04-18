/**  
 * Copyright © 2016Kookong.com. All rights reserved.
 *
 * @title: ChooseCountryActivity.java
 * @prject: kksdk_ir_global_sample
 * @package: com.example.kk_global
 * @description: TODO
 * @author: Administrator  
 * @date: 2016年9月22日 下午4:47:26
 * @version: V1.2.0  
 */
package com.example.kk_simple;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.IntentConstants;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.utils.LogUtil;
import com.kookong.app.data.CountryList;
import com.kookong.app.data.CountryList.Country;

/**
 * @className: ChooseCountryActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2016年9月22日 下午4:47:26
 */
public class ChooseCountryActivity extends Activity implements OnClickListener
{
    private ListView mListView;
    private EditText mEdit;
    private List<Country> mCountryList;
    
  //压缩码
    private boolean mIsZip;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_country);
        mListView = (ListView) findViewById(R.id.list_country);
        mEdit = (EditText) findViewById(R.id.search);
        mIsZip = getIntent().getBooleanExtra(IntentConstants.IR_IS_ZIPCODE, false);
        initData();
    }
    
    private void initData() {
        KookongSDK.getCountryList(new IRequestResult<CountryList>()
        {
            
            @Override
            public void onSuccess(String msg, CountryList result) {
                mCountryList = result.countryList;
                refreshList(result.countryList);
                
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                Logger.d("获取国家列表失败：" + msg);
                
            }
        });
    }
    
    private void refreshList(final List<Country> countries) {
        if (countries == null || countries.size() == 0) {
            TipsUtil.toast(ChooseCountryActivity.this, "国家列表是空");
            Logger.d("国家列表是空");
            return;
        }
        List<String> nameList = new ArrayList<String>();
        for (Country country : countries) {
            nameList.add(country.countryName + "-" + country.countryCode);
        }
        mListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, nameList));
        
        mListView.setOnItemClickListener(new OnItemClickListener()
        {
            
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String countryCode = countries.get(position).countryCode;
                String country = countries.get(position).countryName;
                LogUtil.d("选择的国家：" + country + " , " + countryCode);
                Intent intent = new Intent(ChooseCountryActivity.this, MatchDeviceTypeActivity.class);
                intent.putExtra(IntentConstants.SELECT_COUNTRY_CODE, countryCode);
                intent.putExtra(IntentConstants.FLOW_IS_GLOBAL, true);
                intent.putExtra(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                startActivity(intent);
            }
        });
        
    }
    
    @Override
    public void onClick(View v) {
        String search = mEdit.getText().toString();
        for (int i = 0; i < mCountryList.size(); i++) {
            Country country = mCountryList.get(i);
            String countryName = country.countryName;
            if (countryName.toLowerCase().contains(search.toLowerCase()) || country.countryCode.toLowerCase().equals(search.toLowerCase())) {
                mListView.setSelection(i);
                break;
            }
        }
        
    }
    
}
