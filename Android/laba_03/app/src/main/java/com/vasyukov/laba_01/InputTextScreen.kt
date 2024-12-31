package com.vasyukov.laba_01

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity

class InputTextScreen : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_input_text_screen)

        val enteredText = intent.getStringExtra("enteredText")
        val editText = findViewById<TextView>(R.id.editText)
        val backButton = findViewById<Button>(R.id.backButton)
        val showText = findViewById<TextView>(R.id.showText)

        editText.text = enteredText

        backButton.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            intent.putExtra("enteredText", enteredText)
            startActivity(intent)
            overridePendingTransition(0, 0)
        }
    }
}