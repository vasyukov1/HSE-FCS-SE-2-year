package com.vasyukov.laba_19_library

import android.graphics.Color
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.Description
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import net.objecthunter.exp4j.ExpressionBuilder

class LibraryActivity : AppCompatActivity() {

    private lateinit var lineChart: LineChart
    private lateinit var inputFunction: EditText
    private lateinit var btnRandomData: Button
    private lateinit var btnCustomFunction: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_library)

        lineChart = findViewById(R.id.lineChart)
        inputFunction = findViewById(R.id.inputFunction)
        btnRandomData = findViewById(R.id.btnRandomData)
        btnCustomFunction = findViewById(R.id.btnCustomFunction)

        btnRandomData.setOnClickListener {
            generateRandomChart()
        }

        btnCustomFunction.setOnClickListener {
            val functionText = inputFunction.text.toString()
            generateFunctionChart(functionText)
        }
    }

    private fun generateRandomChart() {
        val entries = mutableListOf<Entry>()

        for (i in 0..10) {
            entries.add(Entry(i.toFloat(), (Math.random() * 10).toFloat()))
        }

        val dataSet = LineDataSet(entries, "Random Data").apply {
            color = Color.BLUE
            valueTextColor = Color.BLACK
            lineWidth = 2f
        }

        updateChart(dataSet, "График случайных данных")
    }

    private fun generateFunctionChart(functionText: String) {
        val entries = mutableListOf<Entry>()

        try {
            val expression = ExpressionBuilder(functionText)
                .variables("x")
                .build()

            for (i in -10..10) {
                val y = expression.setVariable("x", i.toDouble()).evaluate().toFloat()
                entries.add(Entry(i.toFloat(), y))
            }

            val dataSet = LineDataSet(entries, functionText).apply {
                color = Color.RED
                valueTextColor = Color.BLACK
                lineWidth = 2f
            }

            updateChart(dataSet, "График функции")
        } catch (e: Exception) {
            Toast.makeText(this, "Ошибка в функции", Toast.LENGTH_SHORT).show()
        }
    }

    private fun updateChart(dataSet: LineDataSet, description: String) {
        lineChart.data = LineData(dataSet)
        lineChart.description = Description().apply { text = description }
        lineChart.invalidate()
    }
}
