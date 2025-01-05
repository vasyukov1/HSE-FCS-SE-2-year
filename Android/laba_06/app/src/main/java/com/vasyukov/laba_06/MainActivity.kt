package com.vasyukov.laba_06

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.graphics.Color
import android.media.MediaPlayer
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.text.SpannableString
import android.text.Spanned
import android.text.style.ClickableSpan
import android.text.method.LinkMovementMethod
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import kotlin.random.Random

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    val colors = mapOf(
        'У' to Color.RED,
        'Б' to Color.GREEN,
        'Н' to Color.YELLOW,
        'Р' to Color.BLUE,
        'Д' to Color.CYAN,
        'И' to Color.GREEN,
        'С' to Color.MAGENTA,
        'К' to Color.GRAY,
        'О' to Color.BLUE,
        'Т' to Color.DKGRAY,
        'Е' to Color.LTGRAY,
        'К' to Color.GRAY,
        'А' to Color.WHITE,
        'Г' to Color.BLUE,
        'В' to Color.CYAN,
        'Х' to Color.YELLOW,
        'М' to Color.WHITE,

        'A' to Color.BLACK,
        'N' to Color.CYAN,
        'D' to Color.BLUE,
        'R' to Color.YELLOW,
        'O' to Color.MAGENTA,
        'I' to Color.DKGRAY,
        'S' to Color.RED,

        '2' to Color.RED,
        '0' to Color.GREEN,
        '5' to Color.BLUE,
    )

    val words = listOf(
        "ДИСКОТЕКА",
        "РАДУГА",
        "ТУБЕСАН",
        "СВЕРХРАЗУМ",
        "ANDROID",
        "2025",
        "SOS"
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        val animatedTextView = findViewById<TextView>(R.id.animatedTextView)
        val changeButton = findViewById<Button>(R.id.changeButton)

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(animatedTextView, changeButton)
        } else {
            showViews(animatedTextView, changeButton)
        }

        updateSpannable(animatedTextView)
        animatedTextView.movementMethod = LinkMovementMethod.getInstance()

        changeButton.setOnClickListener {
            val mediaPlayer = MediaPlayer.create(this, R.raw.click_button)
            mediaPlayer.start()
            animateButton(changeButton) {
                var newText: String
                do {
                    newText = words[Random.nextInt(0, words.size)]
                } while (animatedTextView.text == newText)
                animatedTextView.text = newText
                updateSpannable(animatedTextView)
            }
        }
    }

    private fun animateButton(button: Button, onComplete: () -> Unit) {
        val scaleX = ObjectAnimator.ofFloat(button, "scaleX", 1f, 1.5f, 1f)
        val scaleY = ObjectAnimator.ofFloat(button, "scaleY", 1f, 1.5f, 1f)

        val animatorSet = AnimatorSet()
        animatorSet.playTogether(scaleX, scaleY)
        animatorSet.duration = 300L

        animatorSet.addListener(object : AnimatorListenerAdapter() {
            override fun onAnimationEnd(animation: Animator) {
                super.onAnimationEnd(animation)
                onComplete()
            }
        })

        animatorSet.start()
    }

    private fun updateSpannable(textView: TextView) {
        val spannableString = SpannableString(textView.text)
        textView.text.forEachIndexed { index, char ->
            val color = colors[char] ?: Color.BLACK
            spannableString.setSpan(
                object : ClickableSpan() {
                    override fun onClick(widget: View) {
                        animateLetter(widget as TextView, index, color)
                    }
                },
                index,
                index + 1,
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE
            )
        }
        textView.text = spannableString
    }

    private fun animateLetter(view: TextView, index: Int, color: Int) {

        val animatedLetter = TextView(this).apply {
            text = view.text[index].toString()
            textSize = view.textSize / resources.displayMetrics.scaledDensity
            setTextColor(view.currentTextColor)
            typeface = view.typeface
            x = view.x + view.paint.measureText(view.text, 0, index)
            y = view.y
        }

        val parentView = view.parent as ViewGroup
        parentView.addView(animatedLetter)

        val centerX = parentView.width / 2f - animatedLetter.width / 2f - 50
        val centerY = parentView.height / 2f - animatedLetter.height / 2f

        val moveX = ObjectAnimator.ofFloat(animatedLetter, "x", animatedLetter.x, centerX)
        val moveY = ObjectAnimator.ofFloat(animatedLetter, "y", animatedLetter.y, centerY)

        val scaleUpX = ObjectAnimator.ofFloat(animatedLetter, "scaleX", 1f, 10f)
        val scaleUpY = ObjectAnimator.ofFloat(animatedLetter, "scaleY", 1f, 10f)
        val scaleDownX = ObjectAnimator.ofFloat(animatedLetter, "scaleX", 10f, 1f)
        val scaleDownY = ObjectAnimator.ofFloat(animatedLetter, "scaleY", 10f, 1f)

        val moveBackX = ObjectAnimator.ofFloat(animatedLetter, "translationX", centerX, animatedLetter.x)
        val moveBackY = ObjectAnimator.ofFloat(animatedLetter, "translationY", centerY, animatedLetter.y)

        val rotation = ObjectAnimator.ofFloat(view, "rotation", 0f, -720f)

        val duration = 1000L
        listOf(
            scaleUpX, scaleUpY,
            moveX, moveY,
            scaleDownX, scaleDownY,
            moveBackX, moveBackY,
            rotation
        ).forEach {
            it.duration = duration
        }

        view.setTextColor(color - 100)
        scaleUpX.addListener(object : AnimatorListenerAdapter() {
            override fun onAnimationEnd(animation: Animator) {
                super.onAnimationEnd(animation)
                AnimatorSet().apply {
                    playTogether(scaleDownX, scaleDownY, moveBackX, moveBackY)
                    start()
                }
            }
        })

        val animatorSet = AnimatorSet()
        animatorSet.apply {
            playTogether(scaleUpX, scaleUpY, moveX, moveY, rotation)
            start()
        }
        animatorSet.start()

        window.decorView.setBackgroundColor(adjustColorBrightness(color))

        animatorSet.addListener(object : AnimatorListenerAdapter() {
            override fun onAnimationEnd(animation: Animator) {
                super.onAnimationEnd(animation)
                (view.parent as ViewGroup).removeView(animatedLetter)
            }
        })
    }

    private fun adjustColorBrightness(color: Int): Int {
        val factor = 1.3f
        val r = ((Color.red(color) * factor).coerceAtMost(255f)).toInt()
        val g = ((Color.green(color) * factor).coerceAtMost(255f)).toInt()
        val b = ((Color.blue(color) * factor).coerceAtMost(255f)).toInt()
        return Color.rgb(r, g, b)
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