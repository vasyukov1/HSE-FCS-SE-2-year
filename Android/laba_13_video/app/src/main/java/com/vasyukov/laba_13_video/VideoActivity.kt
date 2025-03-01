package com.vasyukov.laba_13_video

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.GestureDetector
import android.view.MotionEvent
import android.webkit.WebChromeClient
import android.webkit.WebSettings
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import kotlin.random.Random

class VideoActivity : AppCompatActivity() {
    private lateinit var webView: WebView
    private lateinit var gestureDetector: GestureDetector

    @SuppressLint("SetJavaScriptEnabled")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        webView = WebView(this).apply {
            settings.javaScriptEnabled = true
            settings.domStorageEnabled = true
            settings.mediaPlaybackRequiresUserGesture = false
            settings.loadWithOverviewMode = true
            settings.useWideViewPort = true
            settings.cacheMode = WebSettings.LOAD_NO_CACHE
            webViewClient = WebViewClient()
            webChromeClient = WebChromeClient()
        }

        setContentView(webView)

        gestureDetector = GestureDetector(this, object : GestureDetector.SimpleOnGestureListener() {
            override fun onFling(
                e1: MotionEvent?, e2: MotionEvent,
                velocityX: Float, velocityY: Float
            ): Boolean {
                if (e1 != null) {
                    val deltaY = e2.y - e1.y
                    if (deltaY < -100) {
                        loadRandomShorts()
                        return true
                    } else if (deltaY > 100) {
                        loadRandomShorts()
                        return true
                    }
                }
                return false
            }
        })

        loadRandomShorts()
    }

    private fun loadRandomShorts() {
        val videoId = getRandomShortsId()
        val shortsUrl = "https://www.youtube.com/shorts/$videoId"
        webView.loadUrl(shortsUrl)
    }

    private fun getRandomShortsId(): String {
        val shortsList = listOf(
            "Kj4UyMmMUj8"
        )
        return shortsList[Random.nextInt(shortsList.size)]
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        return gestureDetector.onTouchEvent(event) || super.onTouchEvent(event)
    }
}