/**  
 * Copyright © 2017Kookong.com. All rights reserved.
 *
 * @title: MatchIPTVBrandActivity.java
 * @prject: kksdk_full_sample_single
 * @package: com.example.kk_simple
 * @description: TODO
 * @author: Administrator  
 * @date: 2017年9月20日 下午1:52:24
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
import com.kookong.app.data.StbList;
import com.kookong.app.data.StbList.Remote;
import com.kookong.app.data.StbList.Stb;

/**
 * @className: MatchIPTVBrandActivity
 * @description: TODO
 * @author: Administrator
 * @date: 2017年9月20日 下午1:52:24
 */
public class MatchIPTVBrandActivity extends Activity
{
    private ListView mListView;
    
    private int mSpId;
    private int mSpType;
    
    private boolean mIsZip; 
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match_iptv_brand);
        mListView = (ListView) findViewById(R.id.iptv_brand_list);
        Bundle bundle = getIntent().getExtras();
        if (bundle != null) {
            mSpId = bundle.getInt("sp_id");
            mSpType = bundle.getInt("sp_type");
            mIsZip = bundle.getBoolean(IntentConstants.IR_IS_ZIPCODE);
            httpGetIptvBrand(mSpId);
        }
    }
    
    private void httpGetIptvBrand(int spId) {
        KookongSDK.getIPTV(spId, new IRequestResult<StbList>()
        {
            
            @Override
            public void onSuccess(String msg, StbList result) {
                List<Stb> stbs = result.stbList;
                for (int i = 0; i < stbs.size(); i++) {
                    Logger.d("The Stb is " + stbs.get(i).bname);
                }
                refreshList(stbs);
            }
            
            @Override
            public void onFail(Integer errorCode, String msg) {
                TipsUtil.toast(MatchIPTVBrandActivity.this, msg);
                Logger.d("httpGetIptvBrand faild code=" + errorCode + ",msg=" + msg);
            }
        });
    }
    
    private void refreshList(final List<Stb> stbList) {
        List<String> dataList = new ArrayList<String>();
        for (int i = 0; i < stbList.size(); i++) {
            Stb stb = stbList.get(i);
            dataList.add(stb.bname);
            
        }
        
        mListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, dataList));
        mListView.setOnItemClickListener(new OnItemClickListener()
        {
            
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Stb stb = stbList.get(position);
                List<StbList.Remote> list = stb.remotes;
                if (list != null && list.size() > 0) {
                    String remoteIds = listToStr(list);
                    Bundle bundle = new Bundle();
                    bundle.putInt("device_type", Device.STB);
                    bundle.putString("all_remote_id", remoteIds);
                    bundle.putString("device_name", stb.bname);
                    bundle.putBoolean(IntentConstants.IR_IS_ZIPCODE, mIsZip);
                    Intent intent = new Intent(MatchIPTVBrandActivity.this, MatchActivity.class);
                    intent.putExtras(bundle);
                    startActivity(intent);
                }
            }
        });
    }
    
    private String listToStr(List<StbList.Remote> list) {
        StringBuffer stringBuffer = new StringBuffer();
        for (int i = 0; i < list.size(); i++) {
            Remote remote = list.get(i);
            stringBuffer.append(remote.rid);
            if (i < list.size() - 1) {
                stringBuffer.append(",");
            }
        }
        return stringBuffer.toString();
    }
}
