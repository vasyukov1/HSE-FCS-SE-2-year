package com.vasyukov.laba_08_widget

import android.appwidget.AppWidgetManager
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity

class WidgetConfigureActivity : AppCompatActivity() {

    private var appWidgetId = AppWidgetManager.INVALID_APPWIDGET_ID

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setResult(RESULT_CANCELED)

        setContentView(R.layout.widget_configure)

        val intent = intent
        val extras = intent.extras
        if (extras != null) {
            appWidgetId = extras.getInt(AppWidgetManager.EXTRA_APPWIDGET_ID, AppWidgetManager.INVALID_APPWIDGET_ID)
        }

        if (appWidgetId == AppWidgetManager.INVALID_APPWIDGET_ID) {
            finish()
            return
        }

        val urlField = findViewById<EditText>(R.id.urlField)
        findViewById<Button>(R.id.saveButton).setOnClickListener {
            val context = this@WidgetConfigureActivity
            val url = urlField.text.toString()

            saveUrl(context, appWidgetId, url)

            val appWidgetManager = AppWidgetManager.getInstance(context)
            MyWidgetProvider().onUpdate(context, appWidgetManager, intArrayOf(appWidgetId))

            val resultIntent = Intent().apply {
                putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId)
            }
            setResult(RESULT_OK, resultIntent)
            finish()
        }
    }

    private fun saveUrl(context: Context, appWidgetId: Int, url: String) {
        val prefs = context.getSharedPreferences(MyWidgetProvider.PREFS_NAME, Context.MODE_PRIVATE).edit()
        prefs.putString(MyWidgetProvider.PREF_PREFIX_KEY + appWidgetId, url)
        prefs.apply()
    }
}
