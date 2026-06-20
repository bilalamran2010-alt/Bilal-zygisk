package com.android.support;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.content.Intent;
import android.net.Uri;
import android.provider.Settings;

public class PermissionActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // DISPLAY OVER OTHER APPS PERMISSION
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                Intent i = new Intent(
					Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
					Uri.parse("package:" + getPackageName())
                );
                startActivity(i);
                return; // permission mile bina aage mat jao
            }
        }

        // Permission mil chuki hai
        // Yahan apna service / menu start kar sakta hai
        // startService(new Intent(this, MenuService.class));

        finish(); // Activity band, overlay/service chalti rahe
    }
}
