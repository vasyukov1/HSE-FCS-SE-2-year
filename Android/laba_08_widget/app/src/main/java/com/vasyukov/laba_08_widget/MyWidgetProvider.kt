package com.vasyukov.laba_08_widget

import android.app.PendingIntent
import android.appwidget.AppWidgetManager
import android.appwidget.AppWidgetProvider
import android.content.Context
import android.content.Intent
import android.widget.RemoteViews
import android.widget.Toast

class MyWidgetProvider : AppWidgetProvider() {

    companion object {
        const val ACTION_OPEN_WEBSITE = "com.example.mywidget.OPEN_WEBSITE"
        const val PREFS_NAME = "MyWidgetPrefs"
        const val PREF_PREFIX_KEY = "widget_url_"
    }

    override fun onUpdate(context: Context, appWidgetManager: AppWidgetManager, appWidgetIds: IntArray) {
        for (appWidgetId in appWidgetIds) {
            val url = getSavedUrl(context, appWidgetId) ?: "https://www.example.com"

            val intent = Intent(context, MyWidgetProvider::class.java).apply {
                action = ACTION_OPEN_WEBSITE
                putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId)
                putExtra("URL", url)
            }

            val pendingIntent = PendingIntent.getBroadcast(
                context, appWidgetId, intent, PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE
            )

            val views = RemoteViews(context.packageName, R.layout.widgetLayout).apply {
                setOnClickPendingIntent(R.id.openWebsiteButton, pendingIntent)
            }

            appWidgetManager.updateAppWidget(appWidgetId, views)
        }
    }

    override fun onReceive(context: Context, intent: Intent) {
        super.onReceive(context, intent)

        if (intent.action == ACTION_OPEN_WEBSITE) {
            val url = intent.getStringExtra("URL")
            if (!url.isNullOrEmpty()) {
                val browserIntent = Intent(Intent.ACTION_VIEW).apply {
                    data = android.net.Uri.parse(url)
                    flags = Intent.FLAG_ACTIVITY_NEW_TASK
                }
                context.startActivity(browserIntent)
            } else {
                Toast.makeText(context, "Адрес сайта не указан", Toast.LENGTH_SHORT).show()
            }
        }
    }

    private fun getSavedUrl(context: Context, appWidgetId: Int): String? {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        return prefs.getString(PREF_PREFIX_KEY + appWidgetId, null)
    }
}
