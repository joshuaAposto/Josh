package angou;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import java.util.List;
import java.io.File;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import android.widget.Toast;
import android.content.Intent;
import android.content.ComponentName;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.pm.ActivityInfo;
import android.net.Uri;

import org.json.JSONObject;

public class Main extends Activity {

    private static final String CURRENT_VERSION = "1.0";
    private static final String VERSION_CHECK_URL = "https://bskey.vercel.app/version";

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        checkForUpdate();
    }

    private void checkForUpdate() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    URL url = new URL(VERSION_CHECK_URL);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.setRequestMethod("GET");
                    conn.setConnectTimeout(5000);
                    conn.setReadTimeout(5000);

                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    StringBuilder sb = new StringBuilder();
                    String line;
                    while ((line = reader.readLine()) != null) sb.append(line);
                    reader.close();
                    conn.disconnect();

                    JSONObject json = new JSONObject(sb.toString());
                    final String latestVersion = json.optString("version", CURRENT_VERSION);
                    final String downloadUrl = json.optString("download_url", "");
                    final String notes = json.optString("notes", "");

                    if (!latestVersion.equals(CURRENT_VERSION) && !downloadUrl.isEmpty()) {
                        new Handler(Looper.getMainLooper()).post(new Runnable() {
                            @Override
                            public void run() {
                                showUpdateDialog(latestVersion, downloadUrl, notes);
                            }
                        });
                    } else {
                        new Handler(Looper.getMainLooper()).post(new Runnable() {
                            @Override
                            public void run() {
                                launchGame();
                            }
                        });
                    }
                } catch (Exception e) {
                    new Handler(Looper.getMainLooper()).post(new Runnable() {
                        @Override
                        public void run() {
                            launchGame();
                        }
                    });
                }
            }
        }).start();
    }

    private void showUpdateDialog(final String newVersion, final String downloadUrl, final String notes) {
        String message = "New version available: v" + newVersion + "\nCurrent version: v" + CURRENT_VERSION;
        if (!notes.isEmpty()) {
            message += "\n\nWhat's new:\n" + notes;
        }

        new AlertDialog.Builder(this)
            .setTitle("Update Available")
            .setMessage(message)
            .setCancelable(false)
            .setPositiveButton("Download Update", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(downloadUrl));
                    startActivity(browserIntent);
                    finish();
                }
            })
            .setNegativeButton("Skip", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    launchGame();
                }
            })
            .show();
    }

    private void launchGame() {
        Context context = getApplicationContext();
        PackageManager packageManager = context.getPackageManager();
        String packageName = "com.netease.newspike";

        Runtime r = Runtime.getRuntime();

        try {
            String thisAppLib = (new File(context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir)).getParent() + "/lib/arm64/library.so";
            String bsAppLib = (new File(context.getPackageManager().getApplicationInfo(packageName, 0).publicSourceDir)).getParent() + "/lib/arm64/library.so";
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
