package car.bkrc.com;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.List;

import car.bkrc.com.utils.form;

public class MainActivity extends AppCompatActivity {

    private static final int PICK_IMAGE_REQUEST = 1;
    private ImageView imageView;
    private TextView resultTextView;
    private form formDetector;
    private Bitmap currentBitmap;


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        formDetector = new form();
        imageView = findViewById(R.id.imageView);
        resultTextView = findViewById(R.id.resultTextView);

        setupButtons();
    }
    private Button btnClear;



    private void setupButtons() {
        // 选择图片按钮
        Button btnSelect = findViewById(R.id.selectImageButton);
        btnSelect.setOnClickListener(v -> openImagePicker());

        // 识别按钮
        Button btnDetect = findViewById(R.id.btnDetect);
        btnDetect.setOnClickListener(v -> {
            if (currentBitmap != null) {
                detectAndDisplayResults(currentBitmap);
            } else {
                Toast.makeText(this, "请先选择图片", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void openImagePicker() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, PICK_IMAGE_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null) {
            Uri imageUri = data.getData();
            try {
                currentBitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
                imageView.setImageBitmap(currentBitmap);
            } catch (IOException e) {
                e.printStackTrace();
                Toast.makeText(this, "图片加载失败", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void detectAndDisplayResults(Bitmap bitmap) {
        new Thread(() -> {
            final List<String> results = formDetector.detectMultiple(bitmap);
            runOnUiThread(() -> resultTextView.setText("识别结果：" + results.toString()));
        }).start();
    }
}