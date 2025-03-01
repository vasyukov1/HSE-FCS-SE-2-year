package com.vasyukov.laba_12_music

import android.media.audiofx.Visualizer
import android.os.Bundle
import android.util.Log
import android.widget.Button
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.media3.common.AudioAttributes
import androidx.media3.common.MediaItem
import androidx.media3.exoplayer.ExoPlayer

@androidx.media3.common.util.UnstableApi
class MusicActivity : AppCompatActivity() {
    private lateinit var visualizerView: VisualizerView
    private var exoPlayer: ExoPlayer? = null
    private var visualizer: Visualizer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_music)

        visualizerView = findViewById(R.id.visualizerView)
        val playButton = findViewById<Button>(R.id.playButton)

        playButton.setOnClickListener {
            if (exoPlayer == null) {
                startRadio()
            } else {
                stopRadio()
            }
        }
    }

    private fun startRadio() {
        try {
            exoPlayer = ExoPlayer.Builder(this).build().apply {
                setAudioAttributes(
                    AudioAttributes.Builder()
                        .setContentType(androidx.media3.common.C.AUDIO_CONTENT_TYPE_MUSIC)
                        .setUsage(androidx.media3.common.C.USAGE_MEDIA)
                        .build(),
                    true
                )

                val mediaItem = MediaItem.fromUri("http://air.radioart.com/fLounge.mp3")
                setMediaItem(mediaItem)
                prepare()
                play()

                Log.d("MusicActivity", "ExoPlayer запущен!")
            }

            exoPlayer?.audioSessionId?.let { sessionId ->
                if (sessionId != androidx.media3.common.C.AUDIO_SESSION_ID_UNSET) {
                    startVisualizer(sessionId)
                }
            }

        } catch (e: Exception) {
            Log.e("MusicActivity", "Ошибка при запуске ExoPlayer", e)
        }
    }

    private fun startVisualizer(audioSessionId: Int) {
        visualizer = Visualizer(audioSessionId).apply {
            captureSize = Visualizer.getCaptureSizeRange()[1]
            setDataCaptureListener(object : Visualizer.OnDataCaptureListener {
                override fun onWaveFormDataCapture(visualizer: Visualizer?, bytes: ByteArray?, samplingRate: Int) {
                    if (bytes != null) {
                        visualizerView.updateWaveform(bytes)
                    }
                }

                override fun onFftDataCapture(visualizer: Visualizer?, bytes: ByteArray?, samplingRate: Int) {}
            }, Visualizer.getMaxCaptureRate(), true, false)

            enabled = true
        }
    }

    private fun stopRadio() {
        exoPlayer?.release()
        exoPlayer = null
        visualizer?.release()
        visualizer = null
        Log.d("MusicActivity", "ExoPlayer и визуализатор остановлены!")
    }

    override fun onDestroy() {
        super.onDestroy()
        stopRadio()
    }
}
