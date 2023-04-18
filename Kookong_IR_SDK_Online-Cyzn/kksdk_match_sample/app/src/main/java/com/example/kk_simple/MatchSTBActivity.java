/**  
 * Copyright © 2017Kookong.com. All rights reserved.
 *
 * @title: MatchSTBActivity.java
 * @prject: kksdk_full_sample_single
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2017年9月20日 上午11:56:59
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
import com.kookong.app.data.RemoteList;
import com.kookong.app.data.SpList;
import com.kookong.app.data.SpList.Sp;

/**
 * @className: MatchSTBActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2017年9月20日 上午11:56:59
 */
public class MatchSTBActivity extends Activity
{
    
    private ListView mListView;
    private int mAreaId;
    
    //压缩码
    private boolean mIsZip;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match_stb);
        mListView = (ListView) findViewById(R.id.stb_list);
        Bundle bundle = getIntent().getExtras();
        if(bundle != null){
            mIsZip = bundle.getBoolean(IntentConstants.IR_IS_ZIPCODE);
        }
        httpGetAreadId("北京市", "北京市", "海淀区");//例如获取北京海淀区的AreaId
    }
    
    private void httpGetAreadId(String pro, String city, String dis) {
        //传入省市区，获取AreaId
        KookongSDK.getAreaId(pro, city, dis, new IRequestResult<Integer>()
        {
            
            @Override
            public void onSuccess(String msg, Integer result) {
                Logger.d("AreaId is : " + result);
                mAreaId = result;
                httpGetOperator(mAreaId);
                
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchSTBActivity.this, msg);
                Logger.d("httpGetAreadId faild：errorCode=" + errorCode + ",msg=" + msg);
            }
        });
    }
    
    private void httpGetOperator(int areaId) {
        //根据AreaId获取该地区下的运营商列表
        KookongSDK.getOperaters(areaId, new IRequestResult<SpList>()
        {
            
            @Override
            public void onSuccess(String msg, SpList result) {
                List<Sp> sps = result.spList;
                for (int i = 0; i < sps.size(); i++) {
                    Logger.d("The sp is " + sps.get(i).spName);
                }
                refreshList(sps);
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchSTBActivity.this, msg);
                Logger.d("httpGetOperator faild：errorCode=" + errorCode + ",msg=" + msg);
            }
        });
    }
    
    private void refreshList(final List<Sp> spList) {
        List<String> dataList = new ArrayList<String>();
        for (int i = 0; i < spList.size(); i++) {
            Sp sp = spList.get(i);
            dataList.add(sp.spName);
            
        }
        
        mListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, dataList));
        mListView.setOnItemClickListener(new OnItemClickListener()
        {
            
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Sp sp = spList.get(position);
                if (sp.type == 1) {//如果IPTV，进入IPTV品牌列表页
                    Intent intent = new Intent(MatchSTBActivity.this, MatchIPTVBrandActivity.class);
                    Bundle bundle = new Bundle();
                    bundle.putInt("sp_id", sp.spId);
                    bundle.putInt("sp_type", sp.type);
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                else {
                    httpGetRemoteIds(Device.STB, sp.spId, mAreaId, sp.spName);
                }
            }
        });
    }
    
    //获取该品牌下全部的remoteId
    private void httpGetRemoteIds(int deviceType, int spId, int areaId, final String deviceName) {
        //有线运营商通过deviceType、spId、areaId获取数据，品牌直接传0
        KookongSDK.getAllRemoteIds(deviceType, 0, spId, areaId, new IRequestResult<RemoteList>()
        {
            
            @Override
            public void onSuccess(String msg, RemoteList result) {
                List<Integer> rids = result.rids;
                if (rids != null && rids.size() > 0) {
                    String remoteIds = listToStr(rids);
                    Bundle bundle = new Bundle();
                    bundle.putInt("device_type", Device.STB);
                    bundle.putString("all_remote_id", remoteIds);
                    bundle.putString("device_name", deviceName);
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                    Intent intent = new Intent(MatchSTBActivity.this, MatchActivity.class);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                else {
                    TipsUtil.toast(MatchSTBActivity.this, "该品牌下没有remoteId，无法进行匹配");
                }
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchSTBActivity.this, msg);
                Logger.d("httpGetRemoteIds faild code=" + errorCode + ",msg=" + msg);
            }
        });
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
