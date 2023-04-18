package com.example.kk_simple;

import android.app.Activity;
import android.os.Bundle;

import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.IrUtil;
import com.example.kk_simple.util.Logger;
import com.example.kk_simple.util.StringUtil;
import com.hzy.tvmao.BaseACManager;
import com.hzy.tvmao.KKACManagerV2;
import com.hzy.tvmao.KKACZipManagerV2;
import com.kookong.app.data.IrData;

import java.util.ArrayList;
import java.util.HashMap;
/**
 *  此类是展示如何使用SDK载入离线xml数据，实现空调解码功能
 *
 *  酷控会提供离线xml数据给客户云端，而手机端访问的数据是客户云端返回，
 *  如不清楚离线xml字段跟云端接口字段的映射关系，请咨询贵方云端
 *
 *  IrData为酷控在线SDK数据格式，初始化空调解码工具类需要此数据
 *   此demo展示了如何由离线xml数据创建出IrData，完成空调解码
 */

public class OfflineXMLACActivity extends Activity {
    //发码工具类 实现了基本的发码逻辑 包含酷控红外码格式处理 可供参考
    private IrUtil irUtil = new IrUtil(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_offline_xml);
        //压缩方案 还是非压缩方案 压缩方案用KKACZipManagerV2 , 非压缩方案用KKACManagerV2
        boolean isZip = false;
        BaseACManager baseACManager = isZip ? new KKACZipManagerV2() : new KKACManagerV2();
        //该方法展示了如何将离线xml的字段转化为IrData类，IrData类请看下方绿色部分注释
        IrData irData = buildIrData();
        //将xml的字段转化为IrData类后 完成KKACManagerV2的初始化
        baseACManager.initIRData(irData);
        //设置之前保存的空调状态，空调状态用baseACManager.getACStateV2InString()方法获取 第一次运行需要设置一个空状态
        baseACManager.setACStateV2FromString("");
        //改变空调状态 具体方法请查阅文档
        //开机
        baseACManager.changePowerState();
        //调整模式
        baseACManager.changeACModel();
        //其他空调按键的调用方式请查阅文档，搜索KKACManager，在此不一一列举

        if (isZip) {
            //acParams为遥控器参数 keyParams为按键参数
            //压缩方案下，手机端适时将以上两个参数传输给设备端使用
            KKACZipManagerV2 kkacZipManagerV2 = (KKACZipManagerV2) baseACManager;
            //TODO 遥控器参数需要传输给嵌入式设备保存并且在使用的时候传给设备SDK的create方法
            int remoteId = kkacZipManagerV2.getRemoteId();
            byte[] acParams = kkacZipManagerV2.getAcParams();
            Logger.e("remote(" + remoteId + ") params is " + StringUtil.byte2Hexstr(acParams));
            //TODO 这些数据需要传输给嵌入式设备，调用设备SDK的enc方法获得波形数据发送红外
            byte[] keyParams = kkacZipManagerV2.getACKeyIr();
            Logger.e("remote(" + remoteId + ") key ir data is " + StringUtil.byte2Hexstr(keyParams));

        } else {
            KKACManagerV2 kkacManagerV2 = (KKACManagerV2) baseACManager;
            int frequence = irData.fre;
            String pulse = kkacManagerV2.getACIRPattern();
            //发码
            irUtil.sendIr(frequence, pulse);
        }

    }

    //以下为IrData类数据结构 对应 IrRemoteController.xml > remote_controller
    /**
     //此类对应 IrRemoteController.xml > remote_controller
    public class IrData implements SerializableEx {
        private static final long serialVersionUID = 3228680024489450540L;
        public int rid;//对应  id
        public int fre;//对应  frequency
        public short type;//对应  type
        public HashMap<Integer, String> exts;//对应  exts
        public ArrayList<com.example.kk_simple.util.IrData.IrKey> keys; // 对应keys
        public String extJSON = "";//无对应字段

        public IrData() {
        }
        //此类对应 IrRemoteController.xml > remote_controller > key
        public  class IrKey implements SerializableEx {
            private static final long serialVersionUID = -1231310702792778492L;
            public int fid; //对应 key > id
            public String fkey; //无对应字段
            public String fname; //对应 IrKey.xml > key > name或display_name , name为英文名,display_name为中文名
            public int format;//无对应字段
            public String scode;//无对应字段
            public String dcode;//无对应字段
            public String pulse;//对应 key > pulse
            public HashMap<Integer, String> exts; //对应 key > exts

            public IrKey() {
            }
        }
    }
     **/
    //该方法展示了如何将离线xml的字段转化为IrData类
    private IrData buildIrData() {
        IrData mIrData = new IrData();

        mIrData.rid = 2607;// IrRemoteController.xml > remote_controller > id
        mIrData.fre = 38000;// IrRemoteController.xml > remote_controller > frequence
        mIrData.type = 2;// IrRemoteController.xml > remote_controller > type
        mIrData.exts = new HashMap<Integer, String>();// IrRemoteController.xml > remote_controller > exts
        mIrData.keys = new ArrayList<IrData.IrKey>();// IrRemoteController.xml > remote_controller > keys

        //装载定死的数据 模拟离线xml的exts字段
        putFakeExts(mIrData.exts);
        //创建扩展键假数据 模拟离线xml的keys字段
        createFakeKeys(mIrData.keys);
        return mIrData;
    }
    //该方法展示了如何将离线xml的IrRemoteController.xml > remote_controller > keys字段转化为IrKey类
    private IrData.IrKey createIrKey(int fid, String fkey, String fName, HashMap<Integer, String> exts) {
        IrData.IrKey irKey = new IrData.IrKey();
        irKey.fid = fid;// IrRemoteController.xml > remote_controller > key > id
        irKey.fkey = fkey;//无对应 非必需字段
        irKey.fname = fName;//IrKey.xml  > key > name 或 display_name ，name为英文名,display_name为中文名
        irKey.exts = exts;// IrRemoteController.xml > remote_controller > key > exts
        return irKey;
    }

    private void createFakeKeys(ArrayList<IrData.IrKey> keys) {
        HashMap<Integer, String> emptyExts = new HashMap<Integer, String>();
        HashMap<Integer, String> onExts = new HashMap<Integer, String>();
        onExts.put(300, "9000,4500");
        onExts.put(301, "645,541");
        onExts.put(302, "645,1680");
        onExts.put(303, "4&645,20700|8&645,40700,8930,4500|13&645,20700");
        onExts.put(305, "482");
        onExts.put(306, "1");
        onExts.put(1509, "4&3|13&3");
        keys.add(createIrKey(8, "supper_power", "超强", emptyExts));
        keys.add(createIrKey(9, "timing_on", "定时开", onExts));
        keys.add(createIrKey(10, "timing_off", "定时关", emptyExts));
        keys.add(createIrKey(11, "light", "灯光", emptyExts));
        keys.add(createIrKey(12, "aux_heat", "辅热", emptyExts));
        keys.add(createIrKey(13, "health", "健康", emptyExts));
        keys.add(createIrKey(14, "air_change", "换气", emptyExts));
        keys.add(createIrKey(15, "dry", "干燥", emptyExts));
        keys.add(createIrKey(16, "show_temp", "温度显示", emptyExts));
        keys.add(createIrKey(21, "lr_wind_mode", "左右风向", emptyExts));
        keys.add(createIrKey(22, "sleep", "睡眠", emptyExts));
    }

    private void putFakeExts(HashMap<Integer, String> exts) {
        exts.put(300, "9000,4500");
        exts.put(301, "645,541");
        exts.put(302, "645,1680");
        exts.put(303, "4&645,20700");
        exts.put(305, "197");
        exts.put(306, "1");
        exts.put(1001, "00020001");
        exts.put(1002, "09490020500211200060");
        exts.put(1003, "020101");
        exts.put(1008, "0805100C010305070A");
        exts.put(1012, "0305080103050804030508000305080303050802");
        exts.put(1013, "03020400030204010302040203020403");
        exts.put(1015, "060102012C3001060102002C3002060102002C3003060102002C3004060102002C3005060102002C3006060102012C3007060102012C3009060102012C300B060102002C3000");
        exts.put(1017, "050D00111200@050D01111201@050E001F2000@050E011F2001@051501282C01@051502282C0D@051503282C00@051504282C02@051505282C03@051506282C04@051507282C05@051508282C06@051509282C0C@050F00101100@050F01101101@050C00101101@050C01101100@051000343800@051001343801@051002343802@051003343803@050800131400@050801131401@050B00121300@050B01121301@051600000100@051601000101");
        exts.put(1503, "T");
        exts.put(1506, "0,1,2,3,4,5,6,7,8,9");
        exts.put(1509, "4&3");
        exts.put(1515, "13|0-1|0|CHAFD@14|0-1|0|CHAFD@21|1-9|0|CHAFD@15|0-1|0|CD@12|1,0-1|0|H@16|0-3|0|CHAFD@08|0-1|0|CH@11|1,0-1|2|CHAFD@22|0-1|0|CHD@09|0-1440,30|1|CHAFD@10|0-1440,30|0|CHAFD");
        exts.put(1516, "10@30-1440,30&9|9@30-1440,30&9");
        exts.put(1517, "2@22&1-1*0");
        exts.put(1518, "if(exts~=nil) then" + "\r\n" + "timing_on=exts[9];" + "\r\n" + "timing_off=exts[10];" + "\r\n" + "bytes1={};" + "\r\n" + "for i=1,9,1 do bytes1[i]=bytes[i];bytes1[i+9]=bytes[i];end" + "\r\n" + "bytes1[13]=0x60;" + "\r\n" + "if(((timing_on==0)or(timing_on==nil))and((timing_off==0) or(timing_off==nil)))then return; end" + "\r\n" + "if(timing_on~=nil)and(timing_on>0) then" + "\r\n" + "if(timing_on<600) then" + "\r\n" + "if(timing_on%60==0) then" + "\r\n" + "bytes1[2]=bytes1[2]&0x0F+0x80;" + "\r\n" + "else bytes1[2]=bytes1[2]&0x0F+0x90;" + "\r\n" + "end" + "\r\n" + "elseif(timing_on>=600)and(timing_on<1200) then" + "\r\n" + "if(timing_on%60==0) then" + "\r\n" + "bytes1[2]=bytes[2]&0x0F+0xA0;" + "\r\n" + "else bytes1[2]=bytes1[2]&0x0F+0xB0; " + "\r\n" + "end" + "\r\n" + "elseif(timing_on>=1200) then " + "\r\n" + "if(timing_on%60==0) then " + "\r\n" + "bytes[2]=bytes1[2]&0x0F+0xC0;" + "\r\n" + "else bytes1[2]=bytes1[2]&0x0F+0xD0;" + "\r\n" + "end " + "\r\n" + "end" + "\r\n" + "bytes1[3]=bytes1[3]+math.floor((timing_on/60)%10);" + "\r\n" + "bytes1[9]=((bytes1[1]&0x0F)+(bytes1[2]&0x0F)+(bytes1[3]&0x0F)+(bytes1[4]&0x0F)+(bytes1[6]>>4)+0x0C)<<4;" + "\r\n" + "bytes1[11]=bytes1[2];" + "\r\n" + "bytes1[12]=bytes1[3];" + "\r\n" + "bytes1[15]=timing_on&0xFF;" + "\r\n" + "bytes1[16]=(timing_on>>8)&0x0F;" + "\r\n" + "bytes1[17]=0;" + "\r\n" + "bytes1[18]=(((bytes1[9]>>4)+(bytes1[15]>>4)+(bytes1[16]>>4)+(bytes1[17]>>4)+0x0D)<<4)+0x02;" + "\r\n" + "elseif(timing_off~=nil)and(timing_off>0)then" + "\r\n" + "if(timing_off<600) then " + "\r\n" + "if(timing_off%60==0) then " + "\r\n" + "bytes1[2]=(bytes1[2]&0x0F)+0x80; " + "\r\n" + "else" + "\r\n" + "bytes1[2]=(bytes1[2]&0x0F)+0x90;" + "\r\n" + "end " + "\r\n" + " elseif(timing_off>=600)and(timing_off<1200) then " + "\r\n" + "if(timing_off%60==0) then " + "\r\n" + "bytes1[2]=(bytes1[2]&0x0F)+0xA0; " + "\r\n" + "else bytes1[2]=(bytes1[2]&0x0F)+0xB0;" + "\r\n" + "end " + "\r\n" + "elseif(timing_off)>=1200 then " + "\r\n" + "if(timing_off%60==0) then " + "\r\n" + "bytes1[2]=(bytes1[2]&0x0F)+0xC0;" + "\r\n" + "else bytes1[2]=(bytes1[2]&0x0F)+0xD0;" + "\r\n" + "end " + "\r\n" + "end" + "\r\n" + "bytes1[3]=bytes1[3]+math.floor((timing_off/60)%10);" + "\r\n" + "bytes1[9]=((bytes1[1]&0x0F)+(bytes1[2]&0x0F)+(bytes1[3]&0x0F)+(bytes1[4]&0x0F)+(bytes1[6]>>4)+0x0C)<<4;" + "\r\n" + "bytes1[11]=bytes1[2];" + "\r\n" + "bytes1[12]=bytes1[3];" + "\r\n" + "bytes1[16]=(timing_off&0x0F)<<4;" + "\r\n" + "bytes1[17]=timing_off>>4;" + "\r\n" + "bytes1[15]=0;" + "\r\n" + "    bytes1[18]=(((bytes1[9]>>4)+(bytes1[15]>>4)+(bytes1[16]>>4)+(bytes1[17]>>4)+0x0D)<<4)+0x01;" + "\r\n" + "end" + "\r\n" + "bytes=bytes1;" + "\r\n" + "end");
    }




}