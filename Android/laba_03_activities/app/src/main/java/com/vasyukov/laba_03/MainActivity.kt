package com.vasyukov.laba_03

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.KeyEvent
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        var enteredText: String? = intent.getStringExtra("enteredText")

        val fioLabel = findViewById<TextView>(R.id.fioLabel)
        val groupLabel = findViewById<TextView>(R.id.groupLabel)
        val avatarImage = findViewById<ImageView>(R.id.avatarImage)
        val nextButton = findViewById<Button>(R.id.nextButton)
        val editText = findViewById<EditText>(R.id.editText)
        val sendButton = findViewById<Button>(R.id.sendButton)
        val pageName = findViewById<TextView>(R.id.pageName)

        if (isFirstLoad) {
            listOf(fioLabel, groupLabel, avatarImage).forEach { view ->
                ObjectAnimator.ofFloat(view, "alpha", 0f, 1f).apply {
                    duration = 1000
                    start()
                }
            }

            Handler(Looper.getMainLooper()).postDelayed({
                val fadeOutAvatar = ObjectAnimator.ofFloat(avatarImage, "alpha", 1f, 0f).apply {
                    duration = 1000
                }

                val moveFioDown = ObjectAnimator.ofFloat(fioLabel, "translationY", 0f, 500f).apply {
                    duration = 1000
                }
                val moveGroupDown = ObjectAnimator.ofFloat(groupLabel, "translationY", 0f, 500f).apply {
                    duration = 1000
                }
                fadeOutAvatar.addUpdateListener {
                    if (it.animatedFraction == 1f) {
                        avatarImage.visibility = ImageView.GONE
                        editText.visibility = EditText.VISIBLE
                        sendButton.visibility = Button.VISIBLE
                        pageName.visibility = TextView.VISIBLE

                        listOf(editText, sendButton, nextButton, pageName).forEach { element ->
                            ObjectAnimator.ofFloat(element, "alpha", 0f, 1f).apply {
                                duration = 1000
                                start()
                            }
                        }
                    }
                }

                AnimatorSet().apply {
                    playTogether(fadeOutAvatar, moveFioDown, moveGroupDown)
                    start()
                }
            }, 2000)
        } else {
            avatarImage.visibility = ImageView.GONE
            editText.visibility = EditText.VISIBLE
            sendButton.visibility = Button.VISIBLE
            pageName.visibility = TextView.VISIBLE

            listOf(editText, sendButton, nextButton, pageName).forEach { element ->
                ObjectAnimator.ofFloat(element, "alpha", 0f, 1f).apply {
                    duration = 0
                    start()
                }
            }
        }

        isFirstLoad = false

        fun sendText() {
            enteredText = editText.text.toString()

            if (enteredText.isNullOrBlank()) {
                editText.setError("Сообщение не может быть пустым")
                return
            }

            editText.setText("")
            editText.hint = "Сообщение отправлено!"

            val imm = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(editText.windowToken, 0)
        }

        sendButton.setOnClickListener {
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

        nextButton.setOnClickListener {
            val intent = Intent(this, InputTextScreen::class.java)
            intent.putExtra("enteredText", enteredText)
            startActivity(intent)
            overridePendingTransition(0, 0)
        }
    }
}