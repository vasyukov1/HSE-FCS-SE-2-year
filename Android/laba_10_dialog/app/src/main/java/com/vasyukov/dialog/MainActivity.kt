package com.vasyukov.dialog

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.view.animation.Animation
import android.view.animation.TranslateAnimation
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import nl.dionsegijn.konfetti.core.Party
import nl.dionsegijn.konfetti.core.Position
import nl.dionsegijn.konfetti.core.emitter.Emitter
import nl.dionsegijn.konfetti.xml.KonfettiView
import java.util.concurrent.TimeUnit

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var questionText: TextView
    private lateinit var inputCode: EditText
    private lateinit var sendButton: Button
    private lateinit var hintText: TextView
    private lateinit var confettiView: KonfettiView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        questionText = findViewById(R.id.questionText)
        inputCode = findViewById(R.id.inputCode)
        sendButton = findViewById(R.id.sendButton)
        hintText = findViewById(R.id.hintText)
        confettiView = findViewById(R.id.confettiView)

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(questionText, inputCode, sendButton)
        } else {
            showViews(questionText, inputCode, sendButton)
        }

        sendButton.setOnClickListener {
            val userInput = inputCode.text.toString()

            if (validateInput(userInput)) {
                hintText.visibility = TextView.GONE
                showSuccessDialog()
            } else {
                hintText.visibility = TextView.VISIBLE
                showErrorDialog()
            }
        }
    }

    private fun validateInput(input: String): Boolean {
        val teacherNames = listOf(
            "макаров",
            "сергей",
            "сергей львович",
            "макаров сергей львович",
            "макаров сергей")
        return teacherNames.contains(input.lowercase())
    }

    private fun showSuccessDialog() {
        confettiView.start(
            Party(
                emitter = Emitter(duration = 3, TimeUnit.SECONDS).max(500),
                position = Position.Relative(0.5, 0.0),
                spread = 360,
                colors = listOf(Color.RED, Color.GREEN, Color.BLUE, Color.YELLOW)
            )
        )
        AlertDialog.Builder(this)
            .setTitle(getString(R.string.dialog_success_title))
            .setMessage(getString(R.string.dialog_success_message))
            .setPositiveButton(getString(R.string.dialog_button_ok)) { dialog, _ ->
                dialog.dismiss()
            }
            .show()
    }

    private fun showErrorDialog() {
        val originalBackgroundInputCode = inputCode.background
        val originalBackgroundSendButton = sendButton.background

        inputCode.setBackgroundResource(R.drawable.error_background)
        sendButton.setBackgroundResource(R.drawable.error_background)

        val shakeAnimation = TranslateAnimation(-10f, 10f, 0f, 0f).apply {
            duration = 50
            repeatMode = Animation.REVERSE
            repeatCount = 5
        }

        inputCode.startAnimation(shakeAnimation)
        sendButton.startAnimation(shakeAnimation)

        AlertDialog.Builder(this)
            .setTitle(getString(R.string.dialog_error_title))
            .setMessage(getString(R.string.dialog_error_message))
            .setPositiveButton(getString(R.string.dialog_button_ok)) { dialog, _ ->
                dialog.dismiss()

                inputCode.background = originalBackgroundInputCode
                sendButton.background = originalBackgroundSendButton
            }
            .show()
    }

    private fun showViews(vararg views: View) {
        views.forEach {
            it.visibility = View.VISIBLE
            ObjectAnimator.ofFloat(it, "alpha", 0f, 1f).apply {
                duration = 0
                start()
            }
        }
    }

    private fun startAnimation(vararg views: View) {
        val fioLabel = findViewById<TextView>(R.id.fioLabel)
        val groupLabel = findViewById<TextView>(R.id.groupLabel)
        val avatarImage = findViewById<ImageView>(R.id.avatarImage)

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
            val moveFioDown = ObjectAnimator.ofFloat(fioLabel, "alpha", 1f, 0f).apply {
                duration = 1000
            }
            val moveGroupDown = ObjectAnimator.ofFloat(groupLabel, "alpha", 1f, 0f).apply {
                duration = 1000
            }

            fadeOutAvatar.addUpdateListener {
                if (it.animatedFraction == 1f) {
                    avatarImage.visibility = ImageView.GONE
                    fioLabel.visibility = TextView.GONE
                    groupLabel.visibility = TextView.GONE

                    showViews(views[0], views[1], views[2])
                    listOf(fioLabel, groupLabel).forEach { element ->
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
    }
}