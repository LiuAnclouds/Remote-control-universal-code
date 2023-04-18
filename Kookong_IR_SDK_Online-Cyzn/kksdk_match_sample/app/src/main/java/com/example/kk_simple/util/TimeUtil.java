package com.example.kk_simple.util;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * TimeUtils
 * 
 * 时间格式化工具类, 可能根据不同的需求, 需要添加不同的格式.
 */
public class TimeUtil
{
    
    public static final ThreadLocal<java.text.SimpleDateFormat> DEFAULT_DATE_FORMAT = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.CHINA);
        }
    };
    public static final ThreadLocal<java.text.SimpleDateFormat> DATE_FORMAT_DATE = new ThreadLocal<java.text.SimpleDateFormat>()
    {
        protected SimpleDateFormat initialValue() {
            return new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.CHINA);
        }
    };
    public static final ThreadLocal<java.text.SimpleDateFormat> DATE_FORMAT_HOUR = new ThreadLocal<java.text.SimpleDateFormat>()
            {
                protected SimpleDateFormat initialValue() {
                    return new java.text.SimpleDateFormat("HH:mm", Locale.CHINA);
                }
            };
    
    /**
     * long time to string 如果遇到不同的UI需求, 显示不同的时间字符串, 请使用这个函数 传入你想要的dataformat
     * 
     * @param timeInMillis
     * @param dateFormat
     * @return
     */
    public static String getTime(long timeInMillis, SimpleDateFormat dateFormat) {
        return dateFormat.format(new Date(timeInMillis));
    }
    
    /**
     * long time to string, format is {@link #DEFAULT_DATE_FORMAT}
     * 
     * @param timeInMillis
     * @return
     */
    public static String getTime(long timeInMillis) {
        return getTime(timeInMillis, DEFAULT_DATE_FORMAT.get());
    }
    
    public static String getHourTime(long timeInMillis){
        return getTime(timeInMillis,DATE_FORMAT_HOUR.get());
    }
    
    /**
     * get current time in milliseconds
     * 
     * @return
     */
    public static long getCurrentTimeInLong() {
        return System.currentTimeMillis();
    }
    
    /**
     * get current time in milliseconds, format is {@link #DEFAULT_DATE_FORMAT}
     * 
     * @return
     */
    public static String getCurrentTimeInString() {
        return getTime(getCurrentTimeInLong());
    }
    
    /**
     * get current time in milliseconds
     * 
     * @return
     */
    public static String getCurrentTimeInString(SimpleDateFormat dateFormat) {
        return getTime(getCurrentTimeInLong(), dateFormat);
    }
}
