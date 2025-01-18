package com.vasyukov.laba_04

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Parcel
import android.os.Parcelable
import android.util.AttributeSet
import android.view.View
import kotlin.random.Random

class SnakeView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    val snakeSegments = mutableListOf<Pair<Float, Float>>()
    val snakeColors = mutableListOf<Int>()
    var directionAngle = Random.nextDouble(0.0, 360.0)
    var headX = 0f
    var headY = 0f
    var colorDarkMode = true

    private val segmentLength = 30f
    private val snakeLength = 20

    init {
        generateSnake()
    }

    private fun generateSnake() {
        snakeSegments.clear()
        snakeColors.clear()
        headX = width / 2f
        headY = headY / 2f

        for (i in 0 until snakeLength) {
            snakeSegments.add(headX to headY)
            snakeColors.add(generateRandomColor(colorDarkMode))
        }
    }

    private fun generateRandomColor(isDarkMode: Boolean): Int {
        if (isDarkMode) {
            return Color.rgb(
                (200..255).random(),
                (200..255).random(),
                (200..255).random()
            )
        }
        return Color.rgb(
            (0..100).random(),
            (0..100).random(),
            (0..100).random(),
        )

    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        for (i in snakeSegments.indices) {
            paint.color = snakeColors[i]
            canvas.drawCircle(snakeSegments[i].first, snakeSegments[i].second, 15f, paint)
        }
    }

    fun updateSnakeColor(isDarkMode: Boolean) {
        snakeColors.clear()
        repeat(snakeSegments.size) {
            snakeColors.add(generateRandomColor(isDarkMode))
        }
    }

    fun updateSnake(isDarkMode: Boolean) {
        directionAngle += Random.nextDouble(-30.0, 30.0)
        val dx = (segmentLength * Math.cos(Math.toRadians(directionAngle))).toFloat()
        val dy = (segmentLength * Math.sin(Math.toRadians(directionAngle))).toFloat()

        headX += dx
        headY += dy

        if (headX < 0 || headX > width) directionAngle = 180 - directionAngle
        if (headY < 0 || headY > height) directionAngle = -directionAngle

        snakeSegments.add(0, headX to headY)
        if (snakeSegments.size > snakeLength) {
            snakeSegments.removeAt(snakeSegments.size - 1)
        }

        snakeColors.add(0, generateRandomColor(isDarkMode))
        if (snakeColors.size > snakeLength) {
            snakeColors.removeAt(snakeColors.size - 1)
        }

        invalidate()
    }
}

data class SnakeSegment(val x: Float, val y: Float) : Parcelable {
    constructor(parcel: Parcel) : this(
        parcel.readFloat(),
        parcel.readFloat()
    )

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeFloat(x)
        parcel.writeFloat(y)
    }

    override fun describeContents(): Int = 0

    companion object CREATOR : Parcelable.Creator<SnakeSegment> {
        override fun createFromParcel(parcel: Parcel): SnakeSegment {
            return SnakeSegment(parcel)
        }

        override fun newArray(size: Int): Array<SnakeSegment?> {
            return arrayOfNulls(size)
        }
    }
}