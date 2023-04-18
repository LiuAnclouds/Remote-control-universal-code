mkdir -p app/src/main/java
mv src/* app/src/main/java/
rmdir src
mkdir assets 
mv assets app/src/main/
mv res app/src/main/
mv AndroidManifest.xml app/src/main/
mv libs app/
mv build.gradle  app
echo "include ':app'" > settings.gradle
echo """ 

buildscript {
    repositories {
        jcenter()
        maven{
            url 'https://maven.aliyun.com/repository/jcenter'
        }
        maven{
            url 'https://maven.aliyun.com/repository/google'
        }
        google()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:3.4.0'

        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}
allprojects {
    repositories {
        jcenter()
        maven{
            url 'https://maven.aliyun.com/repository/jcenter'
        }
        maven{
            url 'https://maven.aliyun.com/repository/google'
        }
    }
}


""">build.gradle