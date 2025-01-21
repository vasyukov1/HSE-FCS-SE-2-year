package com.vasyukov.laba_11_notification

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.pm.PackageManager
import android.Manifest
import android.net.Uri
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.work.Data
import androidx.work.OneTimeWorkRequest
import androidx.work.WorkManager
import java.util.concurrent.TimeUnit

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var title: TextView
    private lateinit var desireInput: EditText
    private lateinit var submitButton: Button
    private lateinit var statusMessage: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.POST_NOTIFICATIONS) == PackageManager.PERMISSION_GRANTED) {
            createNotificationChannel()
        } else {
            requestPermissionLauncher.launch(Manifest.permission.POST_NOTIFICATIONS)
        }

        title = findViewById(R.id.title)
        desireInput = findViewById(R.id.desireInput)
        submitButton = findViewById(R.id.submitButton)
        statusMessage = findViewById(R.id.statusMessage)

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(title, desireInput, submitButton)
        } else {
            showViews(title, desireInput, submitButton)
        }

        submitButton.setOnClickListener {
            val desireText = desireInput.text.toString()
            if (desireText.isNotBlank()) {
                statusMessage.text = "Ваше желание отправлено во Вселенную!"
                statusMessage.visibility = TextView.VISIBLE

                val workRequest = OneTimeWorkRequest.Builder(NotificationWorker::class.java)
                    .setInitialDelay(3, TimeUnit.SECONDS)
                    .setInputData(
                        Data.Builder()
                            .putString("desire", desireText)
                            .putBoolean("isSuccess", false)
                            .build()
                    )
                    .build()

                WorkManager.getInstance(applicationContext).enqueue(workRequest)
            } else {
                statusMessage.text = "Пожалуйста, введите желание."
                statusMessage.visibility = TextView.VISIBLE
            }
        }
    }

    private fun createNotificationChannel() {
        val channel = NotificationChannel(
            "desire_channel",
            "Desire Notifications",
            NotificationManager.IMPORTANCE_HIGH
        ).apply {
            description = "Notifications for your desires"
            val soundUri = Uri.parse("android.resource://com.vasyukov.laba_11_notification/raw/success_notification_sound")
            setSound(soundUri, Notification.AUDIO_ATTRIBUTES_DEFAULT)
        }
        val notificationManager: NotificationManager = getSystemService(NOTIFICATION_SERVICE) as NotificationManager
        notificationManager.createNotificationChannel(channel)
    }

    private val requestPermissionLauncher = registerForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted: Boolean ->
        if (isGranted) {
            createNotificationChannel()
        } else {
            Log.e("MainActivity", "Доступ к уведомлениям запрещён")
        }
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