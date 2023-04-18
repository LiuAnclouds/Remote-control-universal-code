/**  
 * Copyright © 2016Kookong.com. All rights reserved.
 *
 * @title: CountryUtil.java
 * @prject: kksdk_ir_no_acext_global_sample
 * @package: com.example.kk_simple.util
 * @description: TODO
 * @author: Administrator  
 * @date: 2016年9月28日 下午2:54:44
 * @version: V1.2.0  
 */
package com.example.kk_simple.util;

/**
 * @className: CountryUtil
 * @description: TODO
 * @author: Administrator
 * @date: 2016年9月28日 下午2:54:44
 */
public class CountryUtil
{
    
    public static boolean isChinaMainLand(String countryCode) {
        if (null != countryCode && "CN".equals(countryCode)) {
            return true;
        }
        return false;
    }
}
