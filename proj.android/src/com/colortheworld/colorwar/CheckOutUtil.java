package com.colortheworld.colorwar;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;

import com.colortheworld.colorwar.payment.IabHelper;
import com.colortheworld.colorwar.payment.IabResult;
import com.colortheworld.colorwar.payment.Inventory;
import com.colortheworld.colorwar.payment.Purchase;

public class CheckOutUtil {
	public static final String TAG = "Checkout";

	// TODO Need official key for each application
	public static String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmCuG5z/2Cwmp9cmIyU5rltqra3kNfxuWFqP/WVGkGhsDwKT8MZXjmh7LV5gvETbRmvmFRTeAeUKZzmaC37QShR6XxGwlHlsnGVbSMAJdDcHih7rES0/aHXC3/RH6njPMPacl3v9a+0pVI+bl/eUTNPJk3r+w3TzcQPM0+3SE3H2Hrrfoxq5WON2rKLxd+mq1lQgOjYiC0dU75VVqixd2vVsptZG1NEEqjxZTqwX1lHWj3StJA0Zd0CeYKkZSLvqEIMVwMgkMXdG8qUYUkZ9r1VfyHXPEmTDKguF/QMjME2nXYxNr1eGDNFRMKZe0qaU1UoLITViXlRz4jMWcUzQmSwIDAQAB";

	public static final String PAYLOAD = "";

	// (arbitrary) request code for the purchase flow
	public static final int RC_REQUEST = 10001;
	public static IabHelper iabHelper;

	public static String[] IAP_IDs;
	public static PurchaseCompleteLinster purchaseCompleteLister;
	public static boolean isCheckOutV3Support = true;

	// Listener that's called when we finish querying the items and
	// subscriptions we own
	static IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {

		@Override
		public void onQueryInventoryFinished(IabResult result, Inventory inv) {
			if (result.isFailure()) {
				complain("Failed to query inventory: " + result);
				return;
			}

			/*
			 * Check for items we own. Notice that for each purchase, we check
			 * the developer payload to see if it's correct! See
			 * verifyDeveloperPayload().
			 */

			// Check for product delivery -- if we own product, we should
			// consume it immediately

			for (String id : IAP_IDs) {
				Purchase purchase = inv.getPurchase(id);
				if (purchase != null && verifyDeveloperPayload(purchase)) {
					iabHelper.consumeAsync(purchase, mConsumeFinishedListener);
					return;
				}
			}

		}
	};

	// Callback for when a purchase is finished
	static IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {

		@Override
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			if (result.isFailure()) {
				if (purchaseCompleteLister != null) {
					purchaseCompleteLister.payFail();
				}
				complain("Error purchasing: " + result);
				return;
			}
			if (!verifyDeveloperPayload(purchase)) {
				if (purchaseCompleteLister != null) {
					purchaseCompleteLister.payFail();
				}
				complain("Error purchasing. Authenticity verification failed.");
				return;
			}

			Log.d(TAG, "Purchase successful.");

			for (String id : IAP_IDs) {
				if (purchase.getSku().equals(id)) {
					iabHelper.consumeAsync(purchase, mConsumeFinishedListener);
					break;
				}
			}

		}
	};

	// Called when consumption is complete
	static IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {

		@Override
		public void onConsumeFinished(Purchase purchase, IabResult result) {
			if (result.isSuccess()) {
				// successfully consumed, so we apply the effects of the item in
				// our
				// add to game data
				String id = purchase.getSku();
				int index = 0;
				for (int i = 0; i < IAP_IDs.length; i++) {
					if (id.equals(IAP_IDs[i])) {
						index = i;
						break;
					}
				}
				if (purchaseCompleteLister != null) {
					purchaseCompleteLister.paySuccess(index,
							purchase.getOrderId());
				}
			} else {
				if (purchaseCompleteLister != null) {
					purchaseCompleteLister.payFail();
				}
				complain("Error while consuming: " + result);
			}
		}
	};

	public static void initCheckOut(Context ctx, String[] IDs,
			PurchaseCompleteLinster linster) {
		purchaseCompleteLister = linster;
		IAP_IDs = IDs;
		iabHelper = new IabHelper(ctx, base64EncodedPublicKey);

		// enable debug logging (for a production application, you should set
		// this to false).
		iabHelper.enableDebugLogging(false);

		// Start setup. This is asynchronous and the specified listener
		// will be called once setup completes.
		iabHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			public void onIabSetupFinished(IabResult result) {
				if (!result.isSuccess()) {
					// Oh no, there was a problem.
					isCheckOutV3Support = false;
					complain(result.getMessage());
					return;
				}

				// Hooray, IAB is fully set up. Now, let's get an inventory of
				// stuff we own.
				iabHelper.queryInventoryAsync(mGotInventoryListener);
			}
		});
	}

	public static void startPurchase(Activity ac, String sku) {
		if (isCheckOutV3Support) {
			try {
				iabHelper.launchPurchaseFlow(ac, sku, RC_REQUEST,
						mPurchaseFinishedListener);
			} catch (Exception e) {
				e.printStackTrace();
				if (purchaseCompleteLister != null) {
					purchaseCompleteLister.payFail();
				}
			}
		} else {
			showInfoDialog(ac, R.string.felpay_billing_not_supported_message);
		}
	}

	public static boolean handleActivityResult(int requestCode, int resultCode,
			Intent data) {
		return iabHelper.handleActivityResult(requestCode, resultCode, data);
	}

	static void complain(String message) {
		Log.e(TAG, "**** CheckOut Error: " + message);
	}

	/** Verifies the developer payload of a purchase. */
	static boolean verifyDeveloperPayload(Purchase p) {
		@SuppressWarnings("unused")
		String payload = p.getDeveloperPayload();

		/*
		 * TODO: verify that the developer payload of the purchase is correct.
		 * It will be the same one that you sent when initiating the purchase.
		 * 
		 * WARNING: Locally generating a random string when starting a purchase
		 * and verifying it here might seem like a good approach, but this will
		 * fail in the case where the user purchases an item on one device and
		 * then uses your app on a different device, because on the other device
		 * you will not have access to the random string you originally
		 * generated.
		 * 
		 * So a good developer payload has these characteristics:
		 * 
		 * 1. If two different users purchase an item, the payload is different
		 * between them, so that one user's purchase can't be replayed to
		 * another user.
		 * 
		 * 2. The payload must be such that you can verify it even when the app
		 * wasn't the one who initiated the purchase flow (so that items
		 * purchased by the user on one device work on other devices owned by
		 * the user).
		 * 
		 * Using your own server to store and verify developer payloads across
		 * app installations is recommended.
		 */

		return true;
	}

	private static void showInfoDialog(Context context, int msgId) {
		showInfoDialog(context, context.getString(msgId));
	}

	private static void showInfoDialog(Context context, String msg) {
		AlertDialog.Builder builder = new AlertDialog.Builder(context);
		builder.setIcon(R.drawable.icon);
		builder.setTitle("info");
		builder.setMessage(msg);
		builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				dialog.dismiss();
			}
		});
		builder.show();
	}
}
