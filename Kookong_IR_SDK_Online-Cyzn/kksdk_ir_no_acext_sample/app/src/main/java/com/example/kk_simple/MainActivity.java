package com.example.kk_simple;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.View.OnClickListener;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.hzy.tvmao.ir.Device;
import com.hzy.tvmao.utils.LogUtil;
import com.kookong.app.data.AppConst;
import com.kookong.app.data.BrandList;
import com.kookong.app.data.BrandList.Brand;
import com.kookong.app.data.IrData;
import com.kookong.app.data.IrData.IrKey;
import com.kookong.app.data.IrDataList;
import com.kookong.app.data.RemoteList;
import com.kookong.app.data.SpList;
import com.kookong.app.data.SpList.Sp;
import com.kookong.app.data.StbList;
import com.kookong.app.data.StbList.Stb;

public class MainActivity extends Activity implements OnClickListener
{

    public static final String APP_KEY = "54FE90A0F29CC7CBCBE0F8F502F53D14";
    public static final String irDeviceId = "1";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //1.在App的入口进行初始化, 在Application中初始化也可以
        //按红外设备授权收费的客户，需要传递自己设备唯一的标识，使用KookongSDK.init(Context context, String key, String irDeviceId);
        //其他客户使用KookongSDK.init(Context context, String key);
        boolean result = KookongSDK.init(this, APP_KEY, irDeviceId);
        //irDeviceId是必须传入的，初始化之后sdk首次上传酷控服务器，酷控后台服务器会自动记录，总设备数量自动减一，
        //此irDeviceId一般用红外设备的mac地址或者sn号之类的的唯一值做标识，这样也方便排产。
        LogUtil.d("Verify result is " + result);
        KookongSDK.setDebugMode(true);
    }

    /*
     * 服务器返回错误代码含义
     * 代码     			意义
     * 0,"code 1"	secret 错误
     * 0,"code 2"	secret 不存在
     * 0,"code 3"	客户已被禁用
     * 0,"code 4"	客户已超期
     * 0,"code 6"	选择运营商超出数量限制
     * 0,"code 7"	无访问权限
     * 0,"code 8"	试用数超出限制
     * 0,"code 9"	设备数超出限制
     * 0,"code 10"	单台设备下载红外超出限制
     */
    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            //----------------------------设置流部分------------------
            case R.id.setupflow_get_areaid:
                //获取指定城市的AreaId
                KookongSDK.getAreaId("北京市", "北京市", "海淀区", new IRequestResult<Integer>()
                {

                    @Override
                    public void onSuccess(String msg, Integer result) {
                        Logger.d("AreaId is : " + result);
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_get_operators:
                //获取指定AreaId下的运营商列表
                KookongSDK.getOperaters(110108, new IRequestResult<SpList>()
                {

                    @Override
                    public void onSuccess(String msg, SpList result) {
                        List<Sp> sps = result.spList;
                        for (int i = 0; i < sps.size(); i++) {
                            Logger.d("The sp is " + sps.get(i).spName);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_get_stb_remotes:
                //获取指定运营商, 指定AreaId下的遥控器id brandid传0
                KookongSDK.getAllRemoteIds(Device.STB, 0, 262, 110108, new IRequestResult<RemoteList>()
                {

                    @Override
                    public void onSuccess(String msg, RemoteList result) {
                        List<Integer> remoteids = result.rids;
                        String res = Arrays.toString(remoteids.toArray());
                        Logger.d("remoteids: " + res);
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_test_remotedata://获取的是某套红外码部分按键，仅限按红外设备授权的客户（初始化时需要传递的deviceId），如果初始化时没有传deviceId，请求接口时会提示参数错误
                //对码流程获取rid = 4162 的 红外码, 批量获取红外码的方式是逗号隔开
                KookongSDK.testIRDataById("4162", Device.STB, new IRequestResult<IrDataList>()
                {

                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        for (int i = 0; i < irDatas.size(); i++) {
                            Logger.d("The rid is " + irDatas.get(i).rid);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        //按红外设备授权的客户，才会用到这个指
                        if (errorCode == AppConst.CUSTOMER_DEVICE_NUM_LIMIT) {//设备总数超过了授权的额度
                            msg = "设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_get_remotedata://这是获取完整红外码的方法
                //获取rid = 4162 的 红外码, 批量获取红外码的方式是逗号隔开
                KookongSDK.getIRDataById("4162", Device.STB, new IRequestResult<IrDataList>()
                {

                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        for (int i = 0; i < irDatas.size(); i++) {
                            Logger.d("The rid is " + irDatas.get(i).rid);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {

                        //按红外设备授权的客户，才会用到这两个值
                        if(errorCode==AppConst.CUSTOMER_DEVICE_REMOTE_NUM_LIMIT){//同一个设备下载遥控器超过了50套限制
                            msg = "下载的遥控器超过了套数限制";
                        }else if(errorCode==AppConst.CUSTOMER_DEVICE_NUM_LIMIT){//设备总数超过了授权的额度
                            msg="设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_get_remotedata_allparse://特殊客户调用的下载空调红外码（有状态的空调，IrData.type=2）
                //获取rid = 2607 的 红外码, 批量获取红外码的方式是逗号隔开
                KookongSDK.getNoStateIRDataById("2607", Device.AC,new IRequestResult<IrDataList>()
                {

                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        for (int i = 0; i < irDatas.size(); i++) {
                            IrData irData = irDatas.get(i);
                            Logger.d("空调："+irData.rid);
                            //空调支持的模式、温度、风速
                            HashMap<Integer, String> exts = irData.exts;
                            try {
                                JSONArray ja = new JSONArray(exts.get(0));
                                //遍历模式，模式顺序：制冷、制热、自动、送风、除湿

                                //制冷模式
                                acMode(ja.getJSONObject(0), "制冷模式");
                                //制热模式
                                acMode(ja.getJSONObject(1), "制热模式");
                                //自动模式
                                acMode(ja.getJSONObject(2), "自动模式");
                                //送风模式
                                acMode(ja.getJSONObject(3), "送风模式");
                                //除湿模式
                                acMode(ja.getJSONObject(4), "除湿模式");
                            }
                            catch (JSONException e) {
                                e.printStackTrace();
                            }

                            //空调的组合按键
                            ArrayList<IrKey> keyList = irData.keys;
                            String keySize = irData.rid+"的组合按键个数："+(keyList==null?"0":keyList.size())+"\n";
                            Logger.d(keySize);
                            if(keyList != null){
                                IrKey irKey = keyList.get(0);
                                Logger.d("红外时间码："+irKey.fkey+"="+irKey.pulse);
                                IrKey irKey1 = keyList.get(1);
                                Logger.d("红外时间码："+irKey1.fkey+"="+irKey1.pulse);
                                IrKey irKey2 = keyList.get(keyList.size()-1);
                                Logger.d("红外时间码："+irKey2.fkey+"="+irKey2.pulse);
                            }
                        }

                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        //按红外设备授权的客户，才会用到这两个值
                        if(errorCode==AppConst.CUSTOMER_DEVICE_REMOTE_NUM_LIMIT){//同一个设备下载遥控器超过了50套限制
                            msg = "下载的遥控器超过了套数限制";
                        }else if(errorCode==AppConst.CUSTOMER_DEVICE_NUM_LIMIT){//设备总数超过了授权的额度
                            msg="设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(MainActivity.this, msg);

                    }

                });
                break;
            case R.id.setupflow_get_remotedata_allparse_test://特殊客户调用的下载空调红外码（对码接口）（有状态的空调，IrData.type=2）
                //获取rid = 2607 的 红外码, 批量获取红外码的方式是逗号隔开
                KookongSDK.getTestNoStateIRDataById("2607", Device.AC, false,new IRequestResult<IrDataList>()
                {

                    @Override
                    public void onSuccess(String msg, IrDataList result) {
                        List<IrData> irDatas = result.getIrDataList();
                        for (int i = 0; i < irDatas.size(); i++) {
                            IrData irData = irDatas.get(i);
                            Logger.d("空调："+irData.rid);
                            //空调支持的模式、温度、风速
                            HashMap<Integer, String> exts = irData.exts;
                            try {
                                JSONArray ja = new JSONArray(exts.get(0));
                                //遍历模式，模式顺序：制冷、制热、自动、送风、除湿

                                //制冷模式
                                acMode(ja.getJSONObject(0), "制冷模式");
                                //制热模式
                                acMode(ja.getJSONObject(1), "制热模式");
                                //自动模式
                                acMode(ja.getJSONObject(2), "自动模式");
                                //送风模式
                                acMode(ja.getJSONObject(3), "送风模式");
                                //除湿模式
                                acMode(ja.getJSONObject(4), "除湿模式");
                            }
                            catch (JSONException e) {
                                e.printStackTrace();
                            }

                            //空调的组合按键
                            ArrayList<IrKey> keyList = irData.keys;
                            String keySize = irData.rid+"的组合按键个数："+(keyList==null?"0":keyList.size())+"\n";
                            Logger.d(keySize);
                            if(keyList != null){
                                IrKey irKey = keyList.get(0);
                                Logger.d("红外时间码："+irKey.fkey+"="+irKey.pulse);
                                IrKey irKey1 = keyList.get(1);
                                Logger.d("红外时间码："+irKey1.fkey+"="+irKey1.pulse);
                                IrKey irKey2 = keyList.get(keyList.size()-1);
                                Logger.d("红外时间码："+irKey2.fkey+"="+irKey2.pulse);
                            }
                        }

                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        //按红外设备授权的客户，才会用到这两个值
                        if(errorCode==AppConst.CUSTOMER_DEVICE_REMOTE_NUM_LIMIT){//同一个设备下载遥控器超过了50套限制
                            msg = "下载的遥控器超过了套数限制";
                        }else if(errorCode==AppConst.CUSTOMER_DEVICE_NUM_LIMIT){//设备总数超过了授权的额度
                            msg="设备总数超过了授权的额度";
                        }
                        TipsUtil.toast(MainActivity.this, msg);

                    }

                });
                break;
            case R.id.setupflow_get_iptv:
                //根据spid 获取IPTV列表
                KookongSDK.getIPTV(267, new IRequestResult<StbList>()
                {

                    @Override
                    public void onSuccess(String msg, StbList result) {
                        List<Stb> stbs = result.stbList;
                        for (int i = 0; i < stbs.size(); i++) {
                            Logger.d("The Stb is " + stbs.get(i).bname);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });

                break;
            case R.id.setupflow_searchstb:
                //关键词是为 "华" 的 机顶盒列表
                KookongSDK.searchSTB("华", 110108, new IRequestResult<StbList>()
                {

                    @Override
                    public void onSuccess(String msg, StbList result) {
                        List<Stb> stbs = result.stbList;
                        for (int i = 0; i < stbs.size(); i++) {
                            Logger.d("The Stb is " + stbs.get(i).bname);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.setupflow_get_brandList:
                // 获取电视机, 空调等, (除STB以外) 的设备品牌列表
                /*
                 * public int STB = 1; //机顶盒
                 * public int TV  = 2; //电视
                 * public int BOX = 3; //网络盒子
                 * public int DVD = 4; //DVD
                 * public int AC  = 5; //空调
                 * public int PRO = 6; //投影仪
                 * public int PA  = 7; //功放
                 * public int FAN = 8; //风扇
                 * public int SLR = 9; //单反相机
                 * public int Light = 10; //开关灯泡
                 * public int AIR_CLEANER = 11;// 空气净化器
                 * public int WATER_HEATER = 12;// 热水器
                 */
                KookongSDK.getBrandListFromNet(Device.TV, new IRequestResult<BrandList>()
                {

                    @Override
                    public void onSuccess(String msg, BrandList result) {
                        List<Brand> stbs = result.brandList;
                        for (int i = 0; i < stbs.size(); i++) {
                            Logger.d("The Brand is " + stbs.get(i).cname);
                        }
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;

            case R.id.setupflow_get_ir_by_brand:

                //指定brand下的电视机所有的红外码的id spid和areaid都传0
                //获取指定设备类型(机顶盒是spId和areaid,所以这里不是机顶盒的获取方
                //式，机顶盒的红外码都是按区域划分和品牌关系不大),指定品牌下的红外码
                KookongSDK.getAllRemoteIds(Device.TV, 967, 0, 0, new IRequestResult<RemoteList>()
                {

                    @Override
                    public void onSuccess(String msg, RemoteList result) {
                        List<Integer> remoteids = result.rids;
                        String res = Arrays.toString(remoteids.toArray());
                        Logger.d("tv remoteids: " + res);
                    }

                    @Override
                    public void onFail(Integer errorCode, String msg) {
                        TipsUtil.toast(MainActivity.this, msg);

                    }
                });
                break;
            case R.id.ir_openac:
                Intent intent = new Intent(MainActivity.this, ACActivity.class);
                startActivity(intent);
                break;
            case R.id.ir_openother:
                Intent intent2 = new Intent(MainActivity.this,NonAcActivity.class);
                startActivity(intent2);
                break;
            default:
                break;
        }

    }

    private void acMode(JSONObject jo,String mode) throws JSONException{
        String speed = jo.optString("speed");

        String temperature = jo.optString("temperature");

        if(TextUtils.isEmpty(speed) && TextUtils.isEmpty(temperature)){
            Logger.d("不具备"+mode);
        }else{
            Logger.d(mode+"支持的可调风速："+speed+"，支持的可调温度："+temperature);
        }

    }
}
