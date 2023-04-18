package com.example.kk_simple.adapter;

import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.hzy.tvmao.KKACManagerV2;
import com.kookong.app.data.IrData;

import java.util.ArrayList;
import java.util.List;

public class AcExtPadAdapter extends BaseAdapter {
    private List<IrData.IrKey> irKeyList=new ArrayList<>();
    private KKACManagerV2 kkacManagerV2;

    public void setList(List<IrData.IrKey> irKeyList, KKACManagerV2 kkacManagerV2) {
        this.irKeyList = irKeyList;
        this.kkacManagerV2 = kkacManagerV2;
        notifyDataSetChanged();
    }


    @Override
    public int getCount() {
        return irKeyList.size();
    }

    @Override
    public Object getItem(int position) {
        return irKeyList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        TextView tv;
        if (null == convertView) {
            convertView = tv = new TextView(parent.getContext());
            float desity = parent.getResources().getDisplayMetrics().density;
            tv.setHeight((int) (desity * 60));
            tv.setGravity(Gravity.CENTER);
        } else {
            tv = (TextView) convertView;
        }
        IrData.IrKey irKey = irKeyList.get(position);
        int state = kkacManagerV2.getExpandKeyState(irKey.fid);
        tv.setText(irKey.fname + ":" +state);
        boolean isExtKeyCanUse = kkacManagerV2.isExpandCanUse(irKey.fid);
        tv.setEnabled(isExtKeyCanUse);
        tv.setBackgroundColor(isExtKeyCanUse ? Color.WHITE : Color.GRAY);
        return convertView;
    }
}
