package com.vasyukov.laba_20_gps

import android.animation.ObjectAnimator
import android.animation.ValueAnimator
import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.location.Address
import android.location.Geocoder
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.os.Bundle
import android.provider.Settings
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.ContextCompat
import java.util.Locale

class GpsActivity : AppCompatActivity(), LocationListener {

    private lateinit var latitudeTextView: TextView
    private lateinit var longitudeTextView: TextView
    private lateinit var statusTextView: TextView
    private lateinit var locationManager: LocationManager

    private val requestPermissionLauncher = registerForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted ->
            if (isGranted) startTracking() else showPermissionDeniedMessage()
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_gps)

        latitudeTextView = findViewById(R.id.latitudeTextView)
        longitudeTextView = findViewById(R.id.longitudeTextView)
        statusTextView = findViewById(R.id.statusTextView)

        locationManager = getSystemService(LOCATION_SERVICE) as LocationManager

        checkAndRequestPermissions()
        applyAnimations()
    }

    private fun checkAndRequestPermissions() {
        when {
            ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED -> startTracking()
            else -> requestPermissionLauncher.launch(Manifest.permission.ACCESS_FINE_LOCATION)
        }
    }

    private fun startTracking() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            statusTextView.text = "Нет разрешения на доступ к местоположению"
            return
        }

        if (!locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            statusTextView.text = "GPS выключен"
            startActivity(Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS))
        } else {
            statusTextView.text = "GPS включён"
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 2000, 1f, this)
        }
    }

    private fun showPermissionDeniedMessage() {
        statusTextView.text = "Доступ к GPS отклонён. Разрешите доступ в настройках."
    }

    private fun applyAnimations() {
        ValueAnimator.ofFloat(0.3f, 1f).apply {
            duration = 1500
            repeatMode = ValueAnimator.REVERSE
            repeatCount = ValueAnimator.INFINITE
            addUpdateListener { alphaValue ->
                val value = alphaValue.animatedValue as Float
                latitudeTextView.alpha = value
                longitudeTextView.alpha = value
                statusTextView.alpha = value
            }
            start()
        }

        listOf(latitudeTextView, longitudeTextView, statusTextView).forEachIndexed { index, view ->
            ObjectAnimator.ofFloat(view, "translationY", -50f, 0f).apply {
                duration = 1000L + (index * 200)
                start()
            }
        }
    }

    override fun onLocationChanged(location: Location) {
        latitudeTextView.text = "Широта: %.5f".format(location.latitude)
        longitudeTextView.text = "Долгота: %.5f".format(location.longitude)

        val cityName = getCityName(location.latitude, location.longitude)
        statusTextView.text = "Город: $cityName"
    }

    private fun getCityName(latitude: Double, longitude: Double): String {
        val geocoder = Geocoder(this, Locale.getDefault())
        val addresses: List<Address>? = geocoder.getFromLocation(latitude, longitude, 1)
        return if (addresses != null && addresses.isNotEmpty()) {
            addresses[0].locality ?: "Неизвестный город"
        } else {
            "Неизвестный город"
        }
    }
}