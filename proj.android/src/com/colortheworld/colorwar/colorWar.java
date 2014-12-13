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
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.game.UMGameAgent;
import com.vungle.publisher.VunglePub;

public class colorWar extends Cocos2dxActivity {
	private final String ADMOB_ID = "a153a560efc5136";
	private final String VUNGLE_ID = "548be2d740fabd4201000010";

	static {
		System.loadLibrary("game");
	}
	private AdView adView;
	private InterstitialAd interstitial;

	final VunglePub vunglePub = VunglePub.getInstance();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		adView = new AdView(colorWar.this);
		adView.setAdUnitId(ADMOB_ID);
		adView.setAdSize(AdSize.BANNER);
		RelativeLayout layout = (RelativeLayout) findViewById(R.id.ad_wrapper);
		layout.addView(adView);

		AdRequest adRequest = new AdRequest.Builder().build();
		adView.loadAd(adRequest);

		layout.setVisibility(View.VISIBLE);
		adView.setVisibility(View.VISIBLE);

		interstitial = new InterstitialAd(this);
		interstitial.setAdUnitId(ADMOB_ID);
		AdRequest adRequest1 = new AdRequest.Builder().build();
		interstitial.loadAd(adRequest1);

		vunglePub.init(this, VUNGLE_ID);

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
				// j.nsa(false);
			}
		});
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
		if (adView != null)
			adView.pause();
		UMGameAgent.onPause(this);
		vunglePub.onPause();
		super.onPause();
	}

	@Override
	public void onResume() {
		super.onResume();
		UMGameAgent.onResume(this);
		vunglePub.onResume();
		if (adView != null)
			adView.resume();
	}

	@Override
	public void onDestroy() {
		if (adView != null)
			adView.destroy();
		super.onDestroy();
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
		if (vunglePub.isCachedAdAvailable()) {
			vunglePub.playAd();
		} else {
			if (interstitial.isLoaded()) {
				interstitial.show();
			}
		}
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
