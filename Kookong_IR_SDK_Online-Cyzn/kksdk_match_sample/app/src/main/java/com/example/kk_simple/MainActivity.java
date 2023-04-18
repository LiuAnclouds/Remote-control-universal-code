package com.example.kk_simple;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.IntentConstants;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;

public class MainActivity extends Activity implements OnClickListener {
    private static String TAG = "KKSDK";
    public static final String APP_KEY = "54FE90A0F29CC7CBCBE0F8F502F53D14";

     public static final String irDeviceId = "1";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //1.在App的入口进行初始化, 在Application中初始化也可以
        //按红外设备授权收费的客户，需要传递自己设备唯一的标识，使用KookongSDK.init(Context context, String key, String irDeviceId);
        //其他客户使用KookongSDK.init(Context context, String key);
        KookongSDK.setDebugMode(true);
        boolean result = KookongSDK.init(this, APP_KEY,irDeviceId);
        Log.d(TAG, "SDK 初始化结果：" + result);
        if (result) {
            TipsUtil.toast(this, "KookongSDK初始化成功");
        } else {
            //SDK初始化失败了，后续的接口就无法正常访问
            TipsUtil.toast(this, "KookongSDK初始化失败");
        }

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.sdk_global_flow://全球非压缩对码流程演示
                //首次传全部remoteId作为mr,devicetype,switch请求测试按键，sdk把数据全部返回，记录mr
                //按照顺序可以依次测试每个按键，如果其中某个按键好用，就把这个键的信息传给接口，接口返回下一组测试键或匹配成功
                //如果这一组的按键都不好用，把这些键调用全部传给接口，接口返回下一组测试键或匹配结果
                Intent intent = new Intent(MainActivity.this, ChooseCountryActivity.class);
                intent.putExtra(IntentConstants.IR_IS_ZIPCODE, false);
                startActivity(intent);
                break;
            case R.id.sdk_gloabl_zip_flow://全球压缩对码流程
                Intent gZipintent = new Intent(MainActivity.this, ChooseCountryActivity.class);
                gZipintent.putExtra(IntentConstants.IR_IS_ZIPCODE, true);
                startActivity(gZipintent);
                break;
            case R.id.sdk_flow://大陆非压缩对码流程演示
                Intent intent2 = new Intent(MainActivity.this, MatchDeviceTypeActivity.class);
                intent2.putExtra(IntentConstants.IR_IS_ZIPCODE, false);
                startActivity(intent2);
                break;
            case R.id.sdk_zip_flow://大陆压缩对码流程演示
                Intent zipIntent = new Intent(MainActivity.this, MatchDeviceTypeActivity.class);
                zipIntent.putExtra(IntentConstants.IR_IS_ZIPCODE, true);
                startActivity(zipIntent);
                break;
            case R.id.sdk_ac:
                Intent intent3 = new Intent(MainActivity.this, ACActivity.class);
                startActivity(intent3);
                break;
            case R.id.sdk_match_lineup:
                Intent intent4 = new Intent(MainActivity.this, MatchLineupActivity.class);
                startActivity(intent4);
                break;
            case R.id.sdk_offline_xml:
                intent= new Intent(MainActivity.this, OfflineXMLACActivity.class);
                startActivity(intent);
                break;
            default:
                break;
        }

    }

}
