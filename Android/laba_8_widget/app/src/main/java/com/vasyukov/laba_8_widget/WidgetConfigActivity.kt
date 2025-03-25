package com.vasyukov.laba_8_widget

import android.appwidget.AppWidgetManager
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.edit

class WidgetConfigActivity : AppCompatActivity() {

    private lateinit var editTextUrl: EditText
    private lateinit var buttonSave: Button
    private var appWidgetId = AppWidgetManager.INVALID_APPWIDGET_ID

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val intent = intent
        appWidgetId = intent.getIntExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, AppWidgetManager.INVALID_APPWIDGET_ID)

        setContentView(R.layout.activity_widget_config)

        editTextUrl = findViewById(R.id.editTextUrl)
        buttonSave = findViewById(R.id.buttonSave)

        buttonSave.setOnClickListener {
            val url = editTextUrl.text.toString()
            val prefs = getSharedPreferences("WidgetPrefs", MODE_PRIVATE)
            prefs.edit { putString("url", url) }

            val resultValue = Intent()
            resultValue.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId)
            setResult(RESULT_OK, resultValue)
            finish()
        }
    }
}
