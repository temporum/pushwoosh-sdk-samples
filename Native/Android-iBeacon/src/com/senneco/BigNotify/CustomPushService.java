package com.senneco.BigNotify;

import android.content.Context;
import android.content.Intent;

import com.arellomobile.android.push.PushGCMIntentService;

/**
 * Date: 14.08.13
 * Time: 18:35
 *
 * @author Yuri Shmakov
 */
public class CustomPushService extends PushGCMIntentService
{
	@Override
	protected void onMessage(Context context, Intent intent)
	{
		// write this code line if your want show notification too
		super.onMessage(context, intent);

		Intent launchIntent = context.getPackageManager().getLaunchIntentForPackage(getPackageName());
		//startActivity(launchIntent);
	}
}
