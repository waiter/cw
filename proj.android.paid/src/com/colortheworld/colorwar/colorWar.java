/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.colortheworld.colorwar;

import java.io.File;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.WindowManager;

import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.game.UMGameAgent;

public class colorWar extends Cocos2dxActivity {

	static {
		System.loadLibrary("game");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		hideNavigationBar();

		UMGameAgent.init(this);

		initPath();
		initHandler();
	}
	
	private void initPath() {
		String savePath;
		if (Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED)) {
			try {
				savePath = getApplicationContext().getExternalFilesDir(null)
						.getAbsolutePath() + "/";
			} catch (Exception e) {
				savePath = getApplicationContext().getFilesDir()
						.getAbsolutePath() + "/";
			}
		} else {
			savePath = getApplicationContext().getFilesDir().getAbsolutePath()
					+ "/";
		}
		File file = new File(savePath);
		if (!file.exists()) {
			file.mkdirs();
		}
		final String p = savePath;
		runOnGLThread(new Runnable() {
			@Override
			public void run() {
				j.nsp1(p);
				j.nsa(false);
			}
		});
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) {
			hideNavigationBar();
		}
	}

	@SuppressLint("NewApi")
	private void hideNavigationBar() {
		try {
			int sysVersion = Build.VERSION.SDK_INT;
			if (sysVersion >= 19) {
				getWindow().getDecorView().setSystemUiVisibility(
						View.SYSTEM_UI_FLAG_LAYOUT_STABLE
								| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
								| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
								| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
								| View.SYSTEM_UI_FLAG_FULLSCREEN
								| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
			}
		} catch (Exception e) {

		}
	}
	
	@SuppressLint("HandlerLeak")
	private void initHandler() {
		Constant.handler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case Constant.ACTION_SHARE:
					share();
					break;
				case Constant.SHOW_SCREEN_AD:
					showScreenAd();
					break;
				case Constant.TO_PAID_V:
					topaid();
					break;
				}
			}
		};
	}

	@Override
	public void onPause() {
		UMGameAgent.onPause(this);
		super.onPause();
	}

	@Override
	public void onResume() {
		super.onResume();
		UMGameAgent.onResume(this);
	}

	private void share() {
		try {
			Intent shareIntent = new Intent(Intent.ACTION_SEND);
			Uri uri = Uri.fromFile(new File(Constant.shareImage));
			shareIntent.setType("image/*");
			shareIntent.putExtra(Intent.EXTRA_STREAM, uri);
			shareIntent.putExtra(Intent.EXTRA_TEXT, "来和我一起玩颜色战争吧！！");
			startActivity(Intent.createChooser(shareIntent, "分享"));

			MobclickAgent.onEvent(this, "Share");
		} catch (Exception e) {

		}
	}

	private void showScreenAd() {

	}

	private void topaid() {
		try {
			String url = "market://details?id="
					+ "com.colortheworld.colorwarpaid";
			Uri uri = Uri.parse(url);
			Intent it = new Intent(Intent.ACTION_VIEW, uri);
			colorWar.this.startActivity(it);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
