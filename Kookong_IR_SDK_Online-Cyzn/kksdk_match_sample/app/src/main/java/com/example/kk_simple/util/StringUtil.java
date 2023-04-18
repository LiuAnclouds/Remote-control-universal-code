package com.example.kk_simple.util;

import android.text.TextUtils;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * String Utils
 * 
 * @author <a href="http://www.trinea.cn" target="_blank">Trinea</a> 2011-7-22
 */
public class StringUtil
{
    public static String getBuildString(String remoteid, String all) {
        StringBuffer buffer = new StringBuffer();
        LinkedHashSet<String> hash = getName(remoteid, all);
        if (hash.size() > 0) {
            Iterator<String> iterator = hash.iterator();
            while (iterator.hasNext()) {
                buffer.append(iterator.next() + ",");
            }
            System.out.println("获得的上传的remotebuffer:" + buffer.toString());
            String remote = buffer.toString().substring(0, buffer.toString().lastIndexOf(","));
            System.out.println("获得的上传的remote:" + remote);
            return remote;
        }
        return null;
    }
    
    public static LinkedHashSet<String> getName(String remoteid, String all) {
        
        LinkedHashSet<String> h1 = new LinkedHashSet<String>();
        LinkedHashSet<String> h2 = new LinkedHashSet<String>();
        for (int i = 0; i < all.split(",").length; i++) {
            h1.add(all.split(",")[i]);
        }
        for (int i = 0; i < remoteid.split(",").length; i++) {
            h2.add(remoteid.split(",")[i]);
        }
        h1.removeAll(h2);
        return h1;
    }
    
    /**
     * change string to long safely.
     * 
     * @param strLong
     * @return
     */
    public static long parseLong(String strLong) {
        long num = 0;
        try {
            num = Long.parseLong(strLong);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return num;
    }
    
    /**
     * change string to integer safely.
     * 
     * @param strInt
     * @return
     */
    public static int parseInt(String strInt) {
        int num = 0;
        try {
            num = Integer.parseInt(strInt);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return num;
    }
    
    /**
     * 字符串是否是数字 123 true 1 true a1 false
     * 
     * @param str
     * @return
     */
    public static boolean isNumeric(String str) {
        Pattern pattern = Pattern.compile("[0-9]*");
        return pattern.matcher(str).matches();
    }
    
    /**
     * is null or its length is 0 or it is made by space
     * <p/>
     * 
     * <pre>
     * isBlank(null) = true;
     * isBlank(&quot;&quot;) = true;
     * isBlank(&quot;  &quot;) = true;
     * isBlank(&quot;a&quot;) = false;
     * isBlank(&quot;a &quot;) = false;
     * isBlank(&quot; a&quot;) = false;
     * isBlank(&quot;a b&quot;) = false;
     * </pre>
     * 
     * @param str
     * @return if string is null or its size is 0 or it is made by space, return
     *         true, else return false.
     */
    public static boolean isBlank(String str) {
        return (str == null || str.trim().length() == 0);
    }
    
    /**
     * is null or its length is 0
     * <p/>
     * 
     * <pre>
     * isNull(null) = true;
     * isNull(&quot;&quot;) = true;
     * isNull(&quot;  &quot;) = false;
     * </pre>
     * 
     * @param str
     * @return if string is null or its size is 0, return true, else return
     *         false.
     */
    public static boolean isNull(String str) {
        return (str == null || str.length() == 0 || "null".equals(str));
    }
    
    /**
     * null string to empty string
     * <p/>
     * 
     * <pre>
     * nullStrToEmpty(null) = &quot;&quot;;
     * nullStrToEmpty(&quot;&quot;) = &quot;&quot;;
     * nullStrToEmpty(&quot;aa&quot;) = &quot;aa&quot;;
     * </pre>
     * 
     * @param str
     * @return
     */
    public static String nullStrToEmpty(String str) {
        return (str == null ? "" : str);
    }
    
    /**
     * capitalize first letter
     * <p/>
     * 
     * <pre>
     * capitalizeFirstLetter(null)     =   null;
     * capitalizeFirstLetter("")       =   "";
     * capitalizeFirstLetter("2ab")    =   "2ab"
     * capitalizeFirstLetter("a")      =   "A"
     * capitalizeFirstLetter("ab")     =   "Ab"
     * capitalizeFirstLetter("Abc")    =   "Abc"
     * </pre>
     * 
     * @param str
     * @return
     */
    public static String capitalizeFirstLetter(String str) {
        if (isNull(str)) {
            return str;
        }
        
        char c = str.charAt(0);
        return (!Character.isLetter(c) || Character.isUpperCase(c)) ? str : new StringBuilder(str.length()).append(Character.toUpperCase(c)).append(str.substring(1)).toString();
    }
    
    /**
     * encoded in utf-8
     * <p/>
     * 
     * <pre>
     * utf8Encode(null)        =   null
     * utf8Encode("")          =   "";
     * utf8Encode("aa")        =   "aa";
     * utf8Encode("啊啊啊啊")   = "%E5%95%8A%E5%95%8A%E5%95%8A%E5%95%8A";
     * </pre>
     * 
     * @param str
     * @return
     * @throws UnsupportedEncodingException
     *             if an error occurs
     */
    public static String utf8Encode(String str) {
        if (!isNull(str) && str.getBytes().length != str.length()) {
            try {
                return URLEncoder.encode(str, "UTF-8");
            }
            catch (UnsupportedEncodingException e) {
                throw new RuntimeException("UnsupportedEncodingException occurred. ", e);
            }
        }
        return str;
    }
    
    public static String utf8Decode(String str) {
        if (!isNull(str)) {
            try {
                return URLDecoder.decode(str, "UTF-8");
            }
            catch (UnsupportedEncodingException e) {
                throw new RuntimeException("UnsupportedEncodingException occurred. ", e);
            }
        }
        return str;
    }
    
    /**
     * encoded in utf-8, if exception, return defultReturn
     * 
     * @param str
     * @param defultReturn
     * @return
     */
    public static String utf8Encode(String str, String defultReturn) {
        if (!isNull(str) && str.getBytes().length != str.length()) {
            try {
                return URLEncoder.encode(str, "UTF-8");
            }
            catch (UnsupportedEncodingException e) {
                return defultReturn;
            }
        }
        return str;
    }
    
    /**
     * get innerHtml from href
     * <p/>
     * 
     * <pre>
     * getHrefInnerHtml(null)                                  = ""
     * getHrefInnerHtml("")                                    = ""
     * getHrefInnerHtml("mp3")                                 = "mp3";
     * getHrefInnerHtml("&lt;a innerHtml&lt;/a&gt;")                    = "&lt;a innerHtml&lt;/a&gt;";
     * getHrefInnerHtml("&lt;a&gt;innerHtml&lt;/a&gt;")                    = "innerHtml";
     * getHrefInnerHtml("&lt;a&lt;a&gt;innerHtml&lt;/a&gt;")                    = "innerHtml";
     * getHrefInnerHtml("&lt;a href="baidu.com"&gt;innerHtml&lt;/a&gt;")               = "innerHtml";
     * getHrefInnerHtml("&lt;a href="baidu.com" title="baidu"&gt;innerHtml&lt;/a&gt;") = "innerHtml";
     * getHrefInnerHtml("   &lt;a&gt;innerHtml&lt;/a&gt;  ")                           = "innerHtml";
     * getHrefInnerHtml("&lt;a&gt;innerHtml&lt;/a&gt;&lt;/a&gt;")                      = "innerHtml";
     * getHrefInnerHtml("jack&lt;a&gt;innerHtml&lt;/a&gt;&lt;/a&gt;")                  = "innerHtml";
     * getHrefInnerHtml("&lt;a&gt;innerHtml1&lt;/a&gt;&lt;a&gt;innerHtml2&lt;/a&gt;")        = "innerHtml2";
     * </pre>
     * 
     * @param href
     * @return <ul>
     *         <li>if href is null, return ""</li> <li>if not match regx, return
     *         source</li> <li>return the last string that match regx</li>
     *         </ul>
     */
    public static String getHrefInnerHtml(String href) {
        if (isNull(href)) {
            return "";
        }
        
        String hrefReg = ".*<[\\s]*a[\\s]*.*>(.+?)<[\\s]*/a[\\s]*>.*";
        Pattern hrefPattern = Pattern.compile(hrefReg, Pattern.CASE_INSENSITIVE);
        Matcher hrefMatcher = hrefPattern.matcher(href);
        if (hrefMatcher.matches()) {
            return hrefMatcher.group(1);
        }
        return href;
    }
    
/**
	 * process special char in html
	 * <p/>
	 * <pre>
	 * htmlEscapeCharsToString(null) = null;
	 * htmlEscapeCharsToString("") = "";
	 * htmlEscapeCharsToString("mp3") = "mp3";
	 * htmlEscapeCharsToString("mp3&lt;") = "mp3<";
	 * htmlEscapeCharsToString("mp3&gt;") = "mp3\>";
	 * htmlEscapeCharsToString("mp3&amp;mp4") = "mp3&mp4";
	 * htmlEscapeCharsToString("mp3&quot;mp4") = "mp3\"mp4";
	 * htmlEscapeCharsToString("mp3&lt;&gt;&amp;&quot;mp4") = "mp3\<\>&\"mp4";
	 * </pre>
	 *
	 * @param source
	 * @return
	 */
    public static String htmlEscapeCharsToString(String source) {
        return StringUtil.isNull(source) ? source : source.replaceAll("&lt;", "<").replaceAll("&gt;", ">").replaceAll("&amp;", "&").replaceAll("&quot;", "\"");
    }
    
    /**
     * transform half width char to full width char
     * <p/>
     * 
     * <pre>
     * fullWidthToHalfWidth(null) = null;
     * fullWidthToHalfWidth("") = "";
     * fullWidthToHalfWidth(new String(new char[] {12288})) = " ";
     * fullWidthToHalfWidth("！＂＃＄％＆) = "!\"#$%&";
     * </pre>
     * 
     * @param s
     * @return
     */
    public static String fullWidthToHalfWidth(String s) {
        if (isNull(s)) {
            return s;
        }
        
        char[] source = s.toCharArray();
        for (int i = 0; i < source.length; i++) {
            if (source[i] == 12288) {
                source[i] = ' ';
                // } else if (source[i] == 12290) {
                // source[i] = '.';
            }
            else if (source[i] >= 65281 && source[i] <= 65374) {
                source[i] = (char) (source[i] - 65248);
            }
            else {
                source[i] = source[i];
            }
        }
        return new String(source);
    }
    
    /**
     * transform full width char to half width char
     * <p/>
     * 
     * <pre>
     * halfWidthToFullWidth(null) = null;
     * halfWidthToFullWidth("") = "";
     * halfWidthToFullWidth(" ") = new String(new char[] {12288});
     * halfWidthToFullWidth("!\"#$%&) = "！＂＃＄％＆";
     * </pre>
     * 
     * @param s
     * @return
     */
    public static String halfWidthToFullWidth(String s) {
        if (isNull(s)) {
            return s;
        }
        
        char[] source = s.toCharArray();
        for (int i = 0; i < source.length; i++) {
            if (source[i] == ' ') {
                source[i] = (char) 12288;
                // } else if (source[i] == '.') {
                // source[i] = (char)12290;
            }
            else if (source[i] >= 33 && source[i] <= 126) {
                source[i] = (char) (source[i] + 65248);
            }
            else {
                source[i] = source[i];
            }
        }
        return new String(source);
    }
    
    public final static String md5(String s) {
        try {
            byte[] btInput = s.getBytes();
            MessageDigest mdInst = MessageDigest.getInstance("MD5");
            mdInst.update(btInput);
            byte[] md = mdInst.digest();
            StringBuffer sb = new StringBuffer();
            for (int i = 0; i < md.length; i++) {
                int val = ((int) md[i]) & 0xff;
                if (val < 16)
                    sb.append("0");
                sb.append(Integer.toHexString(val));
            }
            return sb.toString();
        }
        catch (Exception e) {
            return null;
        }
    }
    
    /**
     * 比较两个字符串（大小写不敏感）。
     * 
     * <pre>
     * StringUtil.equals(null, null) = true
     * StringUtil.equals(null, "abc") = false
     * StringUtil.equals("abc", null) = false
     * StringUtil.equals("abc", "abc") = true
     * StringUtil.equals("abc", "ABC") = true
     * </pre>
     * 
     */
    public static boolean equalsIgnoreCase(String str1, String str2) {
        if (str1 == null) {
            return str2 == null;
        }
        
        return str1.equalsIgnoreCase(str2);
    }
    
    /**
     * 比较两个字符串（大小写敏感）。
     * 
     * <pre>
     * StringUtil.equals(null, null) = true
     * StringUtil.equals(null, "abc") = false
     * StringUtil.equals("abc", null) = false
     * StringUtil.equals("abc", "abc") = true
     * StringUtil.equals("abc", "ABC") = false
     * </pre>
     * 
     * *
     * 
     * @param str1
     *            要比较的字符串1
     * @param str2
     *            要比较的字符串2
     * @return 如果两个字符串相同，或者都是<code>null</code>，则返回<code>true</code>
     */
    public static boolean equals(String str1, String str2) {
        if (str1 == null) {
            return str2 == null;
        }
        
        return str1.equals(str2);
    }
    
    /**
     * 解析 字符串 到 int数组 到 int数组 <br>
     * 
     * String = 1,2,3 => int[] = {1, 2, 3}
     */
    public static int[] parseIntArray(String text) {
        return parseIntArray(text, ",");
    }
    
    /**
     * 解析 字符串
     * 
     * @param pattern
     * @return
     */
    public static int[] parseIntArray(String text, String delimiter) {
        if (TextUtils.isEmpty(text)) {
            return null;
        }
        String[] strings = text.split(delimiter);
        int[] intpatterns = new int[strings.length];
        for (int i = 0; i < strings.length; i++) {
            intpatterns[i] = StringUtil.parseInt(strings[i]);
        }
        
        return intpatterns;
    }
    
    /**
     * 
     * @param text
     * @param delimiter
     * @return
     */
    public static String[] parseStringArray(String text, String delimiter) {
        if (TextUtils.isEmpty(text)) {
            return null;
        }
        return text.split(delimiter);
    }
    
    /**
     * 字符串前面补充字符，保证字符串的最大长度。
     * 
     * @param string
     *            源字符串
     * @param minLength
     *            字符串需要达到的最小长度.
     * @param padChar
     *            前面补充的字符
     * @return 补充完后的字符
     */
    public static String padStart(String string, int minLength, char padChar) {
        if (string.length() >= minLength) {
            return string;
        }
        
        StringBuilder sb = new StringBuilder(minLength);
        for (int i = string.length(); i < minLength; i++) {
            sb.append(padChar);
        }
        
        sb.append(string);
        return sb.toString();
    }
    
    public static String replaceBlank(String str) {
        String dest = "";
        if (str != null) {
            Pattern p = Pattern.compile("\\s*|\t|\r|\n");
            Matcher m = p.matcher(str);
            dest = m.replaceAll("");
        }
        return dest;
    }
    
    //将16进制的字符串转换成byte数组，每2个16进制数字转成一个byte
	public static byte[] hex2Bytes(String hex) {
		hex = hex.replaceAll("[^0-9,a-f,A-F]", "");
		byte[] bytes = new byte[hex.length() / 2];
		for (int i = 0; i < bytes.length; i++) {
			bytes[i] = (byte) Integer.parseInt(hex.substring(i * 2, i * 2 + 2),
					16);
		}
		
		return bytes;
	}
	
	// 将字节数组转换成16进制字符串
	public static String byte2Hexstr(byte[] bytes) {
		StringBuilder buf = new StringBuilder(bytes.length * 2);
		for (int i = 0; i < bytes.length; i++) {
			int tmp = (bytes[i] >> 4) & 0x0F;
			buf.append(getHexChar(tmp));

			tmp = bytes[i] & 0x0F;
			buf.append(getHexChar(tmp));
		}

		return buf.toString();
	}

	private static char getHexChar(int value) {
		if (value >= 0 && value < 10) {
			return (char) ('0' + value);
		} else {
			return (char) ('A' + (value - 10));
		}
	}
}
