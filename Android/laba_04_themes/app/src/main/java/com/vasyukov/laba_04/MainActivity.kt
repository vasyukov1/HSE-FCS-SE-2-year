package com.vasyukov.laba_04

import android.R.attr.height
import android.R.attr.width
import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.SharedPreferences
import android.content.res.Configuration
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var sharedPreferences: SharedPreferences
    private val PREFS_NAME = "AppPreferences"
    private val KEY_IS_DARK_MODE = "isDarkMode"

    private lateinit var changedModeButton: Button

    private lateinit var snakeView: SnakeView
    private val handler = Handler(Looper.getMainLooper())


    override fun onCreate(savedInstanceState: Bundle?) {
        sharedPreferences = getSharedPreferences(PREFS_NAME, MODE_PRIVATE)
        val currentDarkMode = resources.configuration.uiMode and Configuration.UI_MODE_NIGHT_MASK
        val isSystemDarkMode = currentDarkMode == Configuration.UI_MODE_NIGHT_YES
        val isDarkMode = sharedPreferences.getBoolean(KEY_IS_DARK_MODE, isSystemDarkMode)
        setMode(isDarkMode)

        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        changedModeButton = findViewById(R.id.changedModeButton)
        snakeView = findViewById(R.id.snakeView)
        handler.post(object : Runnable {
            override fun run() {
                snakeView.updateSnake(isDarkMode)
                handler.postDelayed(this, 100)
            }
        })

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(changedModeButton, snakeView)
        } else {
            showViews(changedModeButton, snakeView)
        }

        changedModeButton.setOnClickListener {
            val newIsDarkMode = !isDarkMode
            sharedPreferences.edit().putBoolean(KEY_IS_DARK_MODE, newIsDarkMode).apply()
            snakeView.updateSnakeColor(newIsDarkMode)
            setMode(newIsDarkMode)
        }
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)

        val parcelableSegments = snakeView.snakeSegments.map { SnakeSegment(it.first, it.second) }
        outState.putParcelableArrayList("snakeSegments", ArrayList(parcelableSegments))
        outState.putIntArray("snakeColors", snakeView.snakeColors.toIntArray())
        outState.putFloat("headX", snakeView.headX)
        outState.putFloat("headY", snakeView.headY)
        outState.putDouble("directionAngle", snakeView.directionAngle)
    }

    override fun onRestoreInstanceState(savedInstanceState: Bundle) {
        super.onRestoreInstanceState(savedInstanceState)

        val restoredSegments = savedInstanceState.getParcelableArrayList<SnakeSegment>("snakeSegments")
        if (restoredSegments != null) {
            snakeView.snakeSegments.clear()
            snakeView.snakeSegments.addAll(restoredSegments.map { Pair(it.x, it.y) })
        }

        val restoredColors = savedInstanceState.getIntArray("snakeColors")
        if (restoredColors != null) {
            snakeView.snakeColors.clear()
            snakeView.snakeColors.addAll(restoredColors.toList())
        }

        snakeView.headX = savedInstanceState.getFloat("headX", width / 2f)
        snakeView.headY = savedInstanceState.getFloat("headY", height / 2f)
        snakeView.directionAngle = savedInstanceState.getDouble("directionAngle", 0.0)
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun setMode(isDarkMode: Boolean) {
        AppCompatDelegate.setDefaultNightMode(
            if (isDarkMode) {
                AppCompatDelegate.MODE_NIGHT_YES
            } else {
                AppCompatDelegate.MODE_NIGHT_NO
            }
        )
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

                    showViews(views[0], views[1])
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