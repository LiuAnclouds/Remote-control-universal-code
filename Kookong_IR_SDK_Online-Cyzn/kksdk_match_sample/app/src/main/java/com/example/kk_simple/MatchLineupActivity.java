package com.example.kk_simple;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.TextView;
import com.duokan.phone.remotecontroller.R;
import com.example.kk_simple.util.TipsUtil;
import com.hzy.tvmao.KookongSDK;
import com.hzy.tvmao.interf.IRequestResult;
import com.kookong.sdk.bean.Channel;
import com.kookong.sdk.bean.ManualMatchLineupData;

import java.util.List;

/**
 * Created by gaoshuai on 2017/10/23.
 */
public class MatchLineupActivity extends Activity {
    private TextView mTextView;
    private GridView mGridView;
    private int mAreaid, mSpid;

    private boolean canContinue;
    private StringBuilder mMrBuilder = new StringBuilder();
    private GridAdapter mGridAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_match_lineup);

        mTextView = (TextView) findViewById(R.id.sdk_manual_match_view);
        mGridView = (GridView) findViewById(R.id.sdk_manual_match_gridview);

        mGridAdapter = new GridAdapter();
        mGridView.setAdapter(mGridAdapter);


        mAreaid = 370212;
        mSpid = 17;
        //首次传null
        manualMatchLineup(mAreaid, mSpid, null);
    }

    private class GridAdapter extends BaseAdapter {
        private List<Channel> channels;

        public void setChannels(List<Channel> channels) {
            this.channels = channels;
            notifyDataSetChanged();
        }

        @Override
        public int getCount() {
            return channels == null ? 0 : channels.size();
        }

        @Override
        public Object getItem(int position) {
            return position;
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            final Channel channel = channels.get(position);
            TextView textView = new TextView(MatchLineupActivity.this);
            textView.setGravity(Gravity.CENTER);
            textView.setBackgroundResource(R.drawable.btn);
            textView.setText(channel.getName());
            if (canContinue) {
                mTextView.setText("当前测试频道:" + channel.getNum());
            }
            textView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (canContinue) {
                        mMrBuilder.append(String.format("%s_%s,", channel.getNum(), channel.getCid()));
                        String mr = mMrBuilder.substring(0, mMrBuilder.length());
                        manualMatchLineup(mAreaid, mSpid, mr);
                    } else {
                        TipsUtil.toast(MatchLineupActivity.this, "不能继续测试了, 请返回重试");
                    }

                }
            });
            return textView;
        }
    }


    private void manualMatchLineup(int areaid, int spid, String mr) {
        KookongSDK.manualMatchLineup(areaid, spid, mr, new IRequestResult<ManualMatchLineupData>() {
            @Override
            public void onSuccess(String s, ManualMatchLineupData manualMatchLineupData) {
                if (manualMatchLineupData != null) {
                    switch (manualMatchLineupData.getResultCode()) {
                        case 0:
                            canContinue = false;
                            mTextView.setText("获取频道表成功!!");
                            mTextView.setTextColor(Color.GREEN);
                            mGridAdapter.setChannels(manualMatchLineupData.getList());
                            break;
                        case 1:
                            canContinue = true;
                            mTextView.setText("继续测试");
                            mTextView.setTextColor(Color.BLACK);
                            mGridAdapter.setChannels(manualMatchLineupData.getList());
                            break;
                        case 2:
                        default:
                            canContinue = false;
                            mTextView.setTextColor(Color.RED);
                            mTextView.setText("没有匹配到合适的频道表");
                            mGridAdapter.setChannels(null);
                            break;
                    }
                }
            }

            @Override
            public void onFail(Integer integer, String s) {
                TipsUtil.toast(MatchLineupActivity.this, "错误:" + s);
            }
        });
    }

}
