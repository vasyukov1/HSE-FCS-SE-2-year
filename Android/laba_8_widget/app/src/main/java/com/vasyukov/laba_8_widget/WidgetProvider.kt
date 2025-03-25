package com.vasyukov.laba_8_widget

import android.app.PendingIntent
import android.appwidget.AppWidgetManager
import android.appwidget.AppWidgetProvider
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.widget.RemoteViews
import androidx.core.net.toUri

class WidgetProvider : AppWidgetProvider() {

    override fun onUpdate(context: Context, appWidgetManager: AppWidgetManager, appWidgetIds: IntArray) {
        super.onUpdate(context, appWidgetManager, appWidgetIds)

        for (appWidgetId in appWidgetIds) {
            val remoteViews = RemoteViews(context.packageName, R.layout.layout_widget)

            val userUrl = getWidgetUrl(context)
            val cleanedUrl = cleanUrl(userUrl)

            remoteViews.setTextViewText(R.id.widgetSiteName, cleanedUrl)

            val intent = Intent(Intent.ACTION_VIEW, userUrl.toUri())
            val pendingIntent = PendingIntent.getActivity(
                context,
                0,
                intent,
                PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE
            )
            remoteViews.setOnClickPendingIntent(R.id.widgetSiteName, pendingIntent)

            appWidgetManager.updateAppWidget(appWidgetId, remoteViews)
        }
    }

    private fun getWidgetUrl(context: Context): String {
        val prefs = context.getSharedPreferences("WidgetPrefs", Context.MODE_PRIVATE)
        return prefs.getString("url", "https://www.google.com") ?: "https://www.google.com"
    }

    private fun cleanUrl(url: String): String {
        var cleanedUrl = url.replaceFirst("https://", "")
        cleanedUrl = cleanedUrl.replaceFirst("www.", "")

        val extensions = listOf(".com", ".ru", ".org", ".net", ".edu", "/")
        for (ext in extensions) {
            if (cleanedUrl.endsWith(ext)) {
                cleanedUrl = cleanedUrl.substring(0, cleanedUrl.length - ext.length)
                break
            }
        }

        return cleanedUrl.capitalize()
    }
}
