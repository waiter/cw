package com.colortheworld.colorwar;

public class j {

	public static void share(String url){
		Constant.shareImage = url;
		Constant.handler.sendEmptyMessage(Constant.ACTION_SHARE);
	}
	
	public static void showAd(){
		Constant.handler.sendEmptyMessage(Constant.SHOW_SCREEN_AD);
	}
	
	public static native void nsp1(String p);
}
