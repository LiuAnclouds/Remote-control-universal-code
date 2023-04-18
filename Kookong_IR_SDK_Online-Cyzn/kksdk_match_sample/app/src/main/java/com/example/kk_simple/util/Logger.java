/*
 * Copyright (c) 2013. wyouflf (wyouflf@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.kk_simple.util;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import android.annotation.TargetApi;
import android.os.Build;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;

/**
 * Log工具，类似android.util.Log。 tag自动产生，格式:
 * customTagPrefix:className.methodName(L:lineNumber),
 * customTagPrefix为空时只输出：className.methodName(L:lineNumber)。
 * <p/>
 * Author: wyouflf Date: 13-7-24 Time: 下午12:23
 */
public class Logger
{
    
    public static String customTagPrefix = "";
    private static String mLogPath = null;
    
    static {
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File externalStorageDirectory = Environment.getExternalStorageDirectory();
            mLogPath = externalStorageDirectory.getAbsolutePath() + File.separator + "log.txt";
        }
    }
    
    private Logger() {}
    
    public static boolean DEBUG = true;
    public static boolean allowD = DEBUG;
    public static boolean allowE = DEBUG;
    public static boolean allowI = DEBUG;
    public static boolean allowV = DEBUG;
    public static boolean allowW = DEBUG;
    public static boolean allowWtf = DEBUG;
    public static boolean allowWrite = DEBUG;
    
    private static String generateTag(StackTraceElement caller) {
        String tag = "%s.%s(L:%d)";
        String callerClazzName = caller.getClassName();
        callerClazzName = callerClazzName.substring(callerClazzName.lastIndexOf(".") + 1);
        tag = String.format(tag, callerClazzName, caller.getMethodName(), caller.getLineNumber());
        tag = TextUtils.isEmpty(customTagPrefix) ? tag : customTagPrefix + ":" + tag;
        return tag;
    }
    
    public static CustomLogger customLogger;
    
    public interface CustomLogger
    {
        void d(String tag, String content);
        
        void d(String tag, String content, Throwable tr);
        
        void e(String tag, String content);
        
        void e(String tag, String content, Throwable tr);
        
        void i(String tag, String content);
        
        void i(String tag, String content, Throwable tr);
        
        void v(String tag, String content);
        
        void v(String tag, String content, Throwable tr);
        
        void w(String tag, String content);
        
        void w(String tag, String content, Throwable tr);
        
        void w(String tag, Throwable tr);
        
        void wtf(String tag, String content);
        
        void wtf(String tag, String content, Throwable tr);
        
        void wtf(String tag, Throwable tr);
    }
    
    public static void d(String content) {
        if (!allowD)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.d(tag, content);
        }
        else {
            Log.d(tag, content);
        }
    }
    
    public static void d(String content, Throwable tr) {
        if (!allowD)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.d(tag, content, tr);
        }
        else {
            Log.d(tag, content, tr);
        }
    }
    
    public static void e(String content) {
        if (!allowE)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.e(tag, content);
        }
        else {
            Log.e(tag, content);
        }
    }
    
    public static void e(String content, Throwable tr) {
        if (!allowE)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.e(tag, content, tr);
        }
        else {
            Log.e(tag, content, tr);
        }
    }
    
    public static void i(String content) {
        if (!allowI)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.i(tag, content);
        }
        else {
            Log.i(tag, content);
        }
    }
    
    public static void i(String content, Throwable tr) {
        if (!allowI)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.i(tag, content, tr);
        }
        else {
            Log.i(tag, content, tr);
        }
    }
    
    public static void v(String content) {
        if (!allowV)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.v(tag, content);
        }
        else {
            Log.v(tag, content);
        }
    }
    
    public static void v(String content, Throwable tr) {
        if (!allowV)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.v(tag, content, tr);
        }
        else {
            Log.v(tag, content, tr);
        }
    }
    
    public static void w(String content) {
        if (!allowW)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.w(tag, content);
        }
        else {
            Log.w(tag, content);
        }
    }
    
    public static void w(String content, Throwable tr) {
        if (!allowW)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.w(tag, content, tr);
        }
        else {
            Log.w(tag, content, tr);
        }
    }
    
    public static void w(Throwable tr) {
        if (!allowW)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.w(tag, tr);
        }
        else {
            Log.w(tag, tr);
        }
    }
    
    @TargetApi (Build.VERSION_CODES.FROYO)
    public static void wtf(String content) {
        if (!allowWtf)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.wtf(tag, content);
        }
        else {
            Log.wtf(tag, content);
        }
    }
    
    @TargetApi (Build.VERSION_CODES.FROYO)
    public static void wtf(String content, Throwable tr) {
        if (!allowWtf)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.wtf(tag, content, tr);
        }
        else {
            Log.wtf(tag, content, tr);
        }
    }
    
    @TargetApi (Build.VERSION_CODES.FROYO)
    public static void wtf(Throwable tr) {
        if (!allowWtf)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        
        if (customLogger != null) {
            customLogger.wtf(tag, tr);
        }
        else {
            Log.wtf(tag, tr);
        }
    }
    
    /**
     * 把日志写入sd卡的一个函数, 仅做测试之用!
     * 
     * @param tr
     */
    public static void write(String log) {
        if (!allowWrite)
            return;
        StackTraceElement caller = getCallerStackTraceElement();
        String tag = generateTag(caller);
        Log.e(tag, log);
        writeLog(log);
    }
    
    private static FileWriter mFileWriter;
    
    private static void writeLog(String log) {
        try {
            if (mFileWriter == null) {
                File file = new File(mLogPath);
                mFileWriter = new FileWriter(file);
            }
            mFileWriter.write(log);
            mFileWriter.flush();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    public static StackTraceElement getCallerStackTraceElement() {
        return Thread.currentThread().getStackTrace()[4];
    }
    
}
