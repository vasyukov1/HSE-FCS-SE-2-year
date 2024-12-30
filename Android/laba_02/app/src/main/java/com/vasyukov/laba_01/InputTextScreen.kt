package com.vasyukov.laba_01

import android.content.Context
import android.inputmethodservice.InputMethodService
import android.os.Bundle
import android.view.KeyEvent
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity

class InputTextScreen : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_input_text_screen)

        val editText = findViewById<EditText>(R.id.editText)
        val sentTextButton = findViewById<Button>(R.id.sentText)
        val showText = findViewById<TextView>(R.id.showText)

        fun sendText() {
            val enteredText = editText.text.toString()

            if (enteredText.isBlank()) {
                editText.setError("Сообщение не может быть пустым")
                return
            }

            showText.text = enteredText
            editText.setText("")

            val imm = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(editText.windowToken, 0)
        }

        sentTextButton.setOnClickListener {
            sendText()
        }

        editText.setOnKeyListener { v, keyCode, event ->
            if (keyCode == KeyEvent.KEYCODE_ENTER && event.action == KeyEvent.ACTION_DOWN) {
                sendText()
                true
            } else {
                false
            }
        }
    }
}