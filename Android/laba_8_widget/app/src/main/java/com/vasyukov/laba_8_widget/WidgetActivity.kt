package com.vasyukov.laba_8_widget

import android.appwidget.AppWidgetManager
import android.content.ComponentName
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import androidx.activity.ComponentActivity

class WidgetActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_widget)

        val configureButton = findViewById<Button>(R.id.configure_button)
        configureButton.setOnClickListener {
            val intent = Intent(this, WidgetConfigActivity::class.java)

            val appWidgetManager = AppWidgetManager.getInstance(this)
            val appWidgetIds = appWidgetManager.getAppWidgetIds(ComponentName(this, WidgetProvider::class.java))

            if (appWidgetIds.isNotEmpty()) {
                intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetIds[0])
            }

            startActivity(intent)
        }
    }
}