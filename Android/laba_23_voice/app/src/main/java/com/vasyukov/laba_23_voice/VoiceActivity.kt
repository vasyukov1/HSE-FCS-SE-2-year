package com.vasyukov.laba_23_voice

import android.content.Intent
import android.os.Bundle
import android.speech.RecognizerIntent
import android.view.View
import android.widget.Button
import android.widget.ImageButton
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import java.util.*
import androidx.core.net.toUri

class VoiceActivity : AppCompatActivity() {

    private lateinit var resultTextView: TextView
    private lateinit var sendButton: Button
    private lateinit var voiceInputButton: ImageButton
    private var lastRecognizedText: String = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_voice)

        resultTextView = findViewById(R.id.resultTextView) ?: throw RuntimeException("TextView not found!")
        voiceInputButton = findViewById(R.id.voiceInputButton) ?: throw RuntimeException("Button not found!")
        sendButton = findViewById(R.id.sendButton) ?: throw RuntimeException("Send button not found!")

        voiceInputButton.setOnClickListener {
            startVoiceInput()
        }

        sendButton.setOnClickListener {
            sendToBrowser()
        }

        sendButton.visibility = View.GONE
    }

    private fun startVoiceInput() {
        val intent = Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH).apply {
            putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM)
            putExtra(RecognizerIntent.EXTRA_LANGUAGE, Locale.getDefault())
            putExtra(RecognizerIntent.EXTRA_PROMPT, "Говорите...")
        }

        try {
            startActivityForResult(intent, 100)
        } catch (e: Exception) {
            resultTextView.text = "Ошибка: ${e.localizedMessage}"
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 100 && resultCode == RESULT_OK) {
            data?.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS)?.firstOrNull()?.let {
                lastRecognizedText = it
                resultTextView.text = "Результат: $it"
                sendButton.visibility = View.VISIBLE
            }
        }
    }

    private fun sendToBrowser() {
        if (lastRecognizedText.isNotEmpty()) {
            val searchQuery = lastRecognizedText.replace(" ", "+")
            val searchUrl = "https://www.google.com/search?q=$searchQuery"

            val intent = Intent(Intent.ACTION_VIEW).apply {
                data = searchUrl.toUri()
            }

            try {
                startActivity(intent)
            } catch (e: Exception) {
                resultTextView.text = "Не удалось открыть браузер: ${e.localizedMessage}"
            }
        }
    }
}