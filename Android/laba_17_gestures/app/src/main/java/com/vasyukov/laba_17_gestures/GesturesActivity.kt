package com.vasyukov.laba_17_gestures

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ObjectAnimator
import android.graphics.Color
import android.os.Bundle
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import android.view.animation.AccelerateInterpolator
import android.widget.FrameLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import kotlin.random.Random

class GesturesActivity : AppCompatActivity(), GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {

    private lateinit var gestureDetector: GestureDetector
    private lateinit var gestureInfo: TextView
    private lateinit var rootLayout: FrameLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_gestures)

        rootLayout = FrameLayout(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
            )
            setBackgroundColor(Color.WHITE)
        }
        setContentView(rootLayout)

        gestureInfo = TextView(this).apply {
            text = "Выполните жест"
            textSize = 24f
            setTextColor(Color.BLACK)
            layoutParams = FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT,
            ).apply {
                setMargins(50, 50, 50, 50)
            }
        }
        rootLayout.addView(gestureInfo)
        gestureDetector = GestureDetector(this, this)
        gestureDetector.setOnDoubleTapListener(this)
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        if (event != null) {
            createBallAnimation(event.x, event.y)
            return gestureDetector.onTouchEvent(event)
        } else {
            return super.onTouchEvent(event)
        }
    }

    override fun onDown(e: MotionEvent): Boolean {
//        updateGestureInfo("Касание (onDown)")
        return true
    }

    override fun onShowPress(e: MotionEvent) {
//        updateGestureInfo("Короткое нажатие")
    }

    override fun onDoubleTap(e: MotionEvent): Boolean {
        updateGestureInfo("Двойной тап")
        changeBackgroundColor()
        return true
    }

    override fun onSingleTapUp(e: MotionEvent): Boolean {
        updateGestureInfo("Одиночный тап")
        return true
    }

    override fun onScroll(p0: MotionEvent?, p1: MotionEvent, p2: Float, p3: Float): Boolean {
        updateGestureInfo("Прокрутка")
        showScrollIndicator(p2, p3)
        return true
    }

    override fun onLongPress(e: MotionEvent) {
        updateGestureInfo("Долгое нажатие")
    }

    override fun onFling(p0: MotionEvent?, p1: MotionEvent, p2: Float, p3: Float): Boolean {
        updateGestureInfo("Свайп")
        animateSwipe(p2)
        return true
    }

    override fun onSingleTapConfirmed(e: MotionEvent): Boolean {
//        updateGestureInfo("Подтвержденный одиночный тап (onSingleTapConfirmed)")
        return false
    }

    override fun onDoubleTapEvent(e: MotionEvent): Boolean {
        return false
    }

    private fun updateGestureInfo(gesture: String) {
        gestureInfo.text = gesture
    }

    private fun createBallAnimation(x: Float, y: Float) {
        val ballSize = 50
        val ball = View(this).apply {
            layoutParams = FrameLayout.LayoutParams(ballSize, ballSize)
            setBackgroundResource(R.drawable.ball)
            translationX = x - ballSize / 2f
            translationY = y - ballSize / 2f
        }
        rootLayout.addView(ball)

        ball.animate()
            .scaleX(2f)
            .scaleY(2f)
            .alpha(0f)
            .setDuration(600)
            .setInterpolator(AccelerateInterpolator())
            .setListener(object : AnimatorListenerAdapter() {
                override fun onAnimationEnd(animation: Animator) {
                    super.onAnimationEnd(animation)
                    rootLayout.removeView(ball)
                }
            })
            .start()
    }

    private fun changeBackgroundColor() {
        val rndColor = Color.rgb(Random.nextInt(256), Random.nextInt(256), Random.nextInt(256))
        rootLayout.setBackgroundColor(rndColor)
    }

    private fun animateSwipe(velocityX: Float) {
        val direction = if (velocityX > 0) 1 else -1
        val translation = 300f * direction
        ObjectAnimator.ofFloat(gestureInfo, "translationX", 0f, translation, 0f).apply {
            duration = 500
            start()
        }
    }

    private fun showScrollIndicator(distanceX: Float, distanceY: Float) {
        val indicator = View(this).apply {
            setBackgroundColor(Color.GRAY)
            layoutParams = FrameLayout.LayoutParams(
                if (Math.abs(distanceX) > Math.abs(distanceY)) 200 else 10,
                if (Math.abs(distanceX) > Math.abs(distanceY)) 10 else 200
            )
            x = gestureInfo.x + gestureInfo.width / 2f - 100
            y = gestureInfo.y + gestureInfo.height / 2f - 100
            alpha = 0.7f
        }
        rootLayout.addView(indicator)
        indicator.animate().alpha(0f).setDuration(600).withEndAction {
            rootLayout.removeView(indicator)
        }.start()
    }
}
