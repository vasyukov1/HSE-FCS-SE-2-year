package com.vasyukov.laba_07

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.Context
import android.location.Geocoder
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.KeyEvent
import android.view.View
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import org.osmdroid.config.Configuration
import org.osmdroid.tileprovider.tilesource.XYTileSource
import org.osmdroid.util.GeoPoint
import org.osmdroid.views.MapView
import org.osmdroid.views.overlay.Marker
import org.osmdroid.views.overlay.mylocation.MyLocationNewOverlay
import java.util.Locale

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var map: MapView
    private lateinit var title: TextView
    private lateinit var searchField: EditText
    private lateinit var searchButton: Button
    private lateinit var clearMarkersButton: Button

    private val markers = mutableListOf<Marker>()


    override fun onCreate(savedInstanceState: Bundle?) {

        val context = applicationContext
        Configuration.getInstance().load(context, context.getSharedPreferences("osmdroid", MODE_PRIVATE))
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        title = findViewById(R.id.mapLabel)
        map = findViewById(R.id.map)
        map.setTileSource(org.osmdroid.tileprovider.tilesource.TileSourceFactory.MAPNIK)
        map.setMultiTouchControls(true)

        val startPoint = GeoPoint(55.751244, 37.618423)
        val overlay = MyLocationNewOverlay(map)
        overlay.enableMyLocation()
        map.overlays.add(overlay)
        map.controller.setZoom(12.0)
        map.controller.setCenter(startPoint)

        searchField = findViewById(R.id.searchField)
        searchButton = findViewById(R.id.searchButton)
        clearMarkersButton = findViewById(R.id.clearMarkersButton)

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(title, map, searchField, searchButton, clearMarkersButton)
        } else {
            showViews(title, map, searchField, searchButton, clearMarkersButton)
        }

        searchButton.setOnClickListener {
            find()
        }

        searchField.setOnKeyListener { v, keyCode, event ->
            if (keyCode == KeyEvent.KEYCODE_ENTER && event.action == KeyEvent.ACTION_DOWN) {
                find()
                true
            } else {
                false
            }
        }

        clearMarkersButton.setOnClickListener {
            clearMarkers()
        }

    }

    override fun onResume() {
        super.onResume()
        map.onResume()
    }

    override fun onPause() {
        super.onPause()
        map.onPause()
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

                    showViews(views[0], views[1], views[2], views[3], views[4])
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

    private fun find() {
        val query = searchField.text.toString()
        if (query.isNotEmpty()) {
            searchLocation(query)

            searchField.setText("")

            val imm = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
            imm.hideSoftInputFromWindow(searchField.windowToken, 0)

        } else {
            showAlert("Ошибка", "Введите название места")
        }
    }

    private fun searchLocation(query: String) {
        try {
            val geocoder = Geocoder(this, Locale.getDefault())
            val addresses = geocoder.getFromLocationName(query, 1)

            if (!addresses.isNullOrEmpty()) {
                val address = addresses[0]
                val location = GeoPoint(address.latitude, address.longitude)
                map.controller.setCenter(location)
                map.controller.setZoom(15.0)

                val marker = Marker(map)
                marker.position = location
                marker.setAnchor(Marker.ANCHOR_CENTER, Marker.ANCHOR_BOTTOM)
                marker.title = address.getAddressLine(0)
                map.overlays.add(marker)
                markers.add(marker)
                map.invalidate()
            } else {
                showAlert("Ошибка", "Место не найдено")
            }
        } catch (e: Exception) {
            showAlert("Ошибка", "Не удалось выполнить поиск. Попробуйте ещё раз")
        }

    }

    private fun showAlert(title: String, message: String) {
        AlertDialog.Builder(this)
            .setTitle(title)
            .setMessage(message)
            .setPositiveButton("OK") { dialog, _ -> dialog.dismiss() }
            .show()
    }

    private fun clearMarkers() {
        map.overlays.clear()
        markers.clear()
        map.invalidate()
    }
}