package com.example.kk_simple.util;

import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;

public class DateUtil
{
    
    public static final int MINUTE = 1000 * 60;
    public static final int HOUR = MINUTE * 60;
    public static final long DAY = HOUR * 24;
    public static final long YEAR = DAY * 365;
    
    public static List<String> weekList = new ArrayList<String>();
    
    static {
        weekList.add("周一");
        weekList.add("周二");
        weekList.add("周三");
        weekList.add("周四");
        weekList.add("周五");
        weekList.add("周六");
        weekList.add("周日");
        weekList.add("下周一");
        weekList.add("下周二");
        weekList.add("下周三");
        weekList.add("下周四");
        weekList.add("下周五");
        weekList.add("下周六");
        weekList.add("下周日");
    }
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatMMdd = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("MM-dd", Locale.CHINA);
        }
    };
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatYYMMddHHmm = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.CHINA);
        }
    };
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatHHmm = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("HH:mm", Locale.CHINA);
        }
    };
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatYYMMdd = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yy-MM-dd", Locale.CHINA);
        }
    };
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormat_yyyy_MM_ddHHmmss = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.CHINA);
        }
    };
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatyyyyMMdd = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyyMMdd", Locale.CHINA);
        }
    };
    
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatyyyyMM = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyyMM", Locale.CHINA);
        }
    };
    public static final ThreadLocal<java.text.SimpleDateFormat> dateFormatyyyyMMddHHmm = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyyMMddHHmm", Locale.CHINA);
        }
    };
    
    public static Date getMessageTime(long date) {
        try {
            
            Date dateTime = new Date(date);
            return dateTime;
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    
    public static int diffTime(Date d1, Date d2, int type) {
        if (d1 != null && d2 != null) {
            Calendar cal = Calendar.getInstance();
            cal.setTime(d1);
            cal.clear(Calendar.MILLISECOND);
            d1 = cal.getTime();
            
            cal.setTime(d2);
            cal.clear(Calendar.MILLISECOND);
            d2 = cal.getTime();
            
            long diff = d2.getTime() - d1.getTime();
            
            int retValue = 0;
            if (type == YEAR) {
                retValue = (int) (diff / YEAR);
            }
            else if (type == DAY) {
                float r = (float) diff / DAY;
                retValue = Math.round(r);
            }
            else if (type == MINUTE) {
                float m = (float) diff / MINUTE;
                retValue = Math.round(m);
            }
            else {
                throw new IllegalArgumentException("Invalid type " + type);
            }
            return retValue;
        }
        else
            return 0;
    }
    
    public static Date parseLongDate(String longDateString) {
        try {
            return dateFormatYYMMddHHmm.get().parse(longDateString);
        }
        catch (ParseException e) {}
        
        return null;
    }
    
    public static String getTime(Date date) {
        String time = dateFormatMMdd.get().format(date);
        return time;
    }
    
    public static String getTimeHHMM(long timestamp) {
        String time = dateFormatHHmm.get().format(timestamp);
        return time;
    }
    
    public static String getTimeHHMM(Date date) {
        String time = dateFormatHHmm.get().format(date);
        return time;
    }
    
    public static String formatTimeYYMMdd(Date date) {
        return dateFormatYYMMdd.get().format(date);
    }
    
    public static String formatTimeyyyyMMdd(Date date) {
        return dateFormatyyyyMMdd.get().format(date);
    }
    
    public static String formatTimeMMdd(Date date) {
        return dateFormatMMdd.get().format(date);
    }
    
    public static String formatTimeyyyy_MM_dd_HHmmss(Date date) {
        return dateFormat_yyyy_MM_ddHHmmss.get().format(date);
    }
    
    public static long getCurrentTime() {
        //TODO gaoshuai maybe need read time from Util.PREF_NOW_TIME
        return System.currentTimeMillis();
    }
    
    public static int getToday() {
        int today = Calendar.getInstance().get(Calendar.DAY_OF_WEEK) - 1;
        if (today == 0) {
            today = 7;
        }
        
        return today;
    }
    
    private static int getWeek(Date date) {
        Calendar cal = Calendar.getInstance();
        int day1 = cal.get(Calendar.DAY_OF_YEAR);
        int today = cal.get(Calendar.DAY_OF_WEEK) - 1;
        if (today == 0) {
            today = 7;
        }
        
        cal.setTime(date);
        int day = cal.get(Calendar.DAY_OF_WEEK) - 1;
        if (day == 0) {
            day = 7;
        }
        
        cal.add(Calendar.DAY_OF_YEAR, today - day);
        int day2 = cal.get(Calendar.DAY_OF_YEAR);
        if (day2 != day1) {
            day = 7 + day;
        }
        
        return day;
    }
    
    /** 分钟数转换成小时 */
    public static String getMinuteToHourString(int minutes) {
        int hour = minutes / 60;
        int other = minutes % 60;
        StringBuffer sbBuffer = new StringBuffer();
        if (hour > 0) {
            sbBuffer.append(hour + "小时");
        }
        
        if (other > 0) {
            sbBuffer.append(other + "分");
        }
        return sbBuffer.toString();
    }
    
    public static String getHourStringByMinute(int minute) {
        DecimalFormat decimalFormat = new DecimalFormat("0.0");
        double hour = minute * 1.0 / 60;
        return decimalFormat.format(hour) + "h";
    }
    
    public static String getDisplayTime(long minutes, boolean count) {
        Date date = null;
        if (count) {//如果传参是总和的话，不加当前时间
            date = new Date(minutes);
        }
        else {
            date = new Date(TimeUtil.getCurrentTimeInLong() + minutes * 60 * 1000);
        }
        
        StringBuffer stringBuffer = new StringBuffer();
        Calendar calNow = Calendar.getInstance();
        int daysOfYear = calNow.get(Calendar.DAY_OF_YEAR);
        Calendar calDate = Calendar.getInstance();
        calDate.setTime(date);
        int dayOfWeek = calDate.get(Calendar.DAY_OF_WEEK);
        if (dayOfWeek == 1)// 改星期日为7
            dayOfWeek = 7;
        else
            dayOfWeek--;
        
        int diffDay = calDate.get(Calendar.DAY_OF_YEAR) - daysOfYear;
        if (diffDay == 0) {//如果是今天，不做处理
        
        }
        else if (diffDay == 1) {//如果是明天，添加
            stringBuffer.append("明天");
        }
        else {
            if (diffDay < 15) {
                String[] weekDays = (String[])weekList.toArray(new String[weekList.size()]);
                int day = getWeek(date);
                stringBuffer.append(weekDays[day - 1]);
            }
        }
        stringBuffer.append(TimeUtil.getHourTime(date.getTime())).append("分");
        return stringBuffer.toString();
    }
}
