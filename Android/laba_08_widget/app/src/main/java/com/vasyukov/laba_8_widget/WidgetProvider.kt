package com.vasyukov.laba_8_widget

import android.app.PendingIntent
import android.appwidget.AppWidgetManager
import android.appwidget.AppWidgetProvider
import android.content.Context
import android.content.Intent
import android.widget.RemoteViews
import androidx.core.net.toUri

class WidgetProvider : AppWidgetProvider() {

    override fun onUpdate(
        context: Context,
        appWidgetManager: AppWidgetManager,
        appWidgetIds: IntArray,
    ) {
        for (appWidgetId in appWidgetIds) {
            updateAppWidget(context, appWidgetManager, appWidgetId)
        }
    }

    internal fun updateAppWidget(
        context: Context,
        appWidgetManager: AppWidgetManager,
        appWidgetId: Int,
    ) {
        val views = RemoteViews(context.packageName, R.layout.widget_layout)

        val prefs = context.getSharedPreferences("WidgetPrefs", Context.MODE_PRIVATE)
        val url = prefs.getString("url_$appWidgetId", "https://yandex.ru")

        val openUrlIntent = Intent(Intent.ACTION_VIEW, url?.toUri())
        val pendingOpenUrlIntent = PendingIntent.getActivity(context, 0, openUrlIntent, PendingIntent.FLAG_IMMUTABLE)
        views.setOnClickPendingIntent(R.id.widget_button, pendingOpenUrlIntent)

        val editIntent = Intent(context, WidgetConfigActivity::class.java)
        editIntent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId)
        val pendingEditIntent = PendingIntent.getActivity(context, appWidgetId, editIntent, PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_UPDATE_CURRENT)
        views.setOnClickPendingIntent(R.id.widget_edit_button, pendingEditIntent)

        appWidgetManager.updateAppWidget(appWidgetId, views)
    }
}