package com.vasyukov.laba_18_gesture_builder

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    private lateinit var fioLabel: TextView
    private lateinit var groupLabel: TextView
    private lateinit var avatarImage: ImageView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        fioLabel = findViewById(R.id.fioLabel)
        groupLabel = findViewById(R.id.groupLabel)
        avatarImage = findViewById(R.id.avatarImage)

        listOf(fioLabel, groupLabel, avatarImage).forEach { view ->
            ObjectAnimator.ofFloat(view, "alpha", 0f, 1f).apply {
                duration = 1000
                start()
            }
        }

        Handler(Looper.getMainLooper()).postDelayed({
            fadeOutAndNavigate()
        }, 2000)
    }

    private fun fadeOutAndNavigate() {
        val fadeOutAvatar = ObjectAnimator.ofFloat(avatarImage, "alpha", 1f, 0f).apply {
            duration = 1000
        }
        val fadeOutFio = ObjectAnimator.ofFloat(fioLabel, "alpha", 1f, 0f).apply {
            duration = 1000
        }
        val fadeOutGroup = ObjectAnimator.ofFloat(groupLabel, "alpha", 1f, 0f).apply {
            duration = 1000
        }

        fadeOutAvatar.addUpdateListener {
            if (it.animatedFraction == 1f) {
                avatarImage.visibility = ImageView.GONE
                fioLabel.visibility = TextView.GONE
                groupLabel.visibility = TextView.GONE
            }
        }

        val intent = Intent(this, GestureBuilderActivity::class.java)

        AnimatorSet().apply {
            playTogether(fadeOutAvatar, fadeOutFio, fadeOutGroup)
            start()
            startActivity(intent)
            overridePendingTransition(0, 0)
        }
    }
}
