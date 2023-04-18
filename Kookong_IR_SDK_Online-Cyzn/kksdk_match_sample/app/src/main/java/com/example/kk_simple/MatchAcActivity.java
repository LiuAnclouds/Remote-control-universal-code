package com.example.kk_simple;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.duokan.phone.remotecontroller.R;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.kookong.app.data.RemoteList;

public class MatchAcActivity extends Activity {
    private int brandId;
    private String brandName;
    private ListView lv;
    private RemoteList remoteList;
    public static void start(Context context,String brandName,int brandId) {
        Intent starter = new Intent(context, MatchAcActivity.class);
        starter.putExtra("brandId",brandId);
        starter.putExtra("brandName",brandName);
        context.startActivity(starter);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_match_ac);
        setTitle(brandName);
        lv= (ListView) findViewById(R.id.lv);
        brandId=getIntent().getIntExtra("brandId",-1);
        brandName =getIntent().getStringExtra("brandName");
        //alter param "countryCode" as you need
        KookongSDK.getAllRemoteIds(Device.AC, brandId, "CN", new IRequestResult<RemoteList>() {
            @Override
            public void onSuccess(String s, RemoteList remoteList) {
                MatchAcActivity.this.remoteList = remoteList;
                ArrayAdapter<String> adapter=new ArrayAdapter<>(MatchAcActivity.this,android.R.layout.simple_expandable_list_item_1);
                for (int i = 0; i < remoteList.rids.size(); i++) {
                    adapter.add("remoteId:"+remoteList.rids.get(i));
                }
                lv.setAdapter(adapter);
            }

            @Override
            public void onFail(Integer integer, String s) {

            }
        });
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                ACActivity.start(MatchAcActivity.this,remoteList.rids.get(i));
            }
        });
    }

}
