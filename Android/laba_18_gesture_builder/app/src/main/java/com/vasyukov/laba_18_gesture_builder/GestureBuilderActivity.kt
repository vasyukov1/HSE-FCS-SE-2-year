package com.vasyukov.laba_18_gesture_builder

import android.gesture.GestureLibraries
import android.gesture.GestureOverlayView
import android.gesture.Prediction
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.Gravity
import android.view.animation.AccelerateInterpolator
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import kotlin.random.Random

class GestureBuilderActivity : AppCompatActivity() {

    private lateinit var gestureInfo: TextView
    private lateinit var rootLayout: FrameLayout
    private val handler = Handler(Looper.getMainLooper())

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        rootLayout = FrameLayout(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
            setBackgroundColor(Color.WHITE)
        }
        setContentView(rootLayout)

        gestureInfo = TextView(this).apply {
            text = "Выполните жест"
            textSize = 24f
            setTextColor(Color.BLACK)
            setPadding(20, 50, 20, 50)
            gravity = Gravity.CENTER
        }
        rootLayout.addView(gestureInfo)

        val gestureOverlayView = GestureOverlayView(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
            gestureStrokeType = GestureOverlayView.GESTURE_STROKE_TYPE_MULTIPLE
            gestureStrokeWidth = 10f
        }
        rootLayout.addView(gestureOverlayView)

        val gestureLibrary = GestureLibraries.fromRawResource(this, R.raw.gestures)
        if (!gestureLibrary.load()) {
            gestureInfo.text = "Ошибка загрузки жестов"
        }

        gestureOverlayView.addOnGesturePerformedListener { _, gesture ->
            val predictions: ArrayList<Prediction> = gestureLibrary.recognize(gesture)
            if (predictions.isNotEmpty() && predictions[0].score > 2.0) {
                val recognizedGesture = predictions[0].name
                updateGestureInfo("Распознан жест: $recognizedGesture")
                animateButterflies()
                changeBackgroundTemporary(Color.GREEN)
            } else {
                updateGestureInfo("Жест не распознан")
                changeBackgroundTemporary(Color.RED)
            }
        }
    }

    private fun updateGestureInfo(text: String) {
        gestureInfo.text = text
    }

    private fun animateButterflies() {
        repeat(5) {
            val butterfly = ImageView(this).apply {
                setImageResource(R.drawable.butterfly)
                layoutParams = FrameLayout.LayoutParams(100, 100)
                x = Random.nextInt(rootLayout.width).toFloat()
                y = Random.nextInt(rootLayout.height).toFloat()
                alpha = 0f
            }
            rootLayout.addView(butterfly)

            butterfly.animate()
                .alpha(1f)
                .setDuration(500)
                .setInterpolator(AccelerateInterpolator())
                .withEndAction {
                    butterfly.animate()
                        .alpha(0f)
                        .setDuration(1500)
                        .withEndAction { rootLayout.removeView(butterfly) }
                        .start()
                }
                .start()
        }
    }

    private fun changeBackgroundTemporary(color: Int) {
        rootLayout.setBackgroundColor(color)
        handler.postDelayed({ rootLayout.setBackgroundColor(Color.WHITE) }, 2000)
    }
}