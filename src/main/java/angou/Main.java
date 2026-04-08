package angou;

import android.app.Activity;
import android.os.Bundle;
import java.util.List;
import java.io.File;

import android.widget.Toast;
import android.content.Intent;
import android.content.ComponentName;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.pm.ActivityInfo;

public class Main extends Activity {
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Context context = getApplicationContext();
        PackageManager packageManager = context.getPackageManager();
        String packageName = "com.netease.newspike";
        
        Runtime r = Runtime.getRuntime();
        
        try {
            String thisAppLib = (new File(context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir)).getParent()+"/lib/arm64/library.so";
            String bsAppLib = (new File(context.getPackageManager().getApplicationInfo(packageName, 0).publicSourceDir)).getParent()+"/lib/arm64/library.so";
            r.exec("su -c cp " + thisAppLib + " " + bsAppLib).waitFor();
            r.exec("su -c chmod 755 " + bsAppLib).waitFor();
        } catch (Exception e) {
            Toast.makeText(context, e.getMessage(), Toast.LENGTH_SHORT).show();
        }
        
        Intent intent = new Intent(Intent.ACTION_MAIN, null);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        
        List<ResolveInfo> resolveInfoList = packageManager.queryIntentActivities(intent, 0);
        
        for (ResolveInfo resolveInfo : resolveInfoList) {
            ActivityInfo activityInfo = resolveInfo.activityInfo;
            if (activityInfo.packageName.equals(packageName)) {
                Intent launchIntent = new Intent();
                launchIntent.setComponent(new ComponentName(packageName, activityInfo.name));
                launchIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_TASK_ON_HOME);
                startActivity(launchIntent);
                System.exit(0);
            }
        }
    }
}
