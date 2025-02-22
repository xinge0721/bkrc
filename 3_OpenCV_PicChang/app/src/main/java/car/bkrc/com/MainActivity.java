package car.bkrc.com;

import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    static {
        if (!OpenCVLoader.initDebug()) {
            System.out.println("OpenCV not loaded");
        } else {
            System.out.println("OpenCV loaded");
        }
    }

    private ImageView imageView;
    private SeekBar thresholdSeekBar;
    private TextView thresholdValueText;
    private Spinner colorSpinner;
    private Button saveButton;
    private Mat originalImage;
    private int currentThreshold = 100;
    private Map<String, Integer> colorThresholds = new HashMap<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imageView = findViewById(R.id.imageView);
        thresholdSeekBar = findViewById(R.id.thresholdSeekBar);
        thresholdValueText = findViewById(R.id.thresholdValueText);
        colorSpinner = findViewById(R.id.colorSpinner);
        saveButton = findViewById(R.id.saveButton);

        // Set up the spinner with color options
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.colors_array, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        colorSpinner.setAdapter(adapter);

        // Copy the sample image from assets to internal storage
        copyAssetToInternalStorage("sample_image.jpg", "sample_image.jpg");

        // Load the sample image
        originalImage = Imgcodecs.imread(getFilesDir() + "/sample_image.jpg");

        // Initialize the image view with the original image
        updateImageView(originalImage);

        thresholdSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                currentThreshold = progress;
                thresholdValueText.setText("Threshold: " + currentThreshold);
                processImageWithThreshold(currentThreshold);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String selectedColor = colorSpinner.getSelectedItem().toString();
                colorThresholds.put(selectedColor, currentThreshold);
                // Optionally, you can save these thresholds to a file or database here
                // For now, we'll just print them to logcat
                StringBuilder sb = new StringBuilder("Saved Thresholds:\n");
                for (Map.Entry<String, Integer> entry : colorThresholds.entrySet()) {
                    sb.append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
                }
                System.out.println(sb.toString());
            }
        });

        // Process the image with the initial threshold
        processImageWithThreshold(currentThreshold);
    }

    private void copyAssetToInternalStorage(String assetFileName, String destinationPath) {
        try {
            InputStream inputStream = getAssets().open(assetFileName);
            OutputStream outputStream = openFileOutput(destinationPath, MODE_PRIVATE);
            byte[] buffer = new byte[1024];
            int length;
            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }
            inputStream.close();
            outputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void updateImageView(Mat mat) {
        MatOfByte buffer = new MatOfByte();
        Imgcodecs.imencode(".jpg", mat, buffer);
        byte[] byteArray = buffer.toArray();
        Bitmap bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.length);
        imageView.setImageBitmap(bitmap);
    }

    private void processImageWithThreshold(int threshold) {
        Mat gray = new Mat();
        Mat blurred = new Mat();
        Mat binary = new Mat();

        Imgproc.cvtColor(originalImage, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.GaussianBlur(gray, blurred, new Size(5, 5), 0);
        Imgproc.threshold(blurred, binary, threshold, 255, Imgproc.THRESH_BINARY);

        // Update the image view with the processed image
        updateImageView(binary);
    }
}