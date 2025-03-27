package com.vasyukov.laba_8_widget

import android.appwidget.AppWidgetManager
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.edit

class WidgetConfigActivity : AppCompatActivity() {

    private var appWidgetId = AppWidgetManager.INVALID_APPWIDGET_ID
    private lateinit var editTextUrl: EditText

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_widget_config)

        val intent = intent
        val extras = intent.extras
        if (extras != null) {
            appWidgetId = extras.getInt(
                AppWidgetManager.EXTRA_APPWIDGET_ID,
                AppWidgetManager.INVALID_APPWIDGET_ID
            )
        }

        editTextUrl = findViewById(R.id.editTextUrl)
        val prefs = getSharedPreferences("WidgetPrefs", MODE_PRIVATE)
        val currentUrl = prefs.getString("url_$appWidgetId", "https://yandex.ru")
        editTextUrl.setText(currentUrl)

        findViewById<Button>(R.id.buttonSave).setOnClickListener {
            val url = editTextUrl.text.toString()
            saveUrl(url)
            finishConfig()
        }
    }

    private fun saveUrl(url: String) {
        val prefs = getSharedPreferences("WidgetPrefs", MODE_PRIVATE)
        prefs.edit { putString("url_$appWidgetId", url) }

        val appWidgetManager = AppWidgetManager.getInstance(this)
        WidgetProvider().updateAppWidget(this, appWidgetManager, appWidgetId)
    }

    private fun finishConfig() {
        val resultValue = Intent()
        resultValue.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId)
        setResult(RESULT_OK, resultValue)
        finish()
    }
}