/**  
 * Copyright © 2017Kookong.com. All rights reserved.
 *
 * @title: MatchBrandActivity.java
 * @prject: kksdk_full_sample_single
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2017年9月20日 上午11:56:15
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
import com.example.kk_simple.util.IntentConstants;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.kookong.app.data.BrandList;
import com.kookong.app.data.RemoteList;
import com.kookong.app.data.BrandList.Brand;

/**
 * @className: MatchBrandActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2017年9月20日 上午11:56:15
 */
public class MatchBrandActivity extends Activity
{
    
    private ListView listView;
    private List<String> nameList = new ArrayList<String>();
    private int mDeviceType;
    private String mCountryCode;
    private boolean mIsGlobal;
    
    //压缩码
    private boolean mIsZip;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match_brand);
        listView = (ListView) findViewById(R.id.brand_list);
        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            mDeviceType = bundle.getInt("device_type");
            mCountryCode = bundle.getString(IntentConstants.SELECT_COUNTRY_CODE);
            mIsGlobal = bundle.getBoolean(IntentConstants.FLOW_IS_GLOBAL);
            mIsZip = bundle.getBoolean(IntentConstants.IR_IS_ZIPCODE);
            httpGetBrand(mDeviceType,mCountryCode);
        }
    }
    
    private void httpGetBrand(final int deviceType,String countryCode) {
        doGetBrand(deviceType, countryCode, new IRequestResult<BrandList>()
        {

            @Override
            public void onSuccess(String msg, BrandList result) {
                List<Brand> brandList = result.brandList;
                show(brandList, deviceType, result.hotCount);
            }

            @Override
            public void onFail(Integer errorCode, String msg) {
                Logger.d("httpGetBrand faild code=" + errorCode + ",msg=" + msg);
                
            }
            
        });       
        
    }
    
    private void doGetBrand(final int deviceType,String countryCode,IRequestResult<BrandList> requestResult){
        if(mIsGlobal){
            KookongSDK.getBrandListFromNet(deviceType,countryCode,requestResult);
        }else{
            KookongSDK.getBrandListFromNet(deviceType, requestResult);
        }
    }
    
    private void show(final List<Brand> brands, final int deviceType, int hotCount) {
        for (int i = 0; i < brands.size(); i++) {
            Brand brand = brands.get(i);
            if (i < hotCount) {
                nameList.add(brand.cname + "-" + "常用");//前hotCount个品牌是最常用品牌
                continue;
            }
            nameList.add(brand.cname);
        }
        
        listView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, nameList));
        
        listView.setOnItemClickListener(new OnItemClickListener()
        {
            
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                //                Intent intent = new Intent(parent.getContext(), MatchActivity.class);
                //                intent.putExtra("deviceType", deviceType);
                //                intent.putExtra("brand", brands.get(position));
                //                startActivity(intent);
                Brand brand = brands.get(position);
                if (deviceType == Device.AC) {
                    TipsUtil.toast(MatchBrandActivity.this, "空调不走单键对码的流程");
                    MatchAcActivity.start(MatchBrandActivity.this,brand.ename,brand.brandId);
                }
                else {
                    httpGetRemoteIds(deviceType, brand.brandId, brand.cname,mCountryCode);
                }
            }
        });
    }
    
    //获取该品牌下全部的remoteId
    private void httpGetRemoteIds(final int deviceType, int brandId, final String deviceName,String countryCode) {
        //有线运营商通过deviceType、spId、areaId获取数据，品牌直接传0
        doGetAllRemoteIds(deviceType, brandId, deviceName, countryCode, new IRequestResult<RemoteList>()
        {

            @Override
            public void onSuccess(String msg, RemoteList result) {
                List<Integer> rids = result.rids;
                if (rids != null && rids.size() > 0) {
                    String remoteIds = listToStr(rids);
                    Bundle bundle = new Bundle();
                    bundle.putInt("device_type", deviceType);
                    bundle.putString("all_remote_id", remoteIds);
                    bundle.putString("device_name", deviceName);
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE,mIsZip);
                    Intent intent = new Intent(MatchBrandActivity.this, MatchActivity.class);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                else {
                    TipsUtil.toast(MatchBrandActivity.this, "该品牌下没有remoteId，无法进行匹配");
                }
                
            }

            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchBrandActivity.this, msg);
                Logger.d("httpGetRemoteIds faild code=" + errorCode + ",msg=" + msg);
            }
            
        });
      
    }
    
    private void doGetAllRemoteIds(final int deviceType, int brandId, final String deviceName,String countryCode,IRequestResult<RemoteList> requestResult){
        if(mIsGlobal){
            KookongSDK.getAllRemoteIds(deviceType,brandId,countryCode,requestResult);
        }else{
            KookongSDK.getAllRemoteIds(deviceType, brandId, 0, 0, requestResult);
        }
    }
    
    private String listToStr(List<Integer> list) {
        StringBuffer stringBuffer = new StringBuffer();
        for (int i = 0; i < list.size(); i++) {
            stringBuffer.append(list.get(i));
            if (i < list.size() - 1) {
                stringBuffer.append(",");
            }
        }
        return stringBuffer.toString();
    }
    
}
