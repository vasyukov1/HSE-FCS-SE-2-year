package com.vasyukov.laba_12_music

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Path
import android.util.AttributeSet
import android.view.View

class VisualizerView(context: Context, attrs: AttributeSet) : View(context, attrs) {
    private val paint = Paint().apply {
        color = Color.BLUE
        strokeWidth = 4f
        style = Paint.Style.STROKE
    }

    private val wavePath = Path()
    private var waveform: ByteArray? = null

    fun updateWaveform(bytes: ByteArray) {
        waveform = bytes
        invalidate()
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        waveform?.let { drawWaveform(canvas, it) }
    }

    private fun drawWaveform(canvas: Canvas, bytes: ByteArray) {
        wavePath.reset()
        val midY = height / 2f

        for (i in bytes.indices step 2) {
            val x = width * i / bytes.size.toFloat()
            val y = midY + (bytes[i] / 128f) * midY
            if (i == 0) wavePath.moveTo(x, y) else wavePath.lineTo(x, y)
        }

        canvas.drawPath(wavePath, paint)
    }
}
