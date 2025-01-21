package com.vasyukov.laba_11_notification

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.work.Data
import androidx.work.OneTimeWorkRequest
import androidx.work.WorkManager

class ResultActivity : AppCompatActivity() {

    private lateinit var motivationalImage: ImageView
    private lateinit var newDesireButton: Button
    private lateinit var completedButton: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_result)

        motivationalImage = findViewById(R.id.motivationalImage)
        newDesireButton = findViewById(R.id.newDesireButton)
        completedButton = findViewById(R.id.completedButton)

        newDesireButton.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            startActivity(intent)
        }

        completedButton.setOnClickListener {
            startStarAnimation()

            val workRequest = OneTimeWorkRequest.Builder(NotificationWorker::class.java)
                .setInputData(
                    Data.Builder()
                        .putBoolean("isSuccess", true)
                        .build()
                )
                .build()

            WorkManager.getInstance(applicationContext).enqueue(workRequest)
        }
    }

    @SuppressLint("Recycle")
    private fun startStarAnimation() {
        val rootLayout = findViewById<ConstraintLayout>(R.id.rootLayout)
        val starCount = 70
        val starSize = 50

        for (i in 0 until starCount) {
            val starView = ImageView(this).apply {
                setImageResource(R.drawable.star)
                layoutParams = ConstraintLayout.LayoutParams(starSize, starSize)
            }

            val startX = (Math.random() * rootLayout.width).toFloat()
            val startY = -starSize.toFloat()

            starView.x = startX
            starView.y = startY

            rootLayout.addView(starView)

            val duration = (4000..10000).random().toLong()

            val fallAnimator = ObjectAnimator.ofFloat(starView, "translationY", rootLayout.height.toFloat())
            fallAnimator.duration = duration

            val driftAnimator = ObjectAnimator.ofFloat(starView, "translationX", startX, startX + (Math.random() * 200 - 100).toFloat())
            driftAnimator.duration = duration

            val alphaAnimation = ObjectAnimator.ofFloat(starView, "alpha", 1f, 0.5f, 1f)
            alphaAnimation.duration = 500
            alphaAnimation.repeatCount = ObjectAnimator.INFINITE
            alphaAnimation.start()

            AnimatorSet().apply {
                playTogether(fallAnimator, driftAnimator)
                start()
            }

            fallAnimator.addUpdateListener {
                if (fallAnimator.animatedFraction == 1f) {
                    rootLayout.removeView(starView)
                }
            }
        }
    }


}